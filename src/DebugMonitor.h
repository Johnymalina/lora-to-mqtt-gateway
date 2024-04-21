#ifndef DEBUG_MONITOR_H
#define DEBUG_MONITOR_H

#include <WebSerial.h>

#define DEBUG_BAUD_RATE 9600

#define DEBUG
#define DEBUG_LVL 2 // INFO Debug level 1=INFO, 2=ERROR

#ifdef DEBUG
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugBegin(x) Serial.begin(x)
#define wdebug(x) WebSerial.print(x)
#define wdebugln(x) WebSerial.println(x)
#define wdebugBegin(x) WebSerial.begin(x)
#define wdebugCallback(x) WebSerial.msgCallback(x)
#else
#define debug(x)
#define debugln(x)
#define debugBegin(x)
#define wdebug(x)
#define wdebugln(x)
#define wdebugBegin(x)
#endif

#endif
