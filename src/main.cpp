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

#include <Arduino.h>

#include <WiFi.h>
const char *ssid = "TheHorde";
const char *password = "Mylifeforthehorde";

#include <LoRa.h>
#define SS_PIN 5
#define RST_PIN 4
#define DIO0_PIN 2

String loraData;

#include <PubSubClient.h>
const char *mqtt_broker = "10.10.42.4";
const char *topic = "lora/mqtt";
const char *mqtt_username = "mqtt-user";
const char *mqtt_password = "HesloMqtt196455";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

char tempString[256];

void setup()
{
  debugBegin(9600);
  // WIFI
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
  // LORA
  debug("Connecting LoRa");
  LoRa.setPins(SS_PIN, RST_PIN, DIO0_PIN);
  if (!LoRa.begin(868E6))
  {
    debugln("Starting LoRa failed!");
    while (1)
      ;
  }
  else
  {
    debugln("LoRa Runninng");
  }
  // MQTT
  client.setServer(mqtt_broker, mqtt_port);
  debug("MQTT Connecting");
  int counterMqtt = 0;
  while (!client.connect("lora_to_mqtt_gateway", mqtt_username, mqtt_password))
  {
    debug(".");
    delay(200);
    if (++counterMqtt > 100)
    {
      debugln("");
      debugln("Cant connect to MQTT... Restarting");
      delay(100);
      ESP.restart();
    }
  }
  debugln();
  debugln("MQTT Connected");
}

void loop()
{
  if (LoRa.parsePacket())
  {
    while (LoRa.available())
    {
      loraData = LoRa.readString();
      debug("LoRa Received data:");
      debugln(loraData);
    }
    loraData.toCharArray(tempString, 256);
    debugln(tempString);
    client.publish(topic, tempString);
  }
}