#include <JsonHandler.h>

JsonHandler::JsonHandler()
{
}

void JsonHandler::serialize()
{
}

char *JsonHandler::toChar(String inputString)
{
    int strLength = inputString.length() + 1;
    char *outputChar = new char[strLength];
    inputString.toCharArray(outputChar, strLength);

    return outputChar;
}