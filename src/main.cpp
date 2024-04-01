#define DEBUG 1 // INFO Set if the Serial output will be avalaible (1) or not (0)

#include <Arduino.h>
#include <Config.h>

bool msgToSend = 0;

#include <LoRa.h>

#define SCK 14
#define MISO 32
#define MOSI 33
#define SS 16
#define RST 4
#define DIO0 2

String receivedData;

void onReceive(int packetSize)
{
  if (packetSize == 0)
    return; // if there's no packet, return

  while (LoRa.available())
  {
    receivedData = LoRa.readString();
  }
  msgToSend = 1;
}

void loraInitialise()
{
  debugln("Connecting LoRa");
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);
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
  LoRa.onReceive(onReceive);
  LoRa.receive();
}

#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ETH.h>

#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_POWER_PIN -1
#define ETH_TYPE ETH_PHY_LAN8720
#define ETH_ADDR 0
#define ETH_MDC_PIN 23
#define ETH_MDIO_PIN 18
#define NRST 5

const char *mqtt_broker = "10.10.42.4";
const char *mqtt_username = "mqtt-user";
const char *mqtt_password = "HesloMqtt196455";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

static bool eth_connected = false;

void WiFiEvent(WiFiEvent_t event)
{
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

void mqttInitialize()
{
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

bool mqttPublish(String loraData, const char *topic)
{
  bool mqttSent;
  debugln("MQTT data avalaible to send");
  int strLenght = loraData.length() + 1;
  char mqttData[strLenght];
  loraData.toCharArray(mqttData, strLenght);
  while (!client.connect("lora_to_mqtt_gateway", mqtt_username, mqtt_password))
    ;
  debugln("MQTT reconnected");
  if (client.publish(topic, mqttData))
  {
    mqttSent = 1;
  }

  return mqttSent;
}

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

bool mqttConnected;
bool loraConnected;

void websitePublish()
{
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html>\n";
  ptr += "<head>\n";
  ptr += "    <title>ESP32 LoRa2MQTT Gateway</title>\n";
  ptr += "    <meta http-equiv=\"refresh\" content=\"30\" />\n";
  ptr += "    <style>\n";
  ptr += "        /* Reset default margin and padding */\n";
  ptr += "        body, h1, h2, ul, li {\n";
  ptr += "            margin: 0;\n";
  ptr += "            padding: 0;\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        /* Add background color and font styles */\n";
  ptr += "        body {\n";
  ptr += "            font-family: 'Arial', sans-serif;\n";
  ptr += "            background-color: #f2f2f2;\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        /* Center the page content */\n";
  ptr += "        .container {\n";
  ptr += "            text-align: center;\n";
  ptr += "            margin: 20px auto;\n";
  ptr += "            padding: 20px;\n";
  ptr += "            background-color: #ffffff;\n";
  ptr += "            border-radius: 10px;\n";
  ptr += "            box-shadow: 0 0 10px rgba(0, 0, 0, 0.2);\n";
  ptr += "            max-width: 800px;\n";
  ptr += "            position: relative;\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        /* Style the header */\n";
  ptr += "        h1 {\n";
  ptr += "            color: #333;\n";
  ptr += "            margin-bottom: 20px;\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        /* Style the status indicators */\n";
  ptr += "        .status-indicators {\n";
  ptr += "            display: flex; /* Display indicators horizontally */\n";
  ptr += "            align-items: center; /* Vertically center indicators */\n";
  ptr += "            justify-content: center; /* Horizontally center indicators */\n";
  ptr += "            margin-bottom: 10px; /* Add 10px spacing between indicators */\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        .status-indicator {\n";
  ptr += "            width: 20px;\n";
  ptr += "            height: 20px;\n";
  ptr += "            border-radius: 50%;\n";
  ptr += "            margin: 0 10px; /* Add 10px spacing between indicators */\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        .mqtt-indicator {\n";
  ptr += "            background-color: var(--mqtt-color); /* Variable for MQTT status color */\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        .lora-indicator {\n";
  ptr += "            background-color: var(--lora-color); /* Variable for LoRa status color */\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        /* Style the message list */\n";
  ptr += "        .message-list {\n";
  ptr += "            list-style: none;\n";
  ptr += "            padding: 0;\n";
  ptr += "            text-align: left;\n";
  ptr += "            margin-top: 20px;\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        .message-item {\n";
  ptr += "            border: 1px solid #ccc;\n";
  ptr += "            margin: 10px 0;\n";
  ptr += "            padding: 10px;\n";
  ptr += "            background-color: #f9f9f9;\n";
  ptr += "            border-radius: 5px;\n";
  ptr += "            box-shadow: 0 2px 2px rgba(0, 0, 0, 0.1);\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        /* Add spacing between elements */\n";
  ptr += "        h2 {\n";
  ptr += "            margin-top: 20px;\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        /* Style the status text */\n";
  ptr += "        #status-text {\n";
  ptr += "            color: #333;\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        /* Style the \"Update FW\" button */\n";
  ptr += "        .update-fw-button {\n";
  ptr += "            position: absolute;\n";
  ptr += "            top: 20px;\n";
  ptr += "            right: 20px;\n";
  ptr += "            text-decoration: none;\n";
  ptr += "            padding: 10px 20px;\n";
  ptr += "            background-color: #007bff; /* Blue for \"Update Firmware\" button */\n";
  ptr += "            color: #fff;\n";
  ptr += "            border-radius: 5px;\n";
  ptr += "            transition: background-color 0.3s;\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        .update-fw-button:hover {\n";
  ptr += "            background-color: #0056b3; /* Darker Blue on hover */\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        /* Updated button style */\n";
  ptr += "        .new-button-style {\n";
  ptr += "            background-color: #007bff; /* Blue */\n";
  ptr += "            color: #fff;\n";
  ptr += "            border: none;\n";
  ptr += "            padding: 10px 20px;\n";
  ptr += "            border-radius: 5px;\n";
  ptr += "            transition: background-color 0.3s;\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        .new-button-style:hover {\n";
  ptr += "            background-color: #0056b3; /* Darker Blue on hover */\n";
  ptr += "        }\n";
  ptr += "    </style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "    <div class=\"container\">\n";
  ptr += "        <a href=\"/update\" class=\"update-fw-button new-button-style\">Update FW</a>\n";
  ptr += "        <h1>ESP32 LoRa2MQTT Gateway</h1>\n";
  ptr += "        \n";
  ptr += "        <h2>Status Indicators</h2>\n";
  ptr += "        <div class=\"status-indicators\">\n";
  ptr += "            <div class=\"status-indicator mqtt-indicator\" id=\"mqtt-indicator\"></div>\n";
  ptr += "            <span>MQTT Status</span>\n";
  ptr += "            <div class=\"status-indicator lora-indicator\" id=\"lora-indicator\"></div>\n";
  ptr += "            <span>LoRa Status</span>\n";
  ptr += "        </div>\n";
  ptr += "\n";
  ptr += "        <!-- Display the current date and time -->\n";
  ptr += "        <p id=\"current-time\" style=\"font-size: 18px; margin-top: 20px;\"></p>\n";
  ptr += "\n";
  ptr += "        <h2>Last Received Messages</h2>\n";
  ptr += "        <ul class=\"message-list\" id=\"message-list\">\n";
  ptr += "            <li class=\"message-item\">\n";
  ptr += "                <strong>Timestamp:</strong> 2023-09-24 10:00:00<br>\n";
  ptr += "                <strong>Content:</strong> Message 1: Hello, World!\n";
  ptr += "            </li>\n";
  ptr += "            <li class=\"message-item\">\n";
  ptr += "                <strong>Timestamp:</strong> 2023-09-24 10:15:00<br>\n";
  ptr += "                <strong>Content:</strong> Message 2: Temperature: 25°C\n";
  ptr += "            </li>\n";
  ptr += "            <li class=\"message-item\">\n";
  ptr += "                <strong>Timestamp:</strong> 2023-09-24 10:30:00<br>\n";
  ptr += "                <strong>Content:</strong> Message 3: Sensor Data: 123\n";
  ptr += "            </li>\n";
  ptr += "        </ul>\n";
  ptr += "    </div>\n";
  ptr += "\n";
  ptr += "    <script>\n";
  ptr += "        // Function to display the current date and time\n";
  ptr += "        function displayCurrentTime() {\n";
  ptr += "            const currentTimeElement = document.getElementById('current-time');\n";
  ptr += "            const now = new Date();\n";
  ptr += "            const formattedDate = now.toLocaleDateString();\n";
  ptr += "            const formattedTime = now.toLocaleTimeString();\n";
  ptr += "            currentTimeElement.textContent = `Current Date and Time: ${formattedDate} ${formattedTime}`;\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        // Set status colors directly with variables\n";

  if (mqttConnected)
  {
    ptr += "        document.documentElement.style.setProperty('--mqtt-color', 'green');\n";
  }
  else
  {
    ptr += "        document.documentElement.style.setProperty('--mqtt-color', 'red');\n";
  }
  if (loraConnected)
  {
    ptr += "        document.documentElement.style.setProperty('--lora-color', 'green');\n";
  }
  else
  {
    ptr += "        document.documentElement.style.setProperty('--lora-color', 'red');\n";
  }

  ptr += "\n";
  ptr += "        // Update the current time every second\n";
  ptr += "        setInterval(displayCurrentTime, 1000);\n";
  ptr += "    </script>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  server.send(200, "text/html", ptr);
}

#include <ArduinoJson.h>

StaticJsonDocument<192> doc;

String topics[] = {"lora2mqtt/gateway", "lora2mqtt/meteostation"};

unsigned long statusPublishTimer = 0;
#define STATUS_PUBLISH_INTERVAL 60000

void gatewayPublishStatus()
{
  StaticJsonDocument<32> doc;
  String serverStatus;

  mqttConnected = client.connect("lora_to_mqtt_gateway", mqtt_username, mqtt_password);
  loraConnected = LoRa.begin(868E6);
  LoRa.receive();

  doc["lora"] = loraConnected;
  doc["mqtt"] = mqttConnected;
  serverStatus.remove(0);
  serializeJson(doc, serverStatus);
  debug("Server Status Json: ");
  debugln(serverStatus);

  int strLenght = serverStatus.length() + 1;
  char serverStatusChar[strLenght];
  serverStatus.toCharArray(serverStatusChar, strLenght);
  const char *topic = "lora2mqtt/gateway";
  client.publish(topic, serverStatusChar);
}

void setup()
{
  debugBegin(9600);

  loraInitialise();

  // wifiConnect();
  WiFi.onEvent(WiFiEvent);
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
  ETH.config(IPAddress(10, 10, 42, 5), IPAddress(10, 10, 42, 1), IPAddress(255, 255, 255, 0));
  mqttInitialize();

  ElegantOTA.begin(&server);

  server.begin();
  debugln("HTTP server started");
  server.on("/", websitePublish);
}

void loop()
{
  if (millis() > statusPublishTimer + STATUS_PUBLISH_INTERVAL)
  {
    gatewayPublishStatus();
    statusPublishTimer = millis();
  }

  if (msgToSend)
  {
    debug("LoRa Received data: ");
    debugln(receivedData);
    deserializeJson(doc, receivedData);
    if (!doc["dest"])
    {
      int addr = doc["addr"];
      debugln("Data for Gateway");
      debug("Source: ");
      debugln(topics[addr]);
      String address = topics[addr];
      const char *topic = address.c_str();
      if (mqttPublish(receivedData, topic))
      {
        debugln("Published to MQTT!");
      }
      else
      {
        debugln("MQTT Publish Failed!");
      }
    }
    else
    {
      debugln("Data for diferent device");
    }
    msgToSend = 0;
  }

  server.handleClient();

  ElegantOTA.loop();
}