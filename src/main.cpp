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

#include <DebugMonitor.h>

#include <JsonHandler.h>

NetworkConnection network;

MqttConnection mqtt;

LoraConnection lora;

AsyncWebServer server(80);

unsigned long gatewayStatusTimer = 0;
#define STATUS_PUBLISH_INTERVAL 60000

void gatewayPublishStatus()
{
  StaticJsonDocument<32> doc;
  String gatewayStatus;

  doc["lora"] = mqtt.getStatus();
  doc["mqtt"] = lora.getStatus();

  gatewayStatus.remove(0);

  serializeJson(doc, gatewayStatus);

#ifdef DEBUG

  debug("INFO: ");
  debug("Gateway Status: ");
  debugln(gatewayStatus);

  wdebug("INFO: ");
  wdebug("Gateway Status: ");
  wdebugln(gatewayStatus);

#endif

  int strLenght = gatewayStatus.length() + 1;
  char gatewayStatusChar[strLenght];
  gatewayStatus.toCharArray(gatewayStatusChar, strLenght);
  const char *topic = "lora2mqtt/gateway";
  mqtt.publish(gatewayStatusChar, topic);
}

void setup()
{
  debugBegin(DEBUG_BAUD_RATE);

  delay(200);

#ifdef DEBUG
  debug("INFO: ");
  debugln("Debug Monitor Started");
#endif

  wdebugBegin(&server);
  wdebugCallback(recvMsg);

  network.setCallback();

  network.begin();

  mqtt.begin();

  lora.begin();

  SPIFFS.begin(true);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/web.html"); });

  server.on("/lora2mqtt.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/lora2mqtt.png", "image/png"); });

  server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    request->redirect("/");

    delay(100);
    
    ESP.restart(); });

  ElegantOTA.begin(&server);

  server.begin();
}

void loop()
{

  if (millis() > gatewayStatusTimer + STATUS_PUBLISH_INTERVAL)
  {
    gatewayPublishStatus();
    gatewayStatusTimer = millis();
  }

  if (msgToSend)
  {

#ifdef DEBUG

    debug("INFO: ");
    debugln("LoRa message received");
    debug("INFO: ");
    debug("Message: ");
    debugln(receivedData);

    wdebug("INFO: ");
    wdebugln("LoRa message received");
    wdebug("INFO: ");
    wdebug("Message: ");
    wdebugln(receivedData);

#endif

    msgToSend = 0;
  }
}
