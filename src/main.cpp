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

#include <WiFiClient.h>
#include <WebServer.h>

#include <ElegantOTA.h>

WebServer server(80);

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

unsigned long mqttTimer = 0;
bool msgToSend = 0;

const char *website = R"(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 LoRa2MQTT Gateway</title>
    <style>
        /* Reset default margin and padding */
        body, h1, h2, ul, li {
            margin: 0;
            padding: 0;
        }

        /* Add background color and font styles */
        body {
            font-family: 'Arial', sans-serif;
            background-color: #f2f2f2;
        }

        /* Center the page content */
        .container {
            text-align: center;
            margin: 20px auto;
            padding: 20px;
            background-color: #ffffff;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.2);
            max-width: 800px;
            position: relative;
        }

        /* Style the header */
        h1 {
            color: #333;
            margin-bottom: 20px;
        }

        /* Style the status indicator */
        .status-indicator {
            width: 20px;
            height: 20px;
            border-radius: 50%;
            display: inline-block;
            margin-right: 10px;
        }

        .status-ok {
            background-color: #4CAF50; /* Green */
        }

        .status-warning {
            background-color: #FFC107; /* Yellow */
        }

        .status-error {
            background-color: #F44336; /* Red */
        }

        /* Style the message list */
        .message-list {
            list-style: none;
            padding: 0;
            text-align: left;
            margin-top: 20px;
        }

        .message-item {
            border: 1px solid #ccc;
            margin: 10px 0;
            padding: 10px;
            background-color: #f9f9f9;
            border-radius: 5px;
            box-shadow: 0 2px 2px rgba(0, 0, 0, 0.1);
        }

        /* Add spacing between elements */
        h2 {
            margin-top: 20px;
        }

        /* Style the status text */
        #status-text {
            color: #333;
        }

        /* Style the "Update FW" button */
        .update-fw-button {
            position: absolute;
            top: 20px;
            right: 20px;
            text-decoration: none;
            padding: 10px 20px;
            background-color: #007bff; /* Blue */
            color: #fff;
            border-radius: 5px;
            transition: background-color 0.3s;
        }

        .update-fw-button:hover {
            background-color: #0056b3; /* Darker Blue on hover */
        }

        /* Updated button style */
        .new-button-style {
            background-color: #007bff; /* Blue */
            color: #fff;
            border: none;
            padding: 10px 20px;
            border-radius: 5px;
            transition: background-color 0.3s;
        }

        .new-button-style:hover {
            background-color: #0056b3; /* Darker Blue on hover */
        }
    </style>
</head>
<body>
    <div class="container">
        <a href="/update" class="update-fw-button new-button-style">Update FW</a>
        <h1>ESP32 LoRa2MQTT Gateway</h1>
        
        <h2>Status Indicator</h2>
        <div class="status-indicator status-ok" id="status-indicator"></div>
        <span id="status-text">Status: OK</span>

        <!-- Display the current date and time -->
        <p id="current-time" style="font-size: 18px; margin-top: 20px;"></p>

        <h2>Last Received Messages</h2>
        <ul class="message-list" id="message-list">
            <!-- Message items will be added here dynamically -->
            <li class="message-item">
                <strong>Timestamp:</strong> 2023-09-24 10:00:00<br>
                <strong>Content:</strong> Message 1: Hello, World!
            </li>
            <li class="message-item">
                <strong>Timestamp:</strong> 2023-09-24 10:15:00<br>
                <strong>Content:</strong> Message 2: Temperature: 25°C
            </li>
            <li class="message-item">
                <strong>Timestamp:</strong> 2023-09-24 10:30:00<br>
                <strong>Content:</strong> Message 3: Sensor Data: 123
            </li>
        </ul>
    </div>

    <script>
        // Function to display the current date and time
        function displayCurrentTime() {
            const currentTimeElement = document.getElementById('current-time');
            const now = new Date();
            const formattedDate = now.toLocaleDateString();
            const formattedTime = now.toLocaleTimeString();
            currentTimeElement.textContent = `Current Date and Time: ${formattedDate} ${formattedTime}`;
        }

        // Simulated status data (replace with actual status)
        const status = "ok";

        // Simulated message data (replace with actual data retrieval)
        const messages = [
            { timestamp: "2023-09-24 10:00:00", content: "Message 1: Hello, World!" },
            { timestamp: "2023-09-24 10:15:00", content: "Message 2: Temperature: 25°C" },
            { timestamp: "2023-09-24 10:30:00", content: "Message 3: Sensor Data: 123" }
        ];

        // Function to display messages in the list
        function displayMessages() {
            const messageList = document.getElementById('message-list');
            messageList.innerHTML = ''; // Clear existing messages

            messages.forEach((message, index) => {
                const messageItem = document.createElement('li');
                messageItem.classList.add('message-item');
                messageItem.innerHTML = `
                    <strong>Timestamp:</strong> ${message.timestamp}<br>
                    <strong>Content:</strong> ${message.content}
                `;
                messageList.appendChild(messageItem);
            });
        }

        // Function to update the status indicator
        function updateStatusIndicator() {
            const statusIndicator = document.getElementById('status-indicator');
            const statusText = document.getElementById('status-text');

            if (status === "ok") {
                statusIndicator.className = 'status-indicator status-ok';
                statusText.textContent = 'Status: OK';
            } else if (status === "warning") {
                statusIndicator.className = 'status-indicator status-warning';
                statusText.textContent = 'Status: Warning';
            } else if (status === "error") {
                statusIndicator.className = 'status-indicator status-error';
                statusText.textContent = 'Status: Error';
            }
        }

        // Call the displayCurrentTime, displayMessages, and updateStatusIndicator functions to initialize
        displayCurrentTime();
        displayMessages();
        updateStatusIndicator();

        // Update the current time every second
        setInterval(displayCurrentTime, 1000);
    </script>
</body>
</html>
)";

void setup()
{
  debugBegin(9600);
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
            { server.send(200, "text/html", website); });
  ElegantOTA.begin(&server); // Start ElegantOTA
  server.begin();
  debugln("HTTP server started");

  // LORA
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

  if (LoRa.parsePacket())
  {
    while (LoRa.available())
    {
      loraData = LoRa.readString();
      debug("LoRa Received data:");
      debugln(loraData);
      msgToSend = 1;
    }
  }
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