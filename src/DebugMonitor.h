#ifndef DEBUG_MONITOR_H
#define DEBUG_MONITOR_H

#include <Arduino.h>

class DebugMonitor
{
public:
    DebugMonitor(unsigned int baudRate = 9600);
    void begin(unsigned int baudRate = 9600);
    void info(String debugMsg);
    void error(String debugMsg);
    void end();

private:
    // Data members
};

#endif
