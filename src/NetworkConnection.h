#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_POWER_PIN -1
#define ETH_TYPE ETH_PHY_LAN8720
#define ETH_ADDR 0
#define ETH_MDC_PIN 23
#define ETH_MDIO_PIN 18
#define NRST 5

#include <Arduino.h>
#include <WiFiClient.h>
#include <ETH.h>
#include <DebugMonitor.h>

class NetworkConnection
{
public:
    NetworkConnection(eth_clock_mode_t ethClkMode = ETH_CLK_MODE, int ethPowerPin = ETH_POWER_PIN, eth_phy_type_t ethType = ETH_TYPE, int ethAddr = ETH_ADDR, int ethMdcPin = ETH_MDC_PIN, int ethMdioPin = ETH_MDIO_PIN, int nrst = NRST);

private:
    static bool _eth_connected;
};

#endif
