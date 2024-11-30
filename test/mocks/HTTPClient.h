#ifndef MOCK_HTTPCLIENT_H
#define MOCK_HTTPCLIENT_H

#include <Arduino.h>
#include <WiFiClientSecure.h>

class HTTPClient {
public:
    void begin(WiFiClientSecure &client, const String &url) {}
    void addHeader(const String &name, const String &value) {}
    int GET() {
        return 200; // Mock GET response code
    }
    String getString() {
        return "{\"mockKey\":\"mockValue\"}"; // Mock response body
    }
    void end() {}
    String header(const String &name) {
        return "mockHeader"; // Mock header value
    }
    void collectHeaders(const char **headerKeys, size_t headerKeysCount) {}
};

#endif // MOCK_HTTPCLIENT_H