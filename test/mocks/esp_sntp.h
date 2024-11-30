#ifndef MOCK_ESP_SNTP_H
#define MOCK_ESP_SNTP_H

inline void configTime(long gmtOffset_sec, int daylightOffset_sec, const char* server1, const char* server2 = nullptr, const char* server3 = nullptr) {
    // Mock implementation: do nothing
}

#endif // MOCK_ESP_SNTP_H