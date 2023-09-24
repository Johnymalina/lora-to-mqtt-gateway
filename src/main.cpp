#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugBegin(x) Serial.begin(x)
#else
#define debug(x)
#define debugln(x)
#define debugBegin(x)
#endif

bool msgToSend = 0;
bool mqttConnected;

String loraData;

#include <Arduino.h>

#include <radio.h>

#include <mqtt.h>

#include <website.h>

#include <WebServer.h>

#include <ElegantOTA.h>

#include <WiFi.h>
const char *ssid = "TheHorde";
const char *password = "Mylifeforthehorde";

void wifiConnect()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int counterWifi = 0;
  debug("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    debug(".");
    delay(200);
    if (++counterWifi > 100)
    {
      debugln("");
      debugln("Cant connect to WiFi... Restarting");
      delay(100);
      ESP.restart();
    }
  }
  debugln("");
  debug("WiFi Connected to network: ");
  debugln(WiFi.SSID());
  debug("IP address: ");
  debugln(WiFi.localIP());
}

WebServer server(80);

unsigned long mqttTimer = 0;

void setup()
{
  debugBegin(9600);

  Radio::Initialise();

  wifiConnect();

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