#include <JsonHandler.h>

DynamicJsonDocument doc(1024);

JsonHandler::JsonHandler()
{
}

int JsonHandler::sourceAddress(String receivedLoraString)
{
    int sourceAddress;

    deserializeJson(doc, receivedLoraString);
    sourceAddress = doc["addr"];

    return sourceAddress;
}

int JsonHandler::destinationAddress(String receivedLoraString)
{
    int destinationAddress;

    deserializeJson(doc, receivedLoraString);
    doc["dest"] = destinationAddress;

    return destinationAddress;
}

char *JsonHandler::toChar(String inputString)
{
    int strLength = inputString.length() + 1;
    char *outputChar = new char[strLength];
    inputString.toCharArray(outputChar, strLength);

    return outputChar;
}