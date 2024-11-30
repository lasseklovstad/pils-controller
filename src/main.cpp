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

Controller controller1(CONTROLLER_ID_1, API_KEY_1);
Controller controller2(CONTROLLER_ID_2, API_KEY_2);

void setup()
{
#if CURRENT_LOG_LEVEL != LOG_LEVEL_OFF
  Serial.begin(115200);
#endif
  LOG_DEBUG("DS18B20 Temperature Sensor");
  const int numNetworks = sizeof(WIFI_SSIDS) / sizeof(WIFI_SSIDS[0]);
  scanAndConnect(WIFI_SSIDS, WIFI_PASSWORDS, numNetworks);
  delay(10);
  LOG_DEBUG("Configure time");
  configureTime();
  delay(10);
  setupHttps();
  delay(10);
  pinMode(RELAY_OUTPUT_1, OUTPUT);
  pinMode(RELAY_OUTPUT_2, OUTPUT);
  // Start the DallasTemperature library
  sensors.begin();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
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
  
  if (controller1.shouldTurnOnSource(currentTimestamp))
  {
    LOG_DEBUG("Relay on1");
    digitalWrite(RELAY_OUTPUT_1, HIGH);
  }
  else
  {
    LOG_DEBUG("Relay off1");
    digitalWrite(RELAY_OUTPUT_1, LOW);
  }
  if (controller2.shouldTurnOnSource(currentTimestamp))
  {
    LOG_DEBUG("Relay on2");
    digitalWrite(RELAY_OUTPUT_2, HIGH);
  }
  else
  {
    LOG_DEBUG("Relay off2");
    digitalWrite(RELAY_OUTPUT_2, LOW);
  }

  if (ledOn)
  {
    ledOn = false;
    digitalWrite(LED_BUILTIN, LOW);
  }
  else
  {
    ledOn = true;
    digitalWrite(LED_BUILTIN, HIGH);
  }

  // Wait a bit before the next reading
  delay(10000);
}