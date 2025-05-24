// Dummy LittleFS.h for native builds (testing only)
#ifndef DUMMY_LITTLEFS_H
#define DUMMY_LITTLEFS_H

#include <string>
#include <cstddef>

// Minimal mock Arduino String class for testing
class String {
public:
    String() : _str("") {}
    String(const char* s) : _str(s) {}
    String(const std::string& s) : _str(s) {}
    operator const char*() const { return _str.c_str(); }
    int toInt() const { return std::stoi(_str); }
    float toFloat() const { return std::stof(_str); }
    // Add more methods as needed
    std::string _str;
};

// Dummy File class for native builds (testing only)
class File {
public:
    File() : _open(false) {}
    operator bool() const { return _open; }
    bool available() { return false; }
    void close() {}
    int printf(const char*, ...) { return 0; }
    std::string _buffer;
    String readStringUntil(char) { return String("0"); }
    size_t size() { return 0; }
private:
    bool _open;
};

class LittleFSClass {
public:
    bool begin() { return false; }
    void end() {}
    bool format() { return false; }
    File open(const char* /*filename*/, const char* /*mode*/) { return File(); }
    File open(const String& /*filename*/, const char* /*mode*/) { return File(); }
    // Add more dummy methods as needed for compilation
};
static LittleFSClass LittleFS;

#endif // DUMMY_LITTLEFS_H
