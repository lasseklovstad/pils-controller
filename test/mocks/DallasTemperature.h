#ifndef MOCK_DALLASTEMPERATURE_H
#define MOCK_DALLASTEMPERATURE_H

#include <OneWire.h>

class DallasTemperature {
public:
    DallasTemperature(OneWire* oneWire) {}

    void begin() {}

    void requestTemperatures() {}

    float getTempCByIndex(int index) {
        // Mock temperature value
        return 25.0;
    }
};

#endif // MOCK_DALLASTEMPERATURE_H