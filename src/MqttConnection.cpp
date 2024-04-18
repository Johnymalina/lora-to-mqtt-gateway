#include <MqttConnection.h>
#include <Arduino.h>
#include <DebugMonitor.h>

WiFiClient espClient;
PubSubClient client(espClient);

MqttConnection::MqttConnection()
{
}

void MqttConnection::begin()
{
    client.setServer(mqtt_broker, mqtt_port);

#ifdef DEBUG

    debug("INFO: ");
    debugln("MQTT Connecting");

    wdebug("INFO: ");
    wdebugln("MQTT Connecting");

#endif

    int counterMqtt = 0;
    client.connect("lora_to_mqtt_gateway", mqtt_username, mqtt_password);
    while (!client.connected())
    {
        debug(".");
        wdebug(".");
        delay(200);
        if (++counterMqtt > 100)
        {
            debugln("");
            wdebugln("");

#ifdef DEBUG
#if DEBUG_LVL > 1

            debug("ERROR: ");
            debugln("Cant connect to MQTT... Restarting");

            wdebug("ERROR: ");
            wdebugln("Cant connect to MQTT... Restarting");

#endif
#endif

            delay(100);
            ESP.restart();
        }
    }

#ifdef DEBUG

    debug("INFO: ");
    debugln("MQTT Connected");

    wdebug("INFO: ");
    wdebugln("MQTT Connected");

#endif
}

bool MqttConnection::publish(const char *payload, const char *topic)
{
    bool mqttSent;
#ifdef DEBUG

    debug("INFO: ");
    debugln("MQTT Data avalaible to send...");

    wdebug("INFO: ");
    wdebugln("MQTT Data avalaible to send...");

#endif

    while (!client.connect("lora_to_mqtt_gateway", mqtt_username, mqtt_password))
        ;
#ifdef DEBUG

    debug("INFO: ");
    debug("MQTT Client reconnected. Sending data to broker");

    wdebug("INFO: ");
    wdebug("MQTT Client reconnected. Sending data to broker");
    wdebugln(payload);
    wdebugln(topic);

#endif

    int counterPublish = 0;
    while (!client.publish(topic, payload))
    {
        debug(".");
        wdebug(".");
        delay(200);
        if (++counterPublish > 100)
        {
            debugln("");
            wdebugln("");
#ifdef DEBUG
#if DEBUG_LVL > 1

            debug("ERROR: ");
            debugln("Cant publish dato to MQTT broker");

            wdebug("ERROR: ");
            wdebugln("Cant publish dato to MQTT broker");

#endif
#endif

            delay(100);
            mqttSent = 0;
            return false;
        }
    }

    mqttSent = true;
    return mqttSent;
}

bool MqttConnection::getStatus()
{
    bool mqttConnected = client.connect("lora_to_mqtt_gateway", mqtt_username, mqtt_password);

    return mqttConnected;
}