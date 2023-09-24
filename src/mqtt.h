#pragma once

#include <globals.h>
#include <wlan.h>
#include <WiFiClient.h>

#include <PubSubClient.h>
const char *mqtt_broker = "10.10.42.4";
const char *topic = "lora/mqtt";
const char *mqtt_username = "mqtt-user";
const char *mqtt_password = "HesloMqtt196455";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

namespace Mqtt
{
    void Connect()
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
    void Publish()
    {
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
} // namespace Mqtt
