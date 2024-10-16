
#include "wifiUtils.h"
#include <logging.h>



void connectToWiFi(const char *ssid, const char *password)
{
  LOG_DEBUG("Connecting to " + String(ssid));

  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10)
  {
    delay(1000);
    LOG_DEBUG(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    LOG_DEBUG("Wi-Fi connected.");
    LOG_DEBUG("IP address: " + String(WiFi.localIP()));
  } else {
    LOG_ERROR("Failed to connect.");
  }
}

void scanAndConnect(const char* ssidList[], const char* passwordList[], int numSavedNetworks)
{
  // Start the Wi-Fi module in station mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  LOG_DEBUG("Scanning for Wi-Fi networks...");

  // Scan for available networks
  int foundNetworks = WiFi.scanNetworks();

  LOG_DEBUG("Scan complete!");
  LOG_DEBUG("Number of networks found: ");
  LOG_DEBUG(foundNetworks);

  // Try to find and connect to one of your saved networks
  for (int i = 0; i < foundNetworks; i++) {
    String ssid = WiFi.SSID(i);
    LOG_DEBUG("Found network: " + String(ssid));

    for (int j = 0; j < numSavedNetworks; j++) {
      if (ssid == ssidList[j]) {
        LOG_DEBUG("Matching network found: " + String(ssidList[j]));
        connectToWiFi(ssidList[j], passwordList[j]);
        if (WiFi.status() == WL_CONNECTED) {
          return;  // Exit if connected
        }
      }
    }
  }

  LOG_ERROR("No matching networks found or failed to connect.");
}