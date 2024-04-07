#include <Arduino.h>
#include <DebugMonitor.h>

DebugMonitor debug;

#include <NetworkConnection.h>

NetworkConnection network;



#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

AsyncWebServer server(80);

void setup()
{
  debug.begin();

  network.begin();

  

  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  //           { request->send(200, "text/plain", "Hi! I am ESP32."); });

  // ElegantOTA.begin(&server); // Start ElegantOTA
  // server.begin();
}

void loop()
{
}
