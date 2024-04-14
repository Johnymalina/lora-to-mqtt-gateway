#ifndef LORA_CONNECTION_H
#define LORA_CONNECTION_H

#include <Arduino.h>
#include <DebugMonitor.h>
#include <LoRa.h>

#define SCK 14
#define MISO 32
#define MOSI 33
#define SS 16
#define RST 4
#define DIO0 2

class LoraConnection
{
public:
    LoraConnection();
    void begin();

private:
};

extern String receivedData;
extern bool msgToSend;

#endif
