#include <Arduino.h>

#include <WiFi.h>

const char *ssid = "TheHorde";
const char *password = "Mylifeforthehorde";

void setup()
{

  WiFi.begin(ssid, password);
  int counter = 0;
  debug("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    debug(".");
    delay(200);
    if (++counter > 100)
    {
      debugln("");
      debugln("Cant connect to WiFi... Restarting");
      delay(100);
      ESP.restart();
    }
  }
  debugln("");
  debug("WiFi Connected to network: ");
  debugln(WiFi.SSID());
}

void loop()
{
}