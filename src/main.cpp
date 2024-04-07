// #include <Arduino.h>
#include <DebugMonitor.h>

DebugMonitor debug;

#include <NetworkConnection.h>

NetworkConnection network;

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <SPIFFS.h>

static bool eth_connected = false;

void WiFiEvent(WiFiEvent_t event)
{
  Serial.println(event);
  switch (event)
  {
  case SYSTEM_EVENT_ETH_START:
    Serial.println("ETH Started");
    // set eth hostname here
    ETH.setHostname("esp32-ethernet");
    break;
  case SYSTEM_EVENT_ETH_CONNECTED:
    Serial.println("ETH Connected");
    break;
  case SYSTEM_EVENT_ETH_GOT_IP:
    Serial.print("ETH MAC: ");
    Serial.print(ETH.macAddress());
    Serial.print(", IPv4: ");
    Serial.print(ETH.localIP());
    if (ETH.fullDuplex())
    {
      Serial.print(", FULL_DUPLEX");
    }
    Serial.print(", ");
    Serial.print(ETH.linkSpeed());
    Serial.println("Mbps");
    eth_connected = true;
    break;
  case SYSTEM_EVENT_ETH_DISCONNECTED:
    Serial.println("ETH Disconnected");
    eth_connected = false;
    break;
  case SYSTEM_EVENT_ETH_STOP:
    Serial.println("ETH Stopped");
    eth_connected = false;
    break;
  default:
    break;
  }
}

AsyncWebServer server(80);

void setup()
{
  debug.begin();
  WiFi.onEvent(WiFiEvent);
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
