/*
 *
 *lifi-mac-beacon-frame.cc
 *    Author: wanghongqiao
 *     date  2014.4.11
 */
#include "lifi-mac-beacon.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE("LifiMacBeacon");

namespace ns3{


LifiMacBeacon::LifiMacBeacon ()
{
	NS_LOG_FUNCTION (this);
	m_finalCapSlot = 0;
	m_cellSearchEn = false;
	m_gtsDescriptorCount = 0;
	m_gtsDirMask = 0;
	m_bcnPayloadSize = 0;
	m_cellEnLenth = 0;
	m_extendedAddrsCount = 0;
	m_gtsPermit = false;
	m_shortAddrsCount = 0;
	m_supframeOrder = 0;
	m_vpanCoord = false;
	m_assocPermit = false;
	m_bcnOrder  = 0;
}

LifiMacBeacon::LifiMacBeacon (Ptr<Packet> p) {
	NS_LOG_FUNCTION (this << p);
	uint32_t size = p->GetSize();
	uint8_t *data = (uint8_t*) malloc (size);
	p->CopyData(data, size);
	Deserialize(data, size);
	free (data);
}

LifiMacBeacon::~LifiMacBeacon () {
	NS_LOG_FUNCTION (this);
}

LifiMacBeacon& LifiMacBeacon::Construct(Ptr<Packet> p) {
	static LifiMacBeacon beacon;
	memset(&beacon, 0, sizeof (LifiMacBeacon));
	beacon = LifiMacBeacon (p);
	return beacon;
}

Ptr<Packet> LifiMacBeacon::GetPacket(){
	Buffer b = Serialize();
	Ptr<Packet> p = Create<Packet> (b.PeekData(), b.GetSize());
	return p;
}

uint8_t LifiMacBeacon::GetBcnOrder() const {
	return m_bcnOrder;
}

void LifiMacBeacon::SetBcnOrder(uint8_t order) {
	NS_ASSERT((order >= 0) && (order <= 15));
	m_bcnOrder = order;
}

uint8_t LifiMacBeacon::GetSupframeOrder() const {
	return m_supframeOrder;
}

void LifiMacBeacon::SetSupframeOrder(uint8_t order) {
	NS_ASSERT((order >= 0) && (order <= 15));
	m_supframeOrder = order;
}

uint8_t LifiMacBeacon::GetFinalCapSlot() const {
	return m_finalCapSlot;
}

void LifiMacBeacon::SetFinalCapSlot(uint8_t slot) {
	NS_ASSERT((slot >= 0) && (slot <= 15));
	m_finalCapSlot = slot;
}

bool LifiMacBeacon::GetVpanCoord() const {
	return m_vpanCoord;
}

void LifiMacBeacon::SetVpanCoord(bool coord) {
	m_vpanCoord = coord;
}

bool LifiMacBeacon::GetAssocPermit() const {
	return m_assocPermit;
}

void LifiMacBeacon::SetAssocPermit(bool permit) {
	m_assocPermit = permit;
}

bool LifiMacBeacon::GetCellSearchEn() const {
	return m_cellSearchEn;
}

void LifiMacBeacon::SetCellSearchEn(bool enable) {
	m_cellSearchEn = enable;
}

uint8_t LifiMacBeacon::GetGtsDescripCount() const {
	return m_gtsDescriptorCount;
}

void LifiMacBeacon::SetGtsDescripCount(uint8_t count) {
	NS_ASSERT((count >= 0) && (count <= 7));
	m_gtsDescriptorCount = count;
}

bool LifiMacBeacon::GetGtsPermit() const {
	return m_gtsPermit;
}

void LifiMacBeacon::SetGtsPermit(bool permit) {
	m_gtsPermit = permit;
}

uint8_t LifiMacBeacon::GetGtsDirMask() const {
	return m_gtsDirMask;
}

void LifiMacBeacon::SetGtsDirMask(uint8_t mask) {
//	NS_ASSERT((mask >= 0) && (mask <= 127));
	m_gtsDirMask = mask;
}

bool LifiMacBeacon::AddGts(GtsDescriptor gts) {
	NS_ASSERT ((gts.deviceShortAddr != 0xff) && (gts.deviceShortAddr != 0xfe));
	GtsList::iterator it = std::find_if(m_gts.begin(), m_gts.end(), gts);
	NS_ASSERT (it == m_gts.end());
	m_gts.push_back(gts);
	NS_ASSERT(m_gts.size() <= MAX_GTS_DESCRIPTOR);
	return true;
}

bool LifiMacBeacon::RemoveGts(GtsDescriptor gts) {
	GtsList::iterator it = std::find_if(m_gts.begin(), m_gts.end(), gts);
	NS_ASSERT (it != m_gts.end());
	m_gts.erase(it);
	NS_ASSERT(m_gts.size() >= 0);
	return true;
}

bool LifiMacBeacon::AddPendingAddress(Address address) {
	AddrList::iterator it = std::find_if(m_pendingAddrs.begin(), m_pendingAddrs.end(), address);
	NS_ASSERT (it == m_pendingAddrs.end());
	m_pendingAddrs.push_back(address);
	if (Mac16Address::IsMatchingType(address))
	{
		m_shortAddrsCount ++;
	}else if (Mac64Address::IsMatchingType(address))
	{
		m_extendedAddrsCount ++;
	}else
	{
		NS_FATAL_ERROR("Unsupported address type.");
	}
	NS_ASSERT (m_pendingAddrs.size() <= MAX_PENDING_ADDRESS);
	return true;
}

bool LifiMacBeacon::RemovePendingAddress(Address address) {
	AddrList::iterator it = std::find_if(m_pendingAddrs.begin(), m_pendingAddrs.end(), address);
	NS_ASSERT (it != m_pendingAddrs.end());
	if (Mac16Address::IsMatchingType(address))
	{
		m_shortAddrsCount --;
	}else if (Mac64Address::IsMatchingType(address))
	{
		m_extendedAddrsCount --;
	}else
	{
		NS_FATAL_ERROR("Unsupported address type.");
	}
	NS_ASSERT (m_pendingAddrs.size() >= 0);
	m_pendingAddrs.erase(it);
	return true;
}

uint8_t* LifiMacBeacon::GetPayload(uint32_t* size) {
	*size = m_bcnPayloadSize;
	return m_bcnPayload;
}

void LifiMacBeacon::SetPayload(uint8_t* data, uint32_t size) {
	NS_ASSERT (size <= LifiMac::aMaxBeaconPayloadLength);
	memset(m_bcnPayload, 0, LifiMac::aMaxBeaconPayloadLength);
	memcpy(m_bcnPayload, data, size);
	m_bcnPayloadSize = size;
}

uint8_t LifiMacBeacon::GetShortAddressCount() const {
	return m_shortAddrsCount;
}

uint8_t LifiMacBeacon::GetExtendedAddressCount() const {
	return m_extendedAddrsCount;
}

AddrList& LifiMacBeacon::GetAddrs() {
	return m_pendingAddrs;
}

uint32_t LifiMacBeacon::WriteTo(Buffer::Iterator &it, AddrList address) {
	AddrList::iterator i = address.begin();
	uint32_t written = 0;
	AddrList temp64;
	while (i != address.end())
	{
		if (Mac64Address::IsMatchingType(*i))
		{
			temp64.push_back(*i);
			i++;
			continue;
		}
		ns3::WriteTo(it, *i);
		i ++;
		written ++;
	}
	AddrList::iterator i64 = temp64.begin();
	while (i64 != temp64.end())
	{
		ns3::WriteTo(it, *i64);
		i64 ++;
		written ++;
	}
	return written;
}

uint32_t LifiMacBeacon::WriteTo(Buffer::Iterator &it, GtsList descrip) {
	GtsList::iterator i = descrip.begin();
	uint32_t written = 0;
	while (i != descrip.end())
	{
		it.WriteU16((*i).deviceShortAddr);
		it.WriteU8((((*i).gtsStartSlot) & 0x0f) | ((((*i).gtsLenth) & 0x0f) << 4));
		i ++;
		written ++;
	}
	return written;
}

uint32_t LifiMacBeacon::WriteTo(Buffer::Iterator &it, uint8_t *data, uint32_t size) {
	it.Write(data, size);
	return size;
}

uint8_t* LifiMacBeacon::ReadFrom(const uint8_t *data, AddrList& address, uint32_t shorts, uint32_t extendeds) {
	uint32_t read = 0;
	while (read < (shorts + extendeds))
	{
		if (read < shorts)
		{
			Mac16Address addr16;
			addr16.CopyFrom(data);
			address.push_back(Address (addr16));
			data +=2;
			read ++;
			continue;
		}
		Mac64Address addr64;
		addr64.CopyFrom(data);
		address.push_back(Address (addr64));
		data += 8;
		read ++;
	}
	return const_cast<uint8_t*> (data);
}

uint8_t* LifiMacBeacon::ReadFrom(const uint8_t* data, GtsList& descrip, uint32_t size) {
	NS_ASSERT (size <= MAX_GTS_DESCRIPTOR);
	for (uint32_t read = 0; read < size; read ++)
	{
		GtsDescriptor d = GtsDescriptor ((*(uint16_t*)data), *(data+2)&0x0f, (*(data+2) >> 4)&0x0f);
		descrip.push_back(d);
		data += 3;
	}
	return const_cast<uint8_t*> (data);
}

uint32_t LifiMacBeacon::ReadFrom(uint8_t* to, const uint8_t* start, const uint8_t* end) {
	uint32_t read = 0;
	uint8_t* cur = const_cast<uint8_t*> (start);
	while (!(cur > end))
	{
		(*to) = (*cur);
		to ++;
		cur ++;
		read ++;
	}
	return read;
}

uint8_t LifiMacBeacon::GetCellSearchLenth() const {
	return m_cellEnLenth;
}

void LifiMacBeacon::SetCellSearchLenth(uint8_t lenth) {
	m_cellEnLenth = lenth;
}

AddrList::iterator LifiMacBeacon::CheckPendingAddress(Address address) {
	AddrList::iterator it = std::find_if(m_pendingAddrs.begin(), m_pendingAddrs.end(), address);
	return it;
}

GtsList::iterator LifiMacBeacon::FindAssignedGts(Mac16Address address) {
	GtsList::iterator it = m_gts.begin();
	while (it != m_gts.end())
	{
		if (address == (*it).deviceShortAddr)
			return it;
		it ++;
	}
	return m_gts.end();
}

Buffer LifiMacBeacon::Serialize() {
	NS_LOG_FUNCTION (this);
	uint32_t supfSpecSize = 2;
//	uint32_t cellSrchEnSize = 1;
	uint32_t gtsSpecSize = 1;
	uint32_t gtsDirSize = 1;
	uint32_t pendAddrSpecSize = 1;
	uint32_t gtsDescripSize = m_gts.size()*3;
	uint32_t pendAddrSize = m_shortAddrsCount*2 + m_extendedAddrsCount*8;
	uint32_t cellSrchSize = 1;
	uint32_t bcnPayloadSize = m_bcnPayloadSize;

	uint32_t size = supfSpecSize + /*cellSrchEnSize +*/ gtsSpecSize + gtsDirSize + pendAddrSpecSize +
					gtsDescripSize + pendAddrSize + cellSrchSize + bcnPayloadSize;

	uint16_t supframeSpecificField = (m_bcnOrder & 0x0f)
								   | ((m_supframeOrder & 0x0f) << 4)
								   | ((m_finalCapSlot & 0x0f) << 8)
								   | (((uint8_t)m_vpanCoord & 0x01) << 13)
								   | (((uint8_t)m_assocPermit & 0x01) << 14)
								   | (((uint8_t)m_cellSearchEn & 0x01) << 15);

	uint8_t gtsSpecificField = ((uint8_t)(m_gts.size() & 0x000000007))
							 | (((uint8_t)m_gtsPermit & 0x01) << 7);

	uint8_t gtsDirField = (m_gtsDirMask);

//	uint8_t shortAddrCount = GetShortAddressCount();
//	uint8_t extendedAddrCount = GetExtendedAddressCount();
	uint8_t pendingAddrSpecField = (m_shortAddrsCount & 0x07) | ((m_extendedAddrsCount & 0x07) << 4);

//	uint8_t cellEnLength = m_cellEnLenth;

	Buffer data;
	data.AddAtStart(size);
	Buffer::Iterator it = data.Begin();

	it.WriteU16(supframeSpecificField);
	it.WriteU8(gtsSpecificField);
	it.WriteU8(gtsDirField);
	WriteTo(it, m_gts);
	it.WriteU8(pendingAddrSpecField);
	WriteTo(it, m_pendingAddrs);
	it.WriteU8(m_cellEnLenth);
	WriteTo(it, m_bcnPayload, m_bcnPayloadSize);

	return data;
}

void LifiMacBeacon::Deserialize(const uint8_t *data, uint32_t size) {
	NS_LOG_FUNCTION (this << data << size);
	const uint8_t *end = data + size -1;
	uint8_t *cur = const_cast<uint8_t*> (data);

	uint16_t supframeSpecfield = *((uint16_t*)cur);
	m_bcnOrder = (uint8_t) (supframeSpecfield & 0x000f);
	m_supframeOrder = (uint8_t) ((supframeSpecfield >> 4) & 0x000f);
	m_finalCapSlot = (uint8_t) ((supframeSpecfield >> 8) & 0x000f);
	m_vpanCoord = (bool) ((supframeSpecfield >> 13) & 0x0001);
	m_assocPermit = (bool) ((supframeSpecfield >> 14) & 0x0001);
	m_cellSearchEn = (bool) ((supframeSpecfield >> 15) & 0x0001);
	cur += 2;

	uint8_t gtsSpecField = *cur;
	m_gtsDescriptorCount = gtsSpecField & 0x07;
	m_gtsPermit = (bool) ((gtsSpecField >> 7) & 0x01);
	cur += 1;

	uint8_t gtsDirField = *cur;
	m_gtsDirMask = gtsDirField;
	cur += 1;

	cur = ReadFrom(cur, m_gts, (uint32_t)m_gtsDescriptorCount);

	uint8_t pendAddrSpecfield = *cur;
	m_shortAddrsCount = (uint32_t) (pendAddrSpecfield & 0x07);
	m_extendedAddrsCount = (uint32_t) ((pendAddrSpecfield >> 4) & 0x07);
	cur += 1;

	cur = ReadFrom(cur, m_pendingAddrs, m_shortAddrsCount, m_extendedAddrsCount);

	uint8_t cellSrchLenth = *cur;
	m_cellEnLenth = cellSrchLenth;
	cur += 1;

	m_bcnPayloadSize = ReadFrom(m_bcnPayload, cur, end);

}

} /* namespace ns3 */
