#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include <vector>
#include <deque>

enum Mode
{
    WARM,
    COLD,
    NONE
};

enum Status
{
    ACTIVE,
    PREPARE,
    INACTIVE
};

class Controller
{
public:
    Controller(const int controllerId, const char *apiKey, const int relayOutput);
    bool shouldTurnOnSource(unsigned long currentTimestamp);
    void updateSource(unsigned long currentTimestamp);
    void update(const String &mode, const String &status, const String &temperaturePeriods);
    void setTemperature(float temp);

    inline float getTemperature() const { return temperature; }
    inline Mode getMode() const { return mode; }
    inline Status getStatus() const { return status; }
    inline const std::vector<std::pair<unsigned long, float>> &getTemperaturePeriods() const { return temperaturePeriods; }
    inline const char *getApiKey() { return apiKey; };
    inline const int getControllerId() { return controllerId; };

private:
    Mode mode;
    Status status;
    std::vector<std::pair<unsigned long, float>> temperaturePeriods;
    float temperature;
    const int controllerId;
    const char *apiKey;
    const int relayOutput;
    boolean isSourceOn;
    unsigned long lastSwitchTimestamp;
    unsigned long minSwitchDelay; // Minimum delay between source state changes

    /**  Returns the target temperature for the current timestamp from the temperature periods  */
    float getTargetTemperature(unsigned long currentTimestamp);
    void parseTemperaturePeriods(const String &periods);
    Mode stringToMode(const String &modeStr);
    Status stringToStatus(const String &statusStr);
    void turnOnSource();
    void turnOffSource();

    std::deque<float> temperatureBuffer; // Buffer to store recent temperature readings
    const size_t maxBufferSize = 5; // Maximum size of the buffer for moving average

    /** Calculates the moving average of the temperature readings */
    float calculateMovingAverage();
};

#endif // CONTROLLER_H