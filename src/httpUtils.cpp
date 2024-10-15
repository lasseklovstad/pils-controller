#include "httpUtils.h"
#include "timeUtils.h"

const char *serverURL = "https://pils.gataersamla.no/api/controller";

void addAuthenticationHeaders(HTTPClient &http, const char *apiSecret)
{
    // Generate a nonce (random number)
    unsigned long nonce = random(1000000, 9999999);
    unsigned long timestamp = getTimeStamp();
    char data[50];                              // Buffer to hold the combined data
    sprintf(data, "%lu:%lu", timestamp, nonce); // "timestamp:nonce" format

    // Compute HMAC-SHA256
    uint8_t hmacResult[32]; // Array to store the HMAC result (SHA-256 is 32 bytes)

    // Create an instance of the message digest context
    mbedtls_md_context_t ctx;
    mbedtls_md_init(&ctx);

    // Use SHA-256 as the hash function
    const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

    // Initialize the HMAC context with the selected hash function
    mbedtls_md_setup(&ctx, md_info, 1); // 1 means we're using HMAC

    // Start the HMAC calculation with the secret key
    mbedtls_md_hmac_starts(&ctx, (const unsigned char *)apiSecret, strlen(apiSecret));

    // Add the timestamp:nonce data to the HMAC
    mbedtls_md_hmac_update(&ctx, (const unsigned char *)data, strlen(data));

    // Finish the HMAC calculation and store the result in hmacResult
    mbedtls_md_hmac_finish(&ctx, hmacResult);

    // Clean up
    mbedtls_md_free(&ctx);

    // Convert the HMAC result to a hex string
    char hmacHex[65]; // Buffer to hold the HMAC in hex (64 characters + null terminator)
    for (int i = 0; i < 32; i++)
    {
        sprintf(&hmacHex[i * 2], "%02x", hmacResult[i]);
    }

    http.addHeader("HMAC", hmacHex);                // Add HMAC header
    http.addHeader("Timestamp", String(timestamp)); // Add Timestamp header
    http.addHeader("Nonce", String(nonce));         // Add Nonce header
}

void postTemperature(float temperature, const int controllerId, const char *apiSecret)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("POST temperature");
        HTTPClient http; // Declare an HTTPClient object

        // Specify the server URL
        http.begin(serverURL + String("/") + String(controllerId));

        // Set request header
        http.addHeader("Content-Type", "plain/text");
        addAuthenticationHeaders(http, apiSecret);

        // payload to send
        String text = String(temperature);

        // Send the POST request
        int httpResponseCode = http.POST(text);

        // Print the response code
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        // If there is a response, print the response payload
        if (httpResponseCode != 200)
        {
            String response = http.getString();
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
            Serial.println("Server response:");
            Serial.println(response);
        }

        // End the HTTP connection
        http.end();
    }
    else
    {
        Serial.println("Error: Not connected to Wi-Fi");
    }
}

bool getControllerRelayOn(const int controllerId, const char *apiSecret)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Get controller relay on request");
        HTTPClient http; // Declare an HTTPClient object

        // Specify the server URL
        http.begin(serverURL + String("/") + String(controllerId));

        // Set request header
        addAuthenticationHeaders(http, apiSecret);

        // Send the POST request
        int httpResponseCode = http.GET();

        // Print the response code
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String response = http.getString();
        // If there is a response, print the response payload
        if (httpResponseCode != 200)
        {
            Serial.print("Error on sending GET: ");
            Serial.println(httpResponseCode);
            Serial.println("Server response:");
            Serial.println(response);
        }

        // End the HTTP connection
        http.end();
        return response == "true";
    }
    else
    {
        Serial.println("Error: Not connected to Wi-Fi");
    }
    return false;
}