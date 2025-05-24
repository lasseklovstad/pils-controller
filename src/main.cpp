#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <secrets.h>
#include <logging.h>
#include "httpUtils.h"
#include "wifiUtils.h"
#include "timeUtils.h"
#include "Controller.h"

const int RELAY_OUTPUT_1 = 26;
const int RELAY_OUTPUT_2 = 27;
bool ledOn = false;

// Pin where the DS18B20 is connected
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire reference to DallasTemperature
DallasTemperature sensors(&oneWire);

Controller controller1(CONTROLLER_ID_1, API_KEY_1, RELAY_OUTPUT_1);
Controller controller2(CONTROLLER_ID_2, API_KEY_2, RELAY_OUTPUT_2);

void setup()
{
#if CURRENT_LOG_LEVEL != LOG_LEVEL_OFF
  Serial.begin(115200);
#endif
  LOG_DEBUG("Setup temperature sensors and relay outputs");

  pinMode(RELAY_OUTPUT_1, OUTPUT);
  pinMode(RELAY_OUTPUT_2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  sensors.begin();
  controller1.loadStateFromFile();
  controller2.loadStateFromFile();
}

void loop()
{

  if (WiFi.status() != WL_CONNECTED)
  {
    LOG_DEBUG("Not connected to internet starting configuration...");
    const int numNetworks = sizeof(WIFI_SSIDS) / sizeof(WIFI_SSIDS[0]);
    scanAndConnect(WIFI_SSIDS, WIFI_PASSWORDS, numNetworks);
    if (WiFi.status() != WL_CONNECTED)
    {
      LOG_DEBUG("Could not connect to internet, trying again later...");
    }
    else
    {
      LOG_DEBUG("Connected to internet. Starting time and https config.");
      delay(10);
      configureTime();
      delay(10);
      setupHttps();
      delay(10);
    }
  }
  // Request temperature readings from the sensor
  sensors.requestTemperatures();

  // Save temperatures
  float temperatureC1 = sensors.getTempCByIndex(0); // Get temperature in Celsius
  float temperatureC2 = sensors.getTempCByIndex(1); // Get temperature in Celsius
  controller1.setTemperature(temperatureC1);
  controller2.setTemperature(temperatureC2);
  LOG_DEBUG("Temperature1: " + String(temperatureC1) + " °C");
  LOG_DEBUG("Temperature2: " + String(temperatureC2) + " °C");

  // Post temperatures to server
  postTemperature(controller1);
  postTemperature(controller2);

  // Fetch active batch details
  updateControllerActiveBatch(controller1);
  updateControllerActiveBatch(controller2);

  time_t currentTimestamp = getTimeStamp();
  LOG_DEBUG("Timestamp: " + String(currentTimestamp));
  controller1.updateSource(currentTimestamp);
  controller2.updateSource(currentTimestamp);

  // Save controller state in case of power outage
  controller1.saveStateToFile();
  controller2.saveStateToFile();

  // Wait a bit before the next reading
  delay(10000);
}