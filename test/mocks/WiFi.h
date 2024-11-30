#ifndef MOCK_WIFI_H
#define MOCK_WIFI_H

#define WL_CONNECTED 3

class WiFiClass {
public:
    int status() {
        return WL_CONNECTED;
    }
};

extern WiFiClass WiFi;

#endif