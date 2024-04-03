#include <Arduino.h>
#include <DebugMonitor.h>

#include <WiFiClient.h>
#include <ETH.h>
#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER 12

DebugMonitor debug;

void setup()
{
  debug.info("RUNNING");
}

void loop()
{
}
