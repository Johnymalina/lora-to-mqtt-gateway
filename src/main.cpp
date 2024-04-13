/*

Name: Lora2Mqtt Gateway

Author: Jakub Čihánek

First created: 08.04.2024 14:54

Version: 1

Last Updated: 08.04.2024

Description: Main station for converting LoRa messages to MQTT for use in HomeAssistant

*/

#include <Arduino.h>

#include <DebugMonitor.h>

#include <NetworkConnection.h>

#include <AsyncTCP.h>

#include <ESPAsyncWebServer.h>

#include <ElegantOTA.h>

#include <SPIFFS.h>

#include <MqttConnection.h>

NetworkConnection network;

MqttConnection mqtt;

AsyncWebServer server(80);

void setup()
{
  debugBegin(DEBUG_BAUD_RATE);

  delay(1000);

#ifdef DEBUG
  debug("INFO: ");
  debugln("Debug Monitor Started");
#endif

  network.setCallback();

  network.begin();

  mqtt.begin();

  SPIFFS.begin(true);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/web.html"); });
  server.on("/lora2mqtt.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/lora2mqtt.png", "image/png"); });

  ElegantOTA.begin(&server);

  server.begin();
}

void loop()
{
}
