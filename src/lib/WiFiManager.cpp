#include <WiFi.h>

#include "WiFiManager.h"

const char *ssid = "ZPP";
const char *password = "18705170885";

void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("Connected");
  Serial.print("Local IP IS: ");
  Serial.println(WiFi.localIP());
}