#include <NetworkConnection.h>
#include <Arduino.h>
#include <DebugMonitor.h>

static bool eth_connected = false;

void WiFiEvent(WiFiEvent_t event)
{
    switch (event)
    {
    case ARDUINO_EVENT_ETH_START:
        ETH.setHostname("lora2mqtt-gateway");

#ifdef DEBUG

        debug("INFO: ");
        debugln("Ethernet Network Started");

        wdebug("INFO: ");
        wdebugln("Ethernet Network Started");

#endif
        break;
    case ARDUINO_EVENT_ETH_CONNECTED:

#ifdef DEBUG

        debug("INFO: ");
        debugln("Ethernet Network Connected");

        wdebug("INFO: ");
        wdebugln("Ethernet Network Connected");

#endif

        break;

    case ARDUINO_EVENT_ETH_GOT_IP:

#ifdef DEBUG

        debug("INFO: ");
        debug("Ethernet IPv4: ");
        debug(ETH.localIP());
        debug(", Hostname: ");
        debug(ETH.getHostname());
        debug(", MAC: ");
        debug(ETH.macAddress());
        debug(", Speed: ");
        debug(ETH.linkSpeed());
        debugln("Mbps");

        wdebug("INFO: ");
        wdebug("Ethernet IPv4: ");
        wdebug(ETH.localIP());
        wdebug(", Hostname: ");
        wdebug(ETH.getHostname());
        wdebug(", MAC: ");
        wdebug(ETH.macAddress());
        wdebug(", Speed: ");
        wdebug(ETH.linkSpeed());
        wdebugln("Mbps");

#endif

        eth_connected = true;
        break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:

#ifdef DEBUG

        debugln("ETH Disconnected");

        wdebugln("ETH Disconnected");

#endif

        eth_connected = false;
        break;
    case ARDUINO_EVENT_ETH_STOP:

#ifdef DEBUG

        debugln("ETH Stopped");

        wdebugln("ETH Stopped");

#endif

        eth_connected = false;
        break;

    default:
        break;
    }
}

NetworkConnection::NetworkConnection()
{
}

void NetworkConnection::begin()
{
    ETH.begin();

    ETH.config(IPAddress(10, 10, 42, 5), IPAddress(10, 10, 42, 1), IPAddress(255, 255, 255, 0));

    // TODO Fallback WLAN connection when ethernet is not avalaible
}

void NetworkConnection::setCallback()
{

#ifdef DEBUG

    debug("INFO: ");
    debugln("Network Callback Setup");

    wdebug("INFO: ");
    wdebugln("Network Callback Setup");

#endif

    WiFi.onEvent(WiFiEvent);
}

// TODO Periodicaly check connection status wlan/ethernet. Make sense to add also if connected when its not possible to publish this status wehn network is not connected?
