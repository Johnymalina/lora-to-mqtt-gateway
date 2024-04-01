#ifndef CONFIG_H
#define CONFIG_H

#ifdef DEBUG
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
// #define debugBegin(x) Serial.begin(x)
#else
#define debug(x)
#define debugln(x)
// #define debugBegin(x)
#endif

#define DEBUG_BAUD_RATE 9600

bool debugBegin(unsigned int baudRate);

#endif