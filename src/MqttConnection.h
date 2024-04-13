#ifndef MQTT_CONNECTION_H
#define MQTT_CONNECTION_H

#define MQTT_BROKER_IP "10.10.42.4"
#define MQTT_USERNAME "mqtt-user"
#define MQTT_PASSWORD "HesloMqtt196455"

#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ETH.h>

class MqttConnection
{
public:
    MqttConnection();
    void begin();
    bool publish(const char *payload, const char *topic);

private:
    const char *mqtt_broker = MQTT_BROKER_IP;
    const char *mqtt_username = MQTT_USERNAME;
    const char *mqtt_password = MQTT_PASSWORD;
    const int mqtt_port = 1883;
};

#endif
