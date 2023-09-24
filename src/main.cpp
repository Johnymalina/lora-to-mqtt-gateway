#define DEBUG 0

#include <globals.h>

#include <radio.h>

#include <wlan.h>

#include <mqtt.h>

#include <WebServer.h>

#include <ElegantOTA.h>

WebServer server(80);

unsigned long mqttTimer = 0;

void setup()
{
  debugBegin(9600);

  Radio::Initialise();

  Wifi::Connect();

  // OTA
  server.on("/", []()
            { server.send(200, "text/plain", "LORA2MQTT Gateway"); });
  ElegantOTA.begin(&server); // Start ElegantOTA
  server.begin();
  debugln("HTTP server started");

  Mqtt::Connect();
}

void loop()
{
  server.handleClient();
  ElegantOTA.loop();

  Radio::Receive();

  Mqtt::Publish();
}