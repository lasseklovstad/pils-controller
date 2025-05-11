#include "httpUtils.h"
#include "timeUtils.h"
#include <logging.h>

const char *serverURL = "https://pils.gataersamla.no/api/controller";

WiFiClientSecure client;

void setupHttps()
{
    if (!LittleFS.begin(true))
    {
        LOG_ERROR("An Error has occurred while mounting SPIFFS");
        return;
    }

    // Open the certificate file in binary mode
    File certFile = LittleFS.open("/isrgrootx1.pem", "r");
    if (!certFile)
    {
        LOG_ERROR("Failed to open certificate file");
        return;
    }

    // Read the certificate into a buffer
    size_t certSize = certFile.size();

    if (certSize == 0)
    {
        LOG_ERROR("Certificate file is empty");
        certFile.close();
        return;
    }

    // Load the certificate into the WiFiClientSecure
    if (!client.loadCACert(certFile, certSize))
    {
        LOG_ERROR("Failed to load certificate");
    }
    LOG_DEBUG("Successfully loaded certificate");
    certFile.close();
}

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

void addAuthHeaders(HTTPClient &http, Controller &controller)
{
    String nonce = String(random(1000000, 9999999));
    String timestamp = String(getTimeStamp());
    String data = timestamp + ":" + nonce;
    String hmac = generateHMAC(controller.getApiKey(), data);

    http.addHeader("HMAC", hmac);
    http.addHeader("Timestamp", timestamp);
    http.addHeader("Nonce", nonce);
}

void postTemperature(Controller &controller)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http; // Declare an HTTPClient object

        // Specify the server URL
        http.begin(client, serverURL + String("/") + String(controller.getControllerId()));

        String body = String(controller.getTemperature());
        addAuthHeaders(http, controller);
        http.addHeader("Content-Type", "plain/text");

        // Send the POST request
        int httpResponseCode = http.POST(body);

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

void updateControllerActiveBatch(Controller &controller)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        LOG_DEBUG("Get controller batch temperature on request");
        HTTPClient http; // Declare an HTTPClient object
        // Specify the server URL
        http.begin(client, serverURL + String("/") + String(controller.getControllerId()));

        addAuthHeaders(http, controller);

        const char *headerKeys[] = {"x-batch-mode", "x-batch-status", "x-controller-min-delay", "x-controller-avg-buffer-size", "x-controller-hysteresis"};
        const size_t headerKeysCount = sizeof(headerKeys) / sizeof(headerKeys[0]);
        http.collectHeaders(headerKeys, headerKeysCount);

        // Send the POST request
        int httpResponseCode = http.GET();

        String response = http.getString();
        // If there is a response, print the response payload
        if (httpResponseCode == 200)
        {
            String mode = http.header("x-batch-mode");
            String status = http.header("x-batch-status");
            String minDelay = http.header("x-controller-min-delay");
            String avgBufferSize = http.header("x-controller-avg-buffer-size");
            String hysteresis = http.header("x-controller-hysteresis");
            controller.update(mode, status, response, minDelay, avgBufferSize, hysteresis);
            LOG_DEBUG("Server response: " + String(response));
            LOG_DEBUG("Mode: " + mode);
            LOG_DEBUG("Status: " + status);
            LOG_DEBUG("Min delay: " + minDelay);
            LOG_DEBUG("Average buffer size: " + avgBufferSize);
            LOG_DEBUG("Hysteresis: " + hysteresis);
        }
        else if (httpResponseCode == 404)
        {
            LOG_DEBUG("404 No batch active for controller");
            controller.reset();
        }
        else
        {
            LOG_ERROR("Error on sending GET: " + String(httpResponseCode));
            LOG_ERROR("Server response: " + String(response));
        }
        http.end();
    }
    else
    {
        LOG_ERROR("Not connected to Wi-Fi");
    }
}