#ifndef DEBUG_MONITOR_H
#define DEBUG_MONITOR_H

#include <Arduino.h>

class DebugMonitor
{
public:
    DebugMonitor();
    void begin(unsigned int baudRate = 9600);
    void info(String debugMsg);
    void error(String debugMsg);
    void end();

private:
};

#endif
