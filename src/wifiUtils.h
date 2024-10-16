
#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H
#include <Arduino.h>
#include <WiFi.h>

void scanAndConnect(const char* ssidList[], const char* passwordList[], int numSavedNetworks);

#endif