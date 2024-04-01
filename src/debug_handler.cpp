#include <Arduino.h>

#include <debug_handler.h>

bool debugBegin(unsigned int baudRate)
{
    bool connectionStatus;

    Serial.begin(baudRate);

    if (Serial)
    {
        connectionStatus = 1;
    }

    else
    {
        connectionStatus = 0;
    }

    return connectionStatus;
}