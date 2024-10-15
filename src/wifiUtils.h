
#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H
#include <Arduino.h>
#include <WiFi.h>

void connectToWiFi(const char *ssid, const char *password);

void scanNetworks();

#endif