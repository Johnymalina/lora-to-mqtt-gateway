#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <ArduinoJson.h>
#include <DebugMonitor.h>

class JsonHandler
{

public:
    JsonHandler();
    int sourceAddress(String receivedLoraString);
    int destinationAddress(String receivedLoraString);
    char *toChar(String inputString);

private:
};

#endif
