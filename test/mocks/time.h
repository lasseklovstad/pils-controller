#ifndef MOCK_TIME_H
#define MOCK_TIME_H

typedef long time_t;

struct tm {
    int tm_sec;   // seconds after the minute [0-60]
    int tm_min;   // minutes after the hour [0-59]
    int tm_hour;  // hours since midnight [0-23]
    int tm_mday;  // day of the month [1-31]
    int tm_mon;   // months since January [0-11]
    int tm_year;  // years since 1900
    int tm_wday;  // days since Sunday [0-6]
    int tm_yday;  // days since January 1 [0-365]
    int tm_isdst; // Daylight Saving Time flag
};

time_t now;

void configTime(int timezone, int daylightOffset_sec, const char* server1, const char* server2 = nullptr, const char* server3 = nullptr) {
    // Mock implementation
}

time_t time(time_t* t) {
    if (t) {
        *t = now;
    }
    return now;
}

#endif // MOCK_TIME_H