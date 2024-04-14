#include <LoraConnection.h>

String receivedData;
bool msgToSend = false;

void onReceive(int packetSize)
{
    if (packetSize == 0)
        return; // if there's no packet, return

    while (LoRa.available())
    {
        receivedData = LoRa.readString();
    }
    msgToSend = 1;
}

LoraConnection::LoraConnection()
{
}

void LoraConnection::begin()
{

    SPI.begin(SCK, MISO, MOSI, SS);
    LoRa.setPins(SS, RST, DIO0);

#ifdef DEBUG

    debug("INFO: ");
    debug("LoRa Connecting");

#endif

    int counterLora = 0;

    while (!LoRa.begin(868E6))
    {
        debug(".");
        delay(200);
        if (++counterLora > 100)
        {
            debugln("");

#ifdef DEBUG
#if DEBUG_LVL > 1

            debug("ERROR: ");
            debugln("Cant connect to LoRa... Restarting");

#endif
#endif

            delay(100);
            ESP.restart();
        }
    }

#ifdef DEBUG

    debugln("");
    debug("INFO: ");
    debugln("LoRa Connected");

#endif

    LoRa.onReceive(onReceive);
    LoRa.receive();
}
