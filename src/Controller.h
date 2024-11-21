#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include <vector>

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
    Controller(const int controllerId, const char *apiKey);
    bool shouldTurnOnSource(unsigned long currentTimestamp);
    void update(const String &mode, const String &status, const String &temperaturePeriods);
    
    inline float getTemperature() const { return temperature; }
    inline void setTemperature(float temp) { temperature = temp; }
    inline const char * getApiKey() { return apiKey; };
    inline const int getControllerId() { return controllerId; };

private:
    Mode mode;
    Status status;
    std::vector<std::pair<unsigned long, float>> temperaturePeriods;
    float temperature;
    const int controllerId;
    const char *apiKey;

    /**  Returns the target temperature for the current timestamp from the temperature periods  */
    float getTargetTemperature(unsigned long currentTimestamp);
    void parseTemperaturePeriods(const String &periods);
    Mode stringToMode(const String &modeStr);
    Status stringToStatus(const String &statusStr);
};

#endif // CONTROLLER_H