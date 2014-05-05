/*
 * lifi-mac-header.cc
 *
 *  Created on: 2014年4月11日
 *      Author: will
 */

#include "lifi-mac-header.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("LifiMacHeader");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiMacHeader);


LifiMacHeader::LifiMacHeader() : m_version (0x00)
{
	m_sequenceNumber = 0x00;
	m_type = LIFI_RESERVED;
	m_pending = 0x00;
	m_ackRequest = 0x00;
	m_dstVPANId = 0x0000;
	m_srcVPANId = 0x0000;
}

LifiMacHeader::~LifiMacHeader()
{
}

void LifiMacHeader::SetSequenceNumber(uint8_t sequenceNumber)
{
	m_sequenceNumber=sequenceNumber;
}

uint8_t LifiMacHeader::GetSequenceNumber()
{
	return m_sequenceNumber;
}

void LifiMacHeader::SetFrameType(FrameType type)
{
	m_type = type;
}
enum FrameType LifiMacHeader::GetFrameType()
{
  return m_type;
}

void LifiMacHeader::SetFramePending(bool pending)
{
	m_pending = pending;
}

bool LifiMacHeader::GetFramePending()
{
	return m_pending;
}

void LifiMacHeader::SetAckRequest(bool request)
{
	m_ackRequest = request;
}

bool LifiMacHeader::GetAckRequest()
{
	return m_ackRequest;
}

void LifiMacHeader::SetDstVPANId(uint16_t dstId)
{
	m_dstVPANId = dstId;
}

uint16_t LifiMacHeader::GetDstVPANId()
{
	return m_dstVPANId;
}

void LifiMacHeader::SetSrcVPANId(uint16_t srcId)
{
	m_srcVPANId=srcId;
}

uint16_t LifiMacHeader::GetSrcVPANId()
{
	return m_srcVPANId;
}

void LifiMacHeader::SetSrcAddress(Address address) {
	m_srcAddress = address;
}

Address LifiMacHeader::GetSrcAddress() {
	return m_srcAddress;
}

void LifiMacHeader::SetDstAddress(Address address) {
	m_dstAddress = address;
}

Address LifiMacHeader::GetDstAddress() {
	return m_dstAddress;
}

TypeId LifiMacHeader::GetTypeId (void)
{
	static TypeId tid=TypeId("ns3::LifiMacHeader")
			.SetParent<Header>()
			.AddConstructor<LifiMacHeader>()
			;
	return tid;
}
TypeId LifiMacHeader::GetInstanceTypeId (void) const
{
	return GetTypeId();
}
void LifiMacHeader::Print (std::ostream &os) const
{

}
uint32_t LifiMacHeader::GetSerializedSize (void) const
{
	uint32_t frame_control_size = 2;
	uint32_t sqn_size = 1;
	uint32_t dst_vpanid_size = 2;
	uint32_t src_vpanid_size = 2;
	uint32_t dst_addr_size = m_dstAddress.GetLength();
	uint32_t src_addr_size = m_srcAddress.GetLength();

	uint32_t header_size = frame_control_size + sqn_size + dst_vpanid_size + src_vpanid_size
			 	 	 	 	 + dst_addr_size + src_addr_size;
    return header_size;
}
void LifiMacHeader::Serialize (Buffer::Iterator start) const
{
	Buffer::Iterator it = start;
	uint16_t frameControlField = (m_version & 0x03)
									| ((m_type & 0x07) << 6)
									| ((m_pending & 0x01) << 10)
									| (((uint8_t)m_ackRequest & 0x01) << 11)
									| (((uint8_t)GetDstAddressMode() & 0x03) << 12)
									| (((uint8_t)GetSrcAddressMode() & 0x03) << 14);
	it.WriteU16(frameControlField);
	it.WriteU8(m_sequenceNumber);
	it.WriteU16(m_dstVPANId);
	WriteTo(it, m_dstAddress);
	it.WriteU16(m_srcVPANId);
	WriteTo(it, m_srcAddress);
}

AddrMode LifiMacHeader::GetSrcAddressMode() const {
	if (Mac16Address::IsMatchingType(m_srcAddress))
	{
		return SHORT;
	}else if (Mac64Address::IsMatchingType(m_srcAddress))
	{
		return EXTENDED;
	}else
	{
		NS_FATAL_ERROR("Invalid address mode.");
	}
	return ERROR;
}

AddrMode LifiMacHeader::GetDstAddressMode() const {
	if (Mac16Address::IsMatchingType(m_dstAddress))
	{
		return SHORT;
	}else if (Mac64Address::IsMatchingType(m_dstAddress))
	{
		return EXTENDED;
	}else
	{
		NS_FATAL_ERROR("Invalid address mode.");
	}
	return ERROR;
}

uint32_t LifiMacHeader::Deserialize (Buffer::Iterator start)
{
	Buffer::Iterator it = start;
	uint16_t frameControlField = it.ReadU16();
	m_version = (uint8_t) (frameControlField & 0x0003);
	m_type = (FrameType) ((frameControlField >> 6) & 0x0007);
	m_pending = (bool) ((frameControlField >> 10) & 0x0001);
	m_ackRequest = (bool) ((frameControlField >> 11) & 0x0001);

	AddrMode dstAddrMode = (AddrMode) ((frameControlField >> 12) & 0x0003);
	AddrMode srcAddrMode = (AddrMode) ((frameControlField >> 14) & 0x0003);

	m_sequenceNumber = it.ReadU8();
	m_dstVPANId = it.ReadU16();

	if (dstAddrMode == SHORT)
	{
		Mac16Address dstAddress;
		ReadFrom(it, dstAddress);
		m_dstAddress = Address (dstAddress);
	}
	else if (dstAddrMode == EXTENDED)
	{
		Mac64Address dstAddress;
		ReadFrom(it, dstAddress);
		m_dstAddress = Address (dstAddress);
	}
	else
		NS_FATAL_ERROR("Invalid address mode.");

	m_srcVPANId = it.ReadU16();

	if (srcAddrMode == SHORT)
	{
		Mac16Address srcAddress;
		ReadFrom(it, srcAddress);
		m_srcAddress = Address (srcAddress);
	}
	else if (srcAddrMode == EXTENDED)
	{
		Mac64Address srcAddress;
		ReadFrom(it, srcAddress);
		m_srcAddress = Address (srcAddress);
	}
	else
		NS_FATAL_ERROR("Invalid address mode.");

	return it.GetDistanceFrom(start);
}

} /* namespace ns3 */
