#include <NetworkConnection.h>
#include <Arduino.h>
#include <DebugMonitor.h>

NetworkConnection::NetworkConnection()
{
}

void NetworkConnection::begin()
{
    ETH.begin();

    ETH.config(IPAddress(10, 10, 42, 5), IPAddress(10, 10, 42, 1), IPAddress(255, 255, 255, 0));
}