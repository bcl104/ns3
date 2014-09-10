/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef LIFI_MAC_H_
#define LIFI_MAC_H_
#include "ns3/object.h"
#include "lifi-mac-pib-attributes.h"
#include "lifi-phy.h"
#include "mcps-sap-provider.h"
#include "mcps-sap-user.h"
#include "mlme-sap-provider.h"
#include "mlme-sap-user.h"
#include "pd-sap-provider.h"
#include "pd-sap-user.h"
#include "plme-sap-provider.h"
#include "plme-sap-user.h"

namespace ns3 {

class LifiPhy;
class LifiMacImpl;
class LifiNetDevice;


class LifiMac : public Object
{
	friend class PdSpecificSapUser<LifiMac>;
	friend class PlmeSpecificSapUser<LifiMac>;
	friend class MlmeSpecificSapProvider<LifiMac>;
	friend class McpsSpecificSapProvider<LifiMac>;
	friend class LifiMacImpl;
	friend class LifiMacCoordImpl;
	friend class LifiMacDevImpl;

public:

	static const uint32_t aBaseSlotDuration = 60;
	static const uint32_t aNumSuperframeSlots = 16;
	static const uint32_t aBaseSuperframeDuration = aBaseSlotDuration * aNumSuperframeSlots;
	static const uint32_t aGTSDescPersistenceTime = 4;
	static const uint32_t aMaxBeaconOverhead = 75;
	static const uint32_t aMinMpduOverhead = 9;
	static const uint32_t aMaxBeaconPayloadLength = MAX_PHY_FRAME_SIZE - aMaxBeaconOverhead;
	static const uint32_t aMaxLostBeacons = 4;
	static const uint32_t aMaxMacPayloadSize = MAX_PHY_FRAME_SIZE - aMinMpduOverhead;
	static const uint32_t aMaxMpudUnsecuredOverhead = 25;
	static const uint32_t aMaxMacSafePayloadSize = MAX_PHY_FRAME_SIZE - aMaxMpudUnsecuredOverhead;
	static const uint32_t aMaxSifsFrameSize = 18;
	static const uint32_t aMinCapLength = 2;
	static const uint32_t aUnitBackoffPeriod = 20;


	LifiMac();

	virtual ~LifiMac();

	static TypeId GetTypeId ();

	Ptr<LifiMacPibAttribute> GetMacPibAttributes ();

	void Send (AddrMode srcAddrMode, AddrMode dstAddrMode, uint16_t dstVPANId,
			   Address dstAddr, uint32_t msduLength, Ptr<Packet> msdu, uint8_t msduHandle,
			   const TxOption& txOption, DataRateId rate, bool burstMode);
	void Purge (uint8_t handle);

	void AssociateResponse (Mac64Address devAddr, Mac16Address assocShortAddr, MacOpStatus status,
								MacOpStatus capNegoResponse);

	void Associate (LogicChannelId channel, AddrMode coorAddrMode, uint16_t coorVPANId,
										Mac64Address coordAddr, CapabilityInfo capInfo);

	void Disassociate (AddrMode devAddrMode, uint16_t devVPANId, Address devAddr,	DisassocReason reason,
						bool txIndirect);

	void GtsRequest (GTSCharacteristics characteristics, Address dstAddr);

	void Poll (TypeId corAddrMode, uint16_t coordVPANId, Address coordAddr);

	void Reset();

	void RxEnable (bool deferPermit, uint32_t rxOnTime, uint32_t rxOnDuration);

	void ScanChannel (ScanType scanType, uint8_t scanChannels, uint32_t scanDuration);

	void StartVPAN (uint8_t vpanId, LogicChannelId channel, uint32_t startTime,
			uint32_t bcnOrder, uint32_t supframeOrder, bool vpanCoord);

	void Synchronize (LogicChannelId id, bool trackBeacon);

	void SwitchConfirm (PhyOpStatus status);

	const Ptr<McpsSapProvider>& GetMcpsSapProvider() const;

	void SetMcpsSapUser(const Ptr<McpsSapUser>& mcpsSapUser);

	const Ptr<MlmeSapProvider>& GetMlmeSapProvider() const;

	void SetMlmeSapUser(const Ptr<MlmeSapUser>& user);

	Ptr<MlmeSapUser> GetMlmeSapUser () const;

	void SetPdSapProvider(const Ptr<PdSapProvider>& provider);

	Ptr<PdSapProvider> GetPdSapProvider () const;

	const Ptr<PdSapUser>& GetPdSapUser() const;

	void SetPlmeSapProvider(const Ptr<PlmeSapProvider>& provider);

	Ptr<PlmeSapProvider> GetPlmeSapProvider () const;

	const Ptr<PlmeSapUser>& GetPlmeSapUser();

	const Ptr<LifiMacImpl>& GetLifiMacImpl () const;

	void SetLifiMacImpl (const Ptr<LifiMacImpl>& impl);

	void SetDevice (Ptr<LifiNetDevice> device);

	Ptr<LifiNetDevice> GetDevice () const;

	void SetOpticalPeriod (const Time* oc);

	const Time* GetOpticalPeriod () const;

protected:
	void DoDataConfirm(PhyOpStatus status);
	void DoReceive(uint32_t size, Ptr<Packet> p, uint8_t quality);
	void DoTransmit(uint32_t size, Ptr<Packet> pb, uint8_t band);

	Ptr<LifiMacImpl> m_lifiMacImpl;
	Ptr<LifiNetDevice> m_device;

	Ptr<McpsSapProvider> m_mcpsSapProvider;
	Ptr<McpsSapUser> m_mcpsSapUser;
	Ptr<MlmeSapProvider> m_mlmeSapProvider;
	Ptr<MlmeSapUser> m_mlmeSapUser;
	Ptr<PdSapProvider> m_pdSapProvider;
	Ptr<PdSapUser> m_pdSapUser;
	Ptr<PlmeSapProvider> m_plmeSapProvider;
	Ptr<PlmeSapUser> m_plmeSapUser;

	const Time* m_opticalPeriod;

};

}

#endif /* LIFI_H */

