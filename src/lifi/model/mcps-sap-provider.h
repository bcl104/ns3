/*
 * mcps-sap-provider.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef MCPS_SAP_PROVIDER_H_
#define MCPS_SAP_PROVIDER_H_

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/lifi-mac-general.h"

namespace ns3 {

class McpsSapProvider : public SimpleRefCount<McpsSapProvider>
{

public:
	McpsSapProvider();
	virtual ~McpsSapProvider();

	virtual void McpsDataRequest(AddrMode srcAddrMode, AddrMode dstAddrMode, uint16_t dstVPANId,
								Address dstAddr, uint32_t msduLength, Ptr<Packet> msdu, uint8_t msduHandle,
								const TxOption& txOption, DataRateId rate, bool burstMode) = 0;
	virtual void McpsPurgeRequest(uint8_t msduHandle) = 0;

};

template<class C>
class McpsSpecificSapProvider : public McpsSapProvider
{
public:
	McpsSpecificSapProvider(C* mac)
	{
		m_mac = mac;
	}

	virtual ~McpsSpecificSapProvider()
	{

	}

	virtual void McpsDataRequest(AddrMode srcAddrMode, AddrMode dstAddrMode, uint16_t dstVPANId,
									Address dstAddr, uint32_t msduLength, Ptr<Packet> msdu, uint8_t msduHandle,
									const TxOption& txOption, DataRateId rate, bool burstMode)
	{
		m_mac->Send (srcAddrMode, dstAddrMode, dstVPANId, dstAddr, msduLength,
						msdu, msduHandle, txOption, rate, burstMode);
	}

	virtual void McpsPurgeRequest(uint8_t handle)
	{
		m_mac->Purge (handle);
	}

private:
	McpsSpecificSapProvider ()
	{

	}

	C* m_mac;
};

} /* namespace ns3 */

#endif /* MCPS_SAP_PROVIDER_H_ */
