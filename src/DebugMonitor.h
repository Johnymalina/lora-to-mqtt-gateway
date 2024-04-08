#ifndef DEBUG_MONITOR_H
#define DEBUG_MONITOR_H

#define DEBUG_BAUD_RATE 9600

#define DEBUG
#define DEBUG_LVL 1 // INFO Debug level 1=INFO, 2=ERROR

#ifdef DEBUG
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugBegin(x) Serial.begin(x)
#else
#define debug(x)
#define debugln(x)
#define debugBegin(x)
#endif

#endif
