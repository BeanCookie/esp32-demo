#include <WIFI.h>
#include <AsyncMqttClient.hpp>

#include "MQTT.h"

const char *MQTTUsername = "esp32";
const char *MQTTPassword = "esp32";

const char *MQTTHost = "lzzz.work";
const int MQTTPort = 1883;

AsyncMqttClient mqttClient;

void initMQTT()
{
  mqttClient.setServer(MQTTHost, MQTTPort);
  mqttClient.setCredentials(MQTTUsername, MQTTPassword);
  Serial.println("Connecting to MQTT...");
  while (!MQTTConnected)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("Connected");
}

void MQTTReconnect()
{
  mqttClient.connect();
}

uint16_t publishMQTT(const char *topic, uint8_t qos, bool retain, const char *payload)
{
  return mqttClient.publish(topic, qos, retain, payload);
}

bool MQTTConnected()
{
  return mqttClient.connected();
}