#ifndef TIME_UTILS_H
#define TIME_UTILS_H
#include <Arduino.h>
#include <time.h>

// Get current epoch time
extern time_t now;

void configureTime();

time_t getTimeStamp();

#endif