/*
 *
 *lifi-mac-beacon-frame.h
 *    Author: wanghongqiao
 *     date  2014.4.11
 */
#ifndef LIFI_MAC_BEACON_FRAME_H
#define LIFI_MAC_BEACON_FRAME_H

#include "ns3/core-module.h"
#include "lifi-mac-general.h"
#include "lifi-mac.h"

namespace ns3{


class LifiMacBeacon
{

public:
	LifiMacBeacon();

	static LifiMacBeacon& Construct (Ptr<Packet> p);

	virtual ~LifiMacBeacon();

	Ptr<Packet> GetPacket();

	uint8_t GetBcnOrder () const;
	void SetBcnOrder (uint8_t order);

	uint8_t GetSupframeOrder () const;
	void SetSupframeOrder (uint8_t order);

	uint8_t GetFinalCapSlot () const;
	void SetFinalCapSlot (uint8_t slot);

	bool GetVpanCoord () const;
	void SetVpanCoord (bool coord);

	bool GetAssocPermit () const;
	void SetAssocPermit (bool permit);

	bool GetCellSearchEn () const;
	void SetCellSearchEn (bool enable);

	uint8_t GetCellSearchLenth () const;
	void SetCellSearchLenth (uint8_t lenth);

	uint8_t GetGtsDescripCount () const;
	void SetGtsDescripCount (uint8_t count);

	bool GetGtsPermit () const;
	void SetGtsPermit (bool permit);

	uint8_t GetGtsDirMask () const;
	void SetGtsDirMask ( uint8_t mask);

	bool AddGts (GtsDescriptor gts);
	bool RemoveGts (GtsDescriptor gts);

	bool AddPendingAddress (Address address);
	bool RemovePendingAddress (Address address);

	uint8_t* GetPayload (uint32_t *size);
	void SetPayload (uint8_t *data, uint32_t size);

	uint8_t GetShortAddressCount () const;

	uint8_t GetExtendedAddressCount () const;

	bool CheckPendingAddress (Address address);

	GtsList::iterator FindAssignedGts (Mac16Address address);

	AddrList& GetAddrs ();

private:

	LifiMacBeacon(Ptr<Packet> p);

	uint32_t WriteTo (Buffer::Iterator &it, AddrList address);

	uint32_t WriteTo (Buffer::Iterator &it, GtsList descrip);

	uint32_t WriteTo (Buffer::Iterator &it, uint8_t *data, uint32_t size);

	uint8_t* ReadFrom (const uint8_t* data, AddrList &address, uint32_t shorts, uint32_t extendeds);

	uint8_t* ReadFrom (const uint8_t* data, GtsList &descrip, uint32_t size);

	uint32_t ReadFrom (uint8_t* to, const uint8_t* start, const uint8_t* end);

	void Deserialize(const uint8_t *data, uint32_t size);

	Buffer Serialize();

	// Superframe Specification field
	uint8_t m_bcnOrder;
	uint8_t m_supframeOrder;
	uint8_t m_finalCapSlot;
	bool m_vpanCoord;
	bool m_assocPermit;
	bool m_cellSearchEn;

	// GTS Specification field
	uint8_t m_gtsDescriptorCount;
	bool m_gtsPermit;

	// GTS Directions field
	uint8_t m_gtsDirMask;

	// GTS List field
	GtsList m_gts;

	// Pending Address Specification field
	uint32_t m_shortAddrsCount;
	uint32_t m_extendedAddrsCount;

	// Address List field
	AddrList m_pendingAddrs;

	uint8_t m_cellEnLenth;

	uint32_t m_bcnPayloadSize;
	uint8_t m_bcnPayload[LifiMac::aMaxBeaconPayloadLength];
};



} /* namespace ns3 */

#endif /* LIFI_MAC_BEACON_FRAME_H_ */


