// Dummy LittleFS.h for native builds (testing only)
#ifndef DUMMY_LITTLEFS_H
#define DUMMY_LITTLEFS_H

class LittleFSClass {
public:
    bool begin() { return false; }
    void end() {}
    bool format() { return false; }
    bool open(const char*, const char*) { return false; }
    // Add more dummy methods as needed for compilation
};
static LittleFSClass LittleFS;

#endif // DUMMY_LITTLEFS_H
