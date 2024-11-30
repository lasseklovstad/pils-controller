#ifndef MOCK_WIFICLIENTSECURE_H
#define MOCK_WIFICLIENTSECURE_H

#include <Arduino.h>
#include <WiFi.h>

class WiFiClientSecure {
public:
    void setCACert(const char *rootCA) {}
    bool loadCACert(Stream &stream, size_t size) {
        // Mock implementation: always return true for successful loading
        return true;
    }
};

#endif // MOCK_WIFICLIENTSECURE_H