// Dummy LittleFS.h for native builds (testing only)
#ifndef DUMMY_LITTLEFS_H
#define DUMMY_LITTLEFS_H
#include <Arduino.h>

// Dummy File class for native builds (testing only)
class File {
public:
    File() : _open(false) {}
    operator bool() const { return _open; }
    bool available() { return false; }
    void close() {}
    void printf(const char*, ...) { }
    String readStringUntil(char) { return String(""); }
private:
    bool _open;
};

class LittleFSClass {
public:
    bool begin() { return false; }
    void end() {}
    bool format() { return false; }
    File open(const char*, const char*) { return File(); }
    // Add more dummy methods as needed for compilation
};
static LittleFSClass LittleFS;

#endif // DUMMY_LITTLEFS_H
