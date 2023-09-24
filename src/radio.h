#pragma once

#include <globals.h>

#include <LoRa.h>

#define SS_PIN 5
#define RST_PIN 4
#define DIO0_PIN 2

namespace Radio
{

    void Initialise()
    {
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
    }

    void Receive()
    {
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
    }

} // namespace Lora
