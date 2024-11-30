#ifndef MOCK_LITTLEFS_H
#define MOCK_LITTLEFS_H

#include <Arduino.h>

class File {
public:
    bool available() { return true; }
    size_t size() { return 1024; }
    void close() {}
    // Add other necessary mock methods
};

class LittleFSClass {
public:
    bool begin(bool formatOnFail = false) { return true; }
    File open(const char* path, const char* mode) { return File(); }
    // Add other necessary mock methods
};

extern LittleFSClass LittleFS;

#endif // MOCK_LITTLEFS_H