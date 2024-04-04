#include <NetworkConnection.h>

NetworkConnection::NetworkConnection(eth_clock_mode_t ethClkMode, int ethPowerPin, eth_phy_type_t ethType, int ethAddr, int ethMdcPin, int ethMdioPin, int nrst)
{
    ETH.begin(ethAddr, ethPowerPin, ethMdcPin, ethMdioPin, ethType, ethClkMode);
    ETH.config(IPAddress(10, 10, 42, 5), IPAddress(10, 10, 42, 1), IPAddress(255, 255, 255, 0));
}