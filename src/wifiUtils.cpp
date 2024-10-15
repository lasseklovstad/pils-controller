
#include "wifiUtils.h"

void connectToWiFi(const char *ssid, const char *password)
{
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void scanNetworks()
{
  // Start the Wi-Fi module
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Disconnect from any existing connection
  delay(100);

  Serial.println("Scanning for Wi-Fi networks...");

  // Start the scan
  int numNetworks = WiFi.scanNetworks();

  Serial.println("Scan complete!");

  // Print the number of networks found
  Serial.print("Number of networks found: ");
  Serial.println(numNetworks);

  // Loop through the found networks and print their SSIDs
  for (int i = 0; i < numNetworks; i++)
  {
    Serial.print("SSID: ");
    Serial.print(WiFi.SSID(i)); // Print the SSID
    Serial.print(" | Signal strength: ");
    Serial.print(WiFi.RSSI(i)); // Print the signal strength
    Serial.println(" dBm");
    delay(10); // Small delay for stability
  }
}