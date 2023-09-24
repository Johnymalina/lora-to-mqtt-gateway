#pragma once

#include <globals.h>

#include <WiFi.h>
const char *ssid = "TheHorde";
const char *password = "Mylifeforthehorde";

namespace Wifi
{
    void Connect()
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        int counterWifi = 0;
        debug("Connecting WiFi");
        while (WiFi.status() != WL_CONNECTED)
        {
            debug(".");
            delay(200);
            if (++counterWifi > 100)
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
        debug("IP address: ");
        debugln(WiFi.localIP());
    }
} // namespace Wifi
