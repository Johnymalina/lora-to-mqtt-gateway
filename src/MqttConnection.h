#ifndef MQTT_CONNECTION_H
#define MQTT_CONNECTION_H

#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ETH.h>

class MqttConnection
{
public:
    MqttConnection();
    void begin();

private:
    const char *mqtt_broker = "10.10.42.4";
    const char *mqtt_username = "mqtt-user";
    const char *mqtt_password = "HesloMqtt196455";
    const int mqtt_port = 1883;
};

#endif
