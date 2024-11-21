#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <LittleFS.h> 
#include <mbedtls/md.h>
#include "Controller.h"

void setupHttps();
void postTemperature(Controller&);
void updateControllerActiveBatch(Controller&);

#endif