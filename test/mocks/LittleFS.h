#ifndef MOCK_LITTLEFS_H
#define MOCK_LITTLEFS_H

#include <Arduino.h>

class File : public Stream {
public:
    int available() { return 1; }
    size_t size() { return 1024; }
    void close() {}
    
    int read() override { return -1; }
    int peek() override { return -1; }
    void flush() override {}
    size_t write(uint8_t) override { return 0; }
    int read(uint8_t *buffer, size_t size) { return 0; }
    size_t write(const uint8_t *buffer, size_t size) override { return size; }
    int availableForWrite() { return 0; } // Removed override keyword
    operator bool() const { return true; } // Added operator bool()
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