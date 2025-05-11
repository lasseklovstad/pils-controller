#include "Controller.h"
#include <logging.h>

Controller::Controller(const int controllerId, const char *apiKey, const int relayOutput) : mode(Mode::NONE), status(Status::INACTIVE), controllerId(controllerId), apiKey(apiKey), temperature(-127), relayOutput(relayOutput), isSourceOn(false)
{
    parseTemperaturePeriods("");
}

void Controller::update(const String &mode, const String &status, const String &temperaturePeriods)
{
    this->mode = stringToMode(mode);
    this->status = stringToStatus(status);
    this->temperaturePeriods.clear();
    parseTemperaturePeriods(temperaturePeriods);
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
        return 18.0;
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

bool Controller::shouldTurnOnSource(unsigned long currentTimestamp)
{
    if (status == Status::INACTIVE)
    {
        return false;
    }

    float currentTemperature = getTargetTemperature(currentTimestamp);
    LOG_DEBUG("Target temperature: " + String(currentTemperature));

    if (mode == Mode::COLD)
    {
        return temperature > currentTemperature;
    }
    else if (mode == Mode::WARM)
    {
        return temperature < currentTemperature;
    }
    else if (mode == Mode::NONE)
    {
        LOG_ERROR("Mode none, this should not happen");
    }
    return false; // Default case if mode is not recognized
}

void Controller::updateSource(unsigned long currentTimestamp)
{
    if (status == Status::INACTIVE)
    {
        return;
    }

    float currentTemperature = getTargetTemperature(currentTimestamp);
    LOG_DEBUG("Target temperature: " + String(currentTemperature));

    if (mode == Mode::COLD)
    {
        bool shouldTurnOnSource = temperature > currentTemperature;
        if (shouldTurnOnSource)
        {
            turnOnSource();
        }
        else
        {
            turnOffSource();
        }
    }
    else if (mode == Mode::WARM)
    {
        bool shouldTurnOnSource = temperature < currentTemperature;
        if (shouldTurnOnSource)
        {
            turnOnSource();
        }
        else
        {
            turnOffSource();
        }
    }
    else if (mode == Mode::NONE)
    {
        LOG_ERROR("Mode none, this should not happen");
    }
}

void Controller::turnOnSource()
{
    digitalWrite(relayOutput, HIGH);
    isSourceOn = true;
}

void Controller::turnOffSource()
{
    digitalWrite(relayOutput, LOW);
    isSourceOn = false;
}