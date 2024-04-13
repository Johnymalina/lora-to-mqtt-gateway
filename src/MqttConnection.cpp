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

#endif

    int counterMqtt = 0;
    client.connect("lora_to_mqtt_gateway", mqtt_username, mqtt_password);
    while (!client.connected())
    {
        debug(".");
        delay(200);
        if (++counterMqtt > 100)
        {
            debugln("");

#ifdef DEBUG
#if DEBUG_LVL > 1

            debug("ERROR: ");
            debugln("Cant connect to MQTT... Restarting");
            
#endif
#endif

            delay(100);
            ESP.restart();
        }
    }
    debugln("MQTT Connected");
}