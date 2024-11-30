#ifndef MOCK_HTTPCLIENT_H
#define MOCK_HTTPCLIENT_H

#include <Arduino.h>

class HTTPClient {
public:
    int begin(const std::string& url) {
        // Mock begin behavior
        return 1;
    }

    int GET() {
        // Mock GET response
        return 200;
    }

    String getString() {
        // Mock HTTP response body
        return "{\"mockKey\":\"mockValue\"}";
    }

    void end() {
        // Mock end of request
    }
};

#endif // MOCK_HTTPCLIENT_H
