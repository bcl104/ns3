/*
 * mcps-sap-user.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef MCPS_SAP_USER_H_
#define MCPS_SAP_USER_H_

#include "ns3/core-module.h"
#include "ns3/lifi-mac-general.h"

namespace ns3 {

class McpsSapUser :public SimpleRefCount<McpsSapUser>
{

public:
	McpsSapUser();
	virtual ~McpsSapUser();

	virtual void McpsDataConfirm(uint8_t msduHandle, MacOpStatus status, uint32_t timestamp) = 0;
	virtual void McpsDataIndication(TypeId srcAddrMode, uint16_t srcVPANId, Address srcAddr,
									TypeId dstAddrMode, Address dstAddr, uint32_t msduLength,
									Ptr<Packet> msdu, uint8_t mpduLinkQuality, uint8_t dsn,
									uint32_t timestamp, DataRateId dataRate, bool burstMode) = 0;
	virtual void McpsPurgeConfirm(uint8_t msduHandle, MacOpStatus status) = 0;

};

template <class C>
class McpsSpecificSapUser : public McpsSapUser
{
public:
	McpsSpecificSapUser (C* sscs)
	{
		m_sscs = sscs;
	}

	virtual ~McpsSpecificSapUser();

	virtual void McpsDataConfirm(uint8_t handle, MacOpStatus status, uint32_t timestamp)
	{
		m_sscs->DataConfirm (handle, status, timestamp);
	}

	virtual void McpsDataIndication(TypeId srcAddrMode, uint16_t srcVPANId, Address srcAddr,
									TypeId dstAddrMode, Address dstAddr, uint32_t msduLength,
									Ptr<Packet> msdu, uint8_t mpduLinkQuality, uint8_t dsn,
									uint32_t timestamp, DataRateId dataRate, bool burstMode)
	{
		m_sscs->DataIndication (srcAddrMode, srcVPANId, srcAddr, dstAddrMode, dstAddr, msduLength,
								msdu, mpduLinkQuality, dsn, timestamp, dataRate, burstMode);
	}
	virtual void McpsPurgeConfirm(uint8_t handle, MacOpStatus status)
	{
		m_sscs->PurgeConfirm (handle, status);
	}
private:
	McpsSpecificSapUser ();

	C* m_sscs;
};

} /* namespace ns3 */

#endif /* MCPS_SAP_USER_H_ */
