#ifndef MOCK_WIFI_H
#define MOCK_WIFI_H

#define WL_CONNECTED 3
#define WIFI_STA 1

#include "Arduino.h"

class WiFiClass {
public:
    int status() {
        return WL_CONNECTED;
    }
    void begin(const char* ssid, const char* password) {
        // Mock implementation
    }
    void mode(int mode) {
        // Mock implementation
    }
    void disconnect() {
        // Mock implementation
    }
    int scanNetworks() {
        return 5; // Mock number of networks found
    }
    String SSID(int index) {
        return "MockSSID"; // Mock SSID
    }
};

extern WiFiClass WiFi;

#endif // MOCK_WIFI_H