#define DEBUG // INFO Comment to disable Serial Debug

#include <Arduino.h>
#include "debug_handler.h"

void setup()
{
  while (!debugBegin(DEBUG_BAUD_RATE))
  {
    int counterSerial;
    delay(100);
    if (counterSerial++ > 20)
    {
      ESP.restart();
    }
  }
  delay(1000);
  debugln("");
  debugln("Serial Start");
}

void loop()
{
}
