#include "Controller.h"
#include <logging.h>

Controller::Controller(const int controllerId, const char *apiKey, const int relayOutput) : mode(Mode::NONE), status(Status::INACTIVE), controllerId(controllerId), apiKey(apiKey), temperature(-127), relayOutput(relayOutput), isSourceOn(false), lastSwitchTimestamp(0), minSwitchDelay(30000), targetTemperature(14)
{
    parseTemperaturePeriods("");
}

void Controller::update(const String &mode, const String &status, const String &temperaturePeriods, const String &minDelayInSec, const String &avgBufferSize, const String &hysteresis)
{
    this->mode = stringToMode(mode);
    this->status = stringToStatus(status);
    this->temperaturePeriods.clear();
    parseTemperaturePeriods(temperaturePeriods);

    // Handle minDelayInSec
    if (minDelayInSec.length() > 0)
    {
        this->minSwitchDelay = minDelayInSec.toInt() * 1000; // Convert seconds to milliseconds
    }
    else
    {
        this->minSwitchDelay = 30000; // Default to 30 seconds
    }

    // Handle avgBufferSize
    if (avgBufferSize.length() > 0)
    {
        this->maxBufferSize = avgBufferSize.toInt();
    }
    else
    {
        this->maxBufferSize = 5; // Default buffer size
    }

    // Handle hysteresis
    if (hysteresis.length() > 0)
    {
        this->hysteresis = hysteresis.toFloat();
    }
    else
    {
        this->hysteresis = 0; // Default hysteresis
    }
}

Mode Controller::stringToMode(const String &modeStr)
{
    if (modeStr.equalsIgnoreCase("warm"))
    {
        return Mode::WARM;
    }
    else if (modeStr.equalsIgnoreCase("cold"))
    {
        return Mode::COLD;
    }
    else
    {
        return Mode::NONE;
    }
}

Status Controller::stringToStatus(const String &statusStr)
{
    if (statusStr.equalsIgnoreCase("active"))
    {
        return Status::ACTIVE;
    }
    else if (statusStr.equalsIgnoreCase("prepare"))
    {
        return Status::PREPARE;
    }
    else
    {
        return Status::INACTIVE;
    }
}

void Controller::parseTemperaturePeriods(const String &periods)
{
    int start = 0;
    int end = periods.indexOf(';');
    while (end != -1)
    {
        String segment = periods.substring(start, end);
        int dashIndex = segment.indexOf('-');
        if (dashIndex != -1)
        {
            unsigned long timestamp = segment.substring(0, dashIndex).toInt();
            float temperature = segment.substring(dashIndex + 1).toFloat();
            temperaturePeriods.emplace_back(timestamp, temperature);
        }
        start = end + 1;
        end = periods.indexOf(';', start);
    }
    // Handle the last segment
    String segment = periods.substring(start);
    int dashIndex = segment.indexOf('-');
    if (dashIndex != -1)
    {
        unsigned long timestamp = segment.substring(0, dashIndex).toInt();
        float temperature = segment.substring(dashIndex + 1).toFloat();
        temperaturePeriods.emplace_back(timestamp, temperature);
    }
}

float Controller::getTargetTemperature(unsigned long currentTimestamp)
{
    if (temperaturePeriods.size() < 1)
    {
        return targetTemperature;
    }

    if (status == Status::PREPARE || currentTimestamp <= temperaturePeriods[0].first)
    {
        // If the controller is preparing for fermentation return the first temperature
        return temperaturePeriods[0].second;
    }

    for (size_t i = 0; i < temperaturePeriods.size() - 1; ++i)
    {
        if (currentTimestamp >= temperaturePeriods[i].first && currentTimestamp < temperaturePeriods[i + 1].first)
        {
            return temperaturePeriods[i].second;
        }
    }
    return temperaturePeriods.back().second; // Return the last temperature if currentTimestamp is beyond the last timestamp
}

void Controller::updateSource(unsigned long currentTimestamp)
{
    if (status == Status::INACTIVE)
    {
        return;
    }

    targetTemperature = getTargetTemperature(currentTimestamp);
    float averageTemperature = calculateMovingAverage();
    LOG_DEBUG("Target temperature: " + String(targetTemperature));
    LOG_DEBUG("Average temperature: " + String(averageTemperature));

    if (mode == Mode::COLD)
    {
        if (!isSourceOn && averageTemperature > (targetTemperature + hysteresis))
        {
            turnOnSource();
        }
        else if (isSourceOn && averageTemperature <= (targetTemperature - hysteresis))
        {
            turnOffSource();
        }
    }
    else if (mode == Mode::WARM)
    {
        if (!isSourceOn && averageTemperature < (targetTemperature - hysteresis))
        {
            turnOnSource();
        }
        else if (isSourceOn && averageTemperature >= (targetTemperature + hysteresis))
        {
            turnOffSource();
        }
    }
    else if (mode == Mode::NONE)
    {
        LOG_ERROR("Mode none, this should not happen");
        turnOffSource();
    }
}

void Controller::turnOnSource()
{
    if (millis() - lastSwitchTimestamp < minSwitchDelay)
    {
        LOG_DEBUG("Switching too soon, ignoring turnOnSource request");
        return;
    }
    digitalWrite(relayOutput, HIGH);
    LOG_DEBUG("Turn on source");
    isSourceOn = true;
    lastSwitchTimestamp = millis();
}

void Controller::turnOffSource()
{
    if (millis() - lastSwitchTimestamp < minSwitchDelay)
    {
        LOG_DEBUG("Switching too soon, ignoring turnOffSource request");
        return;
    }
    digitalWrite(relayOutput, LOW);
    LOG_DEBUG("Turn off source");
    isSourceOn = false;
    lastSwitchTimestamp = millis();
}

void Controller::setTemperature(float temp)
{
    temperature = temp;

    // Add the new temperature to the buffer
    temperatureBuffer.push_back(temp);

    // Ensure the buffer does not exceed the maximum size
    if (temperatureBuffer.size() > maxBufferSize)
    {
        temperatureBuffer.pop_front();
    }
}

float Controller::calculateMovingAverage()
{
    if (temperatureBuffer.empty())
    {
        return temperature; // Default to the current temperature if buffer is empty
    }

    float sum = 0;
    for (float temp : temperatureBuffer)
    {
        sum += temp;
    }
    return sum / temperatureBuffer.size();
}

void Controller::reset()
{
    mode = Mode::NONE;
    status = Status::INACTIVE;
    temperaturePeriods.clear();
    temperature = -127; // Reset to default invalid temperature
    isSourceOn = false;
    lastSwitchTimestamp = 0;
    minSwitchDelay = 30000; // Reset to default 30 seconds
    temperatureBuffer.clear();
    maxBufferSize = 5; // Reset to default buffer size
    hysteresis = 0;    // Reset to default hysteresis
    targetTemperature = 14;
}

void Controller::saveStateToFile()
{
    if (!LittleFS.begin())
    {
        LOG_ERROR("Failed to mount LittleFS");
        return;
    }
    String filename = "/controller_state_" + String(controllerId) + ".txt";
    File file = LittleFS.open(filename, "w");
    if (!file)
    {
        LOG_ERROR("Failed to open state file for writing");
        return;
    }
    file.printf("%d\n%d\n%f\n", (int)status, (int)mode, targetTemperature);
    file.close();
    LOG_DEBUG("Controller state saved to: " + filename);
}

void Controller::loadStateFromFile()
{
    if (!LittleFS.begin())
    {
        LOG_ERROR("Failed to mount LittleFS");
        return;
    }
    String filename = "/controller_state_" + String(controllerId) + ".txt";
    File file = LittleFS.open(filename, "r");
    if (!file)
    {
        LOG_DEBUG("No state file found, using defaults");
        return;
    }
    int s, m;
    float t;
    if (file.available())
    {
        s = file.readStringUntil('\n').toInt();
        m = file.readStringUntil('\n').toInt();
        t = file.readStringUntil('\n').toFloat();
        status = (Status)s;
        mode = (Mode)m;
        targetTemperature = t;

        LOG_DEBUG("Controller state loaded from: " + filename);
        LOG_DEBUG("Status: " + String(s) + ", Mode: " + String(m) + ", Target temp: " + String(t));
    }
    file.close();
}