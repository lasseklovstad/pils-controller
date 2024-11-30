#ifndef TIME_UTILS_H
#define TIME_UTILS_H
#include <Arduino.h>
#include <time.h>

#ifdef UNIT_TEST
// Declare the mock configTime function for unit tests
void configTime(long gmtOffset_sec, int daylightOffset_sec, const char* server1, const char* server2 = nullptr, const char* server3 = nullptr);
#endif

void configureTime();

time_t getTimeStamp();

#endif