#ifndef LORA_CONNECTION_H
#define LORA_CONNECTION_H

#define SCK 14
#define MISO 32
#define MOSI 33
#define SS 16
#define RST 4
#define DIO0 2

#include <Arduino.h>
#include <DebugMonitor.h>
#include <LoRa.h>

class LoraConnection
{
public:
    LoraConnection();
    void begin();

private:
};

#endif
