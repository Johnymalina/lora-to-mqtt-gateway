#include <Arduino.h>
#include <DebugMonitor.h>

DebugMonitor::DebugMonitor(unsigned int baudRate)
{
    Serial.begin(baudRate);
}

void DebugMonitor::begin(unsigned int baudRate)
{
    Serial.begin(baudRate);
}

void DebugMonitor::info(String debugMsg)
{
    Serial.print("INFO: ");
    Serial.println(debugMsg);
}

void DebugMonitor::error(String debugMsg)
{
    Serial.print("ERROR: ");
    Serial.println(debugMsg);
}

void DebugMonitor::end()
{
    Serial.end();
}