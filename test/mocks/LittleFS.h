// Dummy LittleFS.h for native builds (testing only)
#ifndef DUMMY_LITTLEFS_H
#define DUMMY_LITTLEFS_H

#include <string>
#include <cstddef>

// Dummy File class for native builds (testing only)
class File {
public:
    File() : _open(false) {}
    operator bool() const { return _open; }
    bool available() { return false; }
    void close() {}
    int printf(const char*, ...) { return 0; }
    std::string _buffer;
    int readStringUntil(char) { return 0; }
    int toInt() { return 0; }
    float toFloat() { return 0.0f; }
    size_t size() { return 0; }
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
