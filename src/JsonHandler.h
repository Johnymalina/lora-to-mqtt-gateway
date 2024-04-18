#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <ArduinoJson.h>
#include <DebugMonitor.h>

class JsonHandler
{

public:
    JsonHandler();
    void serialize();
    void deserialize();

private:
};

#endif
