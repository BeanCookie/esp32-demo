#include <Arduino.h>

#include "Thing.CoAP.h"

#include "lib/WiFiManager.h"
#include "lib/MQTT.h"
#include "YACL.h"

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

#define LED 2
#define BUTTON 0

Thing::CoAP::Client coapClient;
Thing::CoAP::ESP::UDPPacketProvider udpProvider;

CBORPair data = CBORPair();
CBORArray array = CBORArray();

void initCoAPClient()
{
  IPAddress ip(192, 168, 31, 27);
  coapClient.SetPacketProvider(udpProvider);
  Serial.println("Connecting to CoAP...");
  coapClient.Start(ip, 5683);
  Serial.println("Connected");
}

void setup()
{
  Serial.begin(115200);
  initWiFi();
  initMQTT();
  initCoAPClient();

  data.append("lat", 48.12010);

  data.append("lon", -1.62863);
  int dots[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 1, 1, 1, 1, 1, 1};
  array.append(dots, sizeof(dots) / sizeof(dots[0]));
  data.append("dots", array);
}

void sendCoAPMessage()
{
  coapClient.Get("hello", std::string((char *)data.to_CBOR()), [](Thing::CoAP::Response response)
                 {
    std::vector<uint8_t> payload = response.GetPayload();
    std::string received(payload.begin(), payload.end());
    Serial.println("Server sent the following message:");
    if (received == "On")
    { // If the message is "On" we will turn the LED on.
      Serial.println(received.c_str());
    }
    else if (received == "Off")
    { // If it is "Off" we will turn the LED off.
      Serial.println(received.c_str());
    } });
}

void sendMQTTMessage()
{

  unsigned long now = millis();
  if (now - lastMsg > 2000)
  {
    lastMsg = now;
    ++value;
    snprintf(msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    publishMQTT("device/3gasd", 1, true, msg);
  }
}

void loop()
{
  if (!MQTTConnected())
  {
    MQTTReconnect();
  }

  sendCoAPMessage();
  sendMQTTMessage();

  coapClient.Process();

  delay(2000);
}