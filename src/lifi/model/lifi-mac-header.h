/*
 * lifi-mac-header.h
 *
 *  Created on: 2014年4月11日
 *      Author: will
 */

#ifndef LIFI_MAC_HEADER_H_
#define LIFI_MAC_HEADER_H_


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "lifi-mac-general.h"

namespace ns3 {

class LifiMacHeader:public Header
{
public:
	LifiMacHeader();
	~LifiMacHeader();

	void SetFrameType(FrameType frameType);
	FrameType GetFrameType();

	void SetSequenceNumber(uint8_t sequenceNumber);
	uint8_t GetSequenceNumber();

	void SetFramePending(bool pending);
	bool GetFramePending();

	void SetAckRequest(bool request);
	bool GetAckRequest();

	void SetDstVPANId(uint16_t dstId);
	uint16_t GetDstVPANId();

	void SetSrcVPANId(uint16_t srcId);
	uint16_t GetSrcVPANId();

	void SetSrcAddress(Address address);
    Address GetSrcAddress();

    void SetDstAddress(Address address);
    Address GetDstAddress();

    AddrMode GetSrcAddressMode () const;

    AddrMode GetDstAddressMode () const;

	static TypeId GetTypeId (void);

	virtual TypeId GetInstanceTypeId (void) const;

	virtual void Print (std::ostream &os) const;

	virtual uint32_t GetSerializedSize (void) const;

	virtual void Serialize (Buffer::Iterator start) const;

	virtual uint32_t Deserialize (Buffer::Iterator start);


private:
	uint8_t m_version;
	uint8_t m_sequenceNumber;
	FrameType m_type;
	bool m_pending;
	bool m_ackRequest;
	uint16_t m_dstVPANId;
	uint16_t m_srcVPANId;
	Address m_dstAddress;
	Address m_srcAddress;
};
} /* namespace ns3 */

#endif /* LIFI_MAC_HEADER_H_ */
