#include <Arduino.h>

#include <DebugMonitor.h>

#include <NetworkConnection.h>

NetworkConnection network;

#include <AsyncTCP.h>

#include <ESPAsyncWebServer.h>

#include <ElegantOTA.h>

#include <SPIFFS.h>

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

  SPIFFS.begin(true);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/web.html"); });
  server.on("/lora2mqtt.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/lora2mqtt.png", "image/png"); });

  ElegantOTA.begin(&server); // Start ElegantOTA
  server.begin();
}

void loop()
{
}
