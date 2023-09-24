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

#include <LoRa.h>

#define SS_PIN 5
#define RST_PIN 4
#define DIO0_PIN 2

void loraInitialise()
{
  debugln("Connecting LoRa");
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
}

String loraReceive()
{
  if (LoRa.parsePacket())
  {
    while (LoRa.available())
    {
      String receivedData = LoRa.readString();
      debug("LoRa Received data:");
      debugln(loraData);
      msgToSend = 1;
    }
  }
  return loraData;
}

#include <WiFiClient.h>
#include <PubSubClient.h>
const char *mqtt_broker = "10.10.42.4";
const char *topic = "lora/mqtt";
const char *mqtt_username = "mqtt-user";
const char *mqtt_password = "HesloMqtt196455";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

void mqttConnect()
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
  client.setKeepAlive(1000);
}

void mqttPublish()
{
  mqttConnected = client.connected();
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

String websiteGenerate()
{
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html>\n";
  ptr += "<head>\n";
  ptr += "    <title>ESP32 LoRa2MQTT Gateway</title>\n";
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
  ptr += "        /* Style the status indicator */\n";
  ptr += "        .status-indicator {\n";
  ptr += "            width: 20px;\n";
  ptr += "            height: 20px;\n";
  ptr += "            border-radius: 50%;\n";
  ptr += "            display: inline-block;\n";
  ptr += "            margin-right: 10px;\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        .status-ok {\n";
  ptr += "            background-color: #4CAF50; /* Green */\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "\n";
  ptr += "        .status-error {\n";
  ptr += "            background-color: #F44336; /* Red */\n";
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
  ptr += "            background-color: #007bff; /* Blue */\n";
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
  ptr += "        <h2>Status Indicator</h2>\n";
  ptr += "        <div class=\"status-indicator status-ok\" id=\"status-indicator\"></div>\n";
  ptr += "        <span id=\"status-text\">Status: OK</span>\n";
  ptr += "\n";
  ptr += "        <!-- Display the current date and time -->\n";
  ptr += "        <p id=\"current-time\" style=\"font-size: 18px; margin-top: 20px;\"></p>\n";
  ptr += "\n";
  ptr += "        <h2>Last Received Messages</h2>\n";
  ptr += "        <ul class=\"message-list\" id=\"message-list\">\n";
  ptr += "            <!-- Message items will be added here dynamically -->\n";
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
  ptr += "        // Simulated status data (replace with actual status)\n";
  if (mqttConnected)
  {
    ptr += "        const status = \"ok\";\n";
  }
  else
  {
    ptr += "        const status = \"error\";\n";
  }
  ptr += "\n";
  ptr += "        // Simulated message data (replace with actual data retrieval)\n";
  ptr += "        const messages = [\n";
  // ptr += msg1;
  ptr += "            { timestamp: \"2023-09-24 10:15:00\", content: \"Message 2: Temperature: 25°C\" },\n";
  ptr += "            { timestamp: \"2023-09-24 10:30:00\", content: \"Message 3: Sensor Data: 123\" }\n";
  ptr += "        ];\n";
  ptr += "\n";
  ptr += "        // Function to display messages in the list\n";
  ptr += "        function displayMessages() {\n";
  ptr += "            const messageList = document.getElementById('message-list');\n";
  ptr += "            messageList.innerHTML = ''; // Clear existing messages\n";
  ptr += "\n";
  ptr += "            messages.forEach((message, index) => {\n";
  ptr += "                const messageItem = document.createElement('li');\n";
  ptr += "                messageItem.classList.add('message-item');\n";
  ptr += "                messageItem.innerHTML = `\n";
  ptr += "                    <strong>Timestamp:</strong> ${message.temp}<br>\n";
  ptr += "                    <strong>Content:</strong> ${message.lux}\n";
  ptr += "                `;\n";
  ptr += "                messageList.appendChild(messageItem);\n";
  ptr += "            });\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        // Function to update the status indicator\n";
  ptr += "        function updateStatusIndicator() {\n";
  ptr += "            const statusIndicator = document.getElementById('status-indicator');\n";
  ptr += "            const statusText = document.getElementById('status-text');\n";
  ptr += "\n";
  ptr += "            if (status === \"ok\") {\n";
  ptr += "                statusIndicator.className = 'status-indicator status-ok';\n";
  ptr += "                statusText.textContent = 'Status: OK';\n";
  ptr += "            } else if (status === \"warning\") {\n";
  ptr += "                statusIndicator.className = 'status-indicator status-warning';\n";
  ptr += "                statusText.textContent = 'Status: Warning';\n";
  ptr += "            } else if (status === \"error\") {\n";
  ptr += "                statusIndicator.className = 'status-indicator status-error';\n";
  ptr += "                statusText.textContent = 'Status: Error';\n";
  ptr += "            }\n";
  ptr += "        }\n";
  ptr += "\n";
  ptr += "        // Call the displayCurrentTime, displayMessages, and updateStatusIndicator functions to initialize\n";
  ptr += "        displayCurrentTime();\n";
  ptr += "        displayMessages();\n";
  ptr += "        updateStatusIndicator();\n";
  ptr += "\n";
  ptr += "        // Update the current time every second\n";
  ptr += "        setInterval(displayCurrentTime, 1000);\n";
  ptr += "    </script>\n";
  ptr += "</body>\n";
  ptr += "</html>";
  return ptr;
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

void setup()
{
  debugBegin(9600);

  loraInitialise();

  wifiConnect();

  // OTA

  ElegantOTA.begin(&server); // Start ElegantOTA
  server.begin();
  debugln("HTTP server started");
  server.on("/", []()
            { server.send(200, "text/html", websiteGenerate()); });
  mqttConnect();
}

void loop()
{
  server.handleClient();
  ElegantOTA.loop();

  loraReceive();

  mqttPublish();
}