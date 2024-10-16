#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#include <HTTPClient.h>
#include <mbedtls/md.h>

void postTemperature(float temperature, const int controllerId, const char *apiSecret);
bool getControllerRelayOn(const int controllerId, const char *apiSecret);

#endif