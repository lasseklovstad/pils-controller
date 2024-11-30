#ifndef TIME_UTILS_H
#define TIME_UTILS_H
#include <Arduino.h>

void configureTime() {};

time_t getTimeStamp() { return 1000; };

void configTime(long gmtOffset_sec, int daylightOffset_sec, const char *server1) {};

#endif