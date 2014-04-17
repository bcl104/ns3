/*
 * pd-sap-user.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef PD_SAP_USER_H_
#define PD_SAP_USER_H_

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "lifi-phy-general.h"

namespace ns3 {

class PdSapUser : public SimpleRefCount<PdSapUser>
{

public:
	PdSapUser();
	virtual ~PdSapUser();

	virtual void PdDataConfirm(PhyOpStatus status) = 0;
	virtual void PdDataIndication(uint32_t size, Ptr<PacketBurst> pb, uint8_t ppduLinkQuality) = 0;

};

template <class C>
class PdSpecificSapUser : public PdSapUser
{
public:
	PdSpecificSapUser (C* mac)
	{
		m_mac = mac;
	}

	virtual void PdDataConfirm(PhyOpStatus status)
	{
		m_mac->DoDataConfirm (status);
	}

	virtual void PdDataIndication(uint32_t size, Ptr<PacketBurst> pb, uint8_t linkQuality)
	{
		m_mac->DoReceive (size, pb, linkQuality);
	}

	virtual ~PdSpecificSapUser ()
	{
	}

private:
	PdSpecificSapUser ()
	{
	}

	C* m_mac;
};

}
#endif /* PD_SAP_USER_H_ */





