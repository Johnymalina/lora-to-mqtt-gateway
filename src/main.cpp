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

#include <LoraConnection.h>

#include <WebSerial.h>

NetworkConnection network;

MqttConnection mqtt;

LoraConnection lora;

AsyncWebServer server(80);

void recvMsg(uint8_t *data, size_t len)
{
  WebSerial.println("Received Data...");
  String d = "";
  for (int i = 0; i < len; i++)
  {
    d += char(data[i]);
  }
}

void setup()
{
  debugBegin(DEBUG_BAUD_RATE);

  delay(1000);

#ifdef DEBUG
  debug("INFO: ");
  debugln("Debug Monitor Started");
#endif

  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);

  network.setCallback();

  network.begin();

  mqtt.begin();

  lora.begin();

  SPIFFS.begin(true);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/web.html"); });
  server.on("/lora2mqtt.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/lora2mqtt.png", "image/png"); });

  ElegantOTA.begin(&server);

  server.begin();

  WebSerial.println("Hello!");
}

void loop()
{
  if (msgToSend)
  {

#ifdef DEBUG

    debug("INFO: ");
    debugln("LoRa message received");
    debug("INFO: ");
    debug("Message: ");
    debugln(receivedData);
    WebSerial.println(receivedData);
#endif

    msgToSend = 0;
  }
}
