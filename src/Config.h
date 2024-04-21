#ifndef CONFIG_H
#define CONFIG_H

// INFO Debug

#define DEBUG_BAUD_RATE 9600

#define DEBUG
#define DEBUG_LVL 2 // INFO Debug level 1=INFO, 2=ERROR

// INFO Gateway

#define STATUS_PUBLISH_INTERVAL 60000

// INFO Lora Connection

enum LoraAdress
{
    GATEWAY_LORA_ADDRESS,
    METEOSTATION_LORA_ADDRESS
};

#endif
