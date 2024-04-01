#ifndef LORA_HANDLER_H
#define LORA_HANDLER_H

#include <LoRa.h>

#define SCK 14
#define MISO 32
#define MOSI 33
#define SS 16
#define RST 4
#define DIO0 2

bool msgToSend;
String receivedData;

void loraInitialise();
void loraOnReceive(int packetSize);

#endif
