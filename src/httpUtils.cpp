#include "httpUtils.h"
#include "timeUtils.h"
#include <logging.h>

const char *serverURL = "https://pils.gataersamla.no/api/controller";

#include <mbedtls/md.h>

String generateHMAC(const char *apiSecret, const String &data)
{
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

    // Add the combined "timestamp:nonce:requestBody" data to the HMAC
    mbedtls_md_hmac_update(&ctx, (const unsigned char *)data.c_str(), data.length());

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

    return String(hmacHex); // Return the HMAC hex string
}

void postTemperature(float temperature, const int controllerId, const char *apiSecret)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        LOG_DEBUG("POST temperature");
        HTTPClient http; // Declare an HTTPClient object

        // Specify the server URL
        http.begin(serverURL + String("/") + String(controllerId));
        
        String body = String(temperature);
        String nonce = String(random(1000000, 9999999));
        String timestamp = String(getTimeStamp());
        String data = timestamp + ":" + nonce;
        String hmac = generateHMAC(apiSecret, data);

        http.addHeader("HMAC", hmac);
        http.addHeader("Timestamp", timestamp);
        http.addHeader("Nonce", nonce);
        http.addHeader("Content-Type", "plain/text");

        // Send the POST request
        int httpResponseCode = http.POST(body);

        // Print the response code
        LOG_DEBUG("HTTP Response code: " + String(httpResponseCode));

        // If there is a response, print the response payload
        if (httpResponseCode != 200)
        {
            String response = http.getString();
            LOG_ERROR("Error on sending POST: " + String(httpResponseCode));
            LOG_ERROR("Server response: " + String(response));
        }

        // End the HTTP connection
        http.end();
    }
    else
    {
        LOG_ERROR("Error: Not connected to Wi-Fi");
    }
}

bool getControllerRelayOn(const int controllerId, const char *apiSecret)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        LOG_DEBUG("Get controller relay on request");
        HTTPClient http; // Declare an HTTPClient object

        // Specify the server URL
        http.begin(serverURL + String("/") + String(controllerId));

        String nonce = String(random(1000000, 9999999));
        String timestamp = String(getTimeStamp());
        String data = timestamp + ":" + nonce;
        String hmac = generateHMAC(apiSecret, data);

        http.addHeader("HMAC", hmac);
        http.addHeader("Timestamp", timestamp);
        http.addHeader("Nonce", nonce);

        // Send the POST request
        int httpResponseCode = http.GET();

        // Print the response code
        LOG_DEBUG("HTTP Response code: " + String(httpResponseCode));
        String response = http.getString();
        // If there is a response, print the response payload
        if (httpResponseCode != 200)
        {
            LOG_ERROR("Error on sending GET: " + String(httpResponseCode));
            LOG_ERROR("Server response: " + String(response));
        }

        // End the HTTP connection
        http.end();
        return response == "true";
    }
    else
    {
        LOG_ERROR("Not connected to Wi-Fi");
    }
    return false;
}