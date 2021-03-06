/*
 * c.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef PD_SAP_PROVIDER_H_
#define PD_SAP_PROVIDER_H_

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "lifi-mac-header.h"

namespace ns3 {

class PdSapProvider : public SimpleRefCount<PdSapProvider>
{

public:
	PdSapProvider();
	virtual ~PdSapProvider();

	virtual void DataRequest(uint32_t size, Ptr<Packet> pb, uint8_t band) = 0;
};

template <class C>
class PdSpecificSapProvider : public PdSapProvider
{
public:
	PdSpecificSapProvider (C* phy)
	{
		m_phy = phy;
	}

	virtual ~PdSpecificSapProvider ()
	{
	}

	virtual void DataRequest(uint32_t size, Ptr<Packet> pb, uint8_t band)
	{
		LifiMacHeader header;
		pb->PeekHeader(header);
		std::cout << (uint16_t)header.GetDstAddressMode() << std::endl;
		m_phy->Transmit (size, pb, band);
	}

private:
	PdSpecificSapProvider ()
	{
	}

	C* m_phy;
};

} /* namespace ns3 */

#endif /* C_H_ */
