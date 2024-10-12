#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <secrets.h>

// Server URL
const char* serverURL = "https://pils.gataersamla.no/api/controller";

const int RELAY_OUTPUT_1 = 26;
const int RELAY_OUTPUT_2 = 27;
bool ledOn = false;

// Pin where the DS18B20 is connected
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire reference to DallasTemperature
DallasTemperature sensors(&oneWire);

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void scanNetworks(){
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
  for (int i = 0; i < numNetworks; i++) {
    Serial.print("SSID: ");
    Serial.print(WiFi.SSID(i)); // Print the SSID
    Serial.print(" | Signal strength: ");
    Serial.print(WiFi.RSSI(i)); // Print the signal strength
    Serial.println(" dBm");
    delay(10); // Small delay for stability
  }
}

void setup() {
  // Start the serial communication
  Serial.begin(115200);
  Serial.println("DS18B20 Temperature Sensor");
  scanNetworks();
  connectToWiFi();
  delay(10);
  pinMode(RELAY_OUTPUT_1, OUTPUT);
  pinMode(RELAY_OUTPUT_2, OUTPUT);
  // Start the DallasTemperature library
  sensors.begin();
  pinMode(BUILTIN_LED, OUTPUT);
}



void postTemperature(float temperature, const int controllerId, const char* apiSecret) {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("POST temperature");
    HTTPClient http;  // Declare an HTTPClient object

    // Specify the server URL
    http.begin(serverURL + String("/") + String(controllerId));

    // Set request header
    http.addHeader("Content-Type", "plain/text");
    http.addHeader("API-X", apiSecret);

    // payload to send
    String text = String(temperature);

    // Send the POST request
    int httpResponseCode = http.POST(text);

    // Print the response code
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    // If there is a response, print the response payload
    if (httpResponseCode != 200) {
      String response = http.getString();
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
      Serial.println("Server response:");
      Serial.println(response);
    }

    // End the HTTP connection
    http.end();
  } else {
    Serial.println("Error: Not connected to Wi-Fi");
  }
}

bool getControllerRelayOn(const int controllerId, const char* apiSecret) {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Get controller relay on request");
    HTTPClient http;  // Declare an HTTPClient object

    // Specify the server URL
    http.begin(serverURL + String("/") + String(controllerId));

    // Set request header
    http.addHeader("API-X", apiSecret);

    // Send the POST request
    int httpResponseCode = http.GET();

    // Print the response code
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    // If there is a response, print the response payload
    if (httpResponseCode != 200) {
      Serial.print("Error on sending GET: ");
      Serial.println(httpResponseCode);
      Serial.println("Server response:");
      Serial.println(response);
    }
    
    // End the HTTP connection
    http.end();
    return response == "true";
  } else {
    Serial.println("Error: Not connected to Wi-Fi");
  }
  return false;
}

void loop() {
  // Request temperature readings from the sensor
  sensors.requestTemperatures();

  // Fetch and print the temperature
  float temperatureC1 = sensors.getTempCByIndex(0); // Get temperature in Celsius
  float temperatureC2 = sensors.getTempCByIndex(1); // Get temperature in Celsius
  Serial.print("Temperature1: ");
  Serial.print(temperatureC1);
  Serial.println(" °C");
  Serial.print("Temperature2: ");
  Serial.print(temperatureC2);
  Serial.println(" °C");
  postTemperature(temperatureC1, CONTROLLER_ID_1, API_KEY_1);
  postTemperature(temperatureC2, CONTROLLER_ID_2, API_KEY_2);

  bool isRelayOn1 = getControllerRelayOn(CONTROLLER_ID_1, API_KEY_1);
  if(isRelayOn1){
    Serial.println("Relay on1");
    digitalWrite(RELAY_OUTPUT_1, HIGH);
  }else{
    Serial.println("Relay off1");
    digitalWrite(RELAY_OUTPUT_1, LOW);
  }
  bool isRelayOn2 = getControllerRelayOn(CONTROLLER_ID_2, API_KEY_2);
  if(isRelayOn2){
    Serial.println("Relay on2");
    digitalWrite(RELAY_OUTPUT_2, HIGH);
  }else{
    Serial.println("Relay off2");
    digitalWrite(RELAY_OUTPUT_2, LOW);
  }

  if(ledOn){
    ledOn = false;
    digitalWrite(BUILTIN_LED, LOW);
  }else{
    ledOn = true;
    digitalWrite(BUILTIN_LED, HIGH);
  }

  // Wait a bit before the next reading
  delay(2000);
}