#include <Arduino.h>
#include <DebugMonitor.h>

DebugMonitor debug;

#include <NetworkConnection.h>

NetworkConnection network;

// static bool eth_connected = false;

// void WiFiEvent(WiFiEvent_t event)
// {
//   switch (event)
//   {
//   case SYSTEM_EVENT_ETH_START:
//     Serial.println("ETH Started");
//     // set eth hostname here
//     ETH.setHostname("lora2mqtt-gateway");
//     break;
//   case SYSTEM_EVENT_ETH_CONNECTED:
//     Serial.println("ETH Connected");
//     break;
//   case SYSTEM_EVENT_ETH_GOT_IP:
//     Serial.print("ETH MAC: ");
//     Serial.print(ETH.macAddress());
//     Serial.print(", IPv4: ");
//     Serial.print(ETH.localIP());
//     if (ETH.fullDuplex())
//     {
//       Serial.print(", FULL_DUPLEX");
//     }
//     Serial.print(", ");
//     Serial.print(ETH.linkSpeed());
//     Serial.println("Mbps");
//     eth_connected = true;
//     break;
//   case SYSTEM_EVENT_ETH_DISCONNECTED:
//     Serial.println("ETH Disconnected");
//     eth_connected = false;
//     break;
//   case SYSTEM_EVENT_ETH_STOP:
//     Serial.println("ETH Stopped");
//     eth_connected = false;
//     break;
//   default:
//     break;
//   }
// }

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

AsyncWebServer server(80);

void setup()
{
  debug.begin();
  debug.info("RUNNING");

  // wifiConnect();
  // WiFi.onEvent(WiFiEvent);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "Hi! I am ESP32."); });

  ElegantOTA.begin(&server); // Start ElegantOTA
  server.begin();
}

void loop()
{
}
