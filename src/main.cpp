#define DEBUG 1

#include <globals.h>

#include <radio.h>

#include <WiFi.h>
const char *ssid = "TheHorde";
const char *password = "Mylifeforthehorde";

#include <WiFiClient.h>
#include <WebServer.h>

#include <ElegantOTA.h>

WebServer server(80);

#include <PubSubClient.h>
const char *mqtt_broker = "10.10.42.4";
const char *topic = "lora/mqtt";
const char *mqtt_username = "mqtt-user";
const char *mqtt_password = "HesloMqtt196455";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long mqttTimer = 0;

void setup()
{
  debugBegin(9600);

  Radio::Initialise();
  // WIFI
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

  // OTA
  server.on("/", []()
            { server.send(200, "text/plain", "LORA2MQTT Gateway"); });
  ElegantOTA.begin(&server); // Start ElegantOTA
  server.begin();
  debugln("HTTP server started");

  // LORA

  // MQTT
  client.setServer(mqtt_broker, mqtt_port);
  debugln("MQTT Connecting");
  int counterMqtt = 0;
  client.connect("lora_to_mqtt_gateway", mqtt_username, mqtt_password);
  while (!client.connected())
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
  debugln("MQTT Connected");
}

void loop()
{
  server.handleClient();
  ElegantOTA.loop();

  Radio::Receive();

  if (msgToSend)
  {
    debugln("MQTT data avalaible to send");
    int strLenght = loraData.length() + 1;
    char mqttData[strLenght];
    loraData.toCharArray(mqttData, strLenght);
    while (!client.connect("lora_to_mqtt_gateway", mqtt_username, mqtt_password))
      ;
    debugln("MQTT reconnected");
    while (!client.publish(topic, mqttData))
    {
      debugln("Sending MQTT data");
    }
    msgToSend = 0;
  }
}