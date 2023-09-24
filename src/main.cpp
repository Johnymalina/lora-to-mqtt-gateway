#define DEBUG 1

#include <globals.h>

#include <radio.h>

#include <wlan.h>

#include <mqtt.h>

#include <website.h>

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

  ElegantOTA.begin(&server); // Start ElegantOTA
  server.begin();
  debugln("HTTP server started");
  server.on("/", []()
            { server.send(200, "text/html", Website::Update()); });
  Mqtt::Connect();
}

void loop()
{
  server.handleClient();
  ElegantOTA.loop();

  Radio::Receive();

  Mqtt::Publish();
}