/*
 * MlmeSapProvider.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef MLME_SAP_PROVIDER_H_
#define MLME_SAP_PROVIDER_H_

#include "ns3/core-module.h"
#include "lifi-mac-general.h"
#include "lifi-mac-pib-attributes.h"

namespace ns3 {

class MlmeSapProvider : public SimpleRefCount<MlmeSapProvider>
{

public:
	MlmeSapProvider();
	virtual ~MlmeSapProvider();

	virtual void MlmeAssocaiteResponse(Mac64Address devAddr, Mac16Address assocShortAddr, MacOpStatus status, MacOpStatus capNegoResponse) = 0;

	virtual void MlmeAssociateRequest(LogicChannelId logicChannel, AddrMode coorAddrMode, uint16_t coorVPANId, Mac64Address coordAddr, CapabilityInfo capInfo) = 0;

	virtual void MlmeDisassociateRequest(TypeId devAddrMode, uint16_t devVPANId, Address devAddr, DisassocReason reason, bool txIndirect) = 0;

	template <class T>
	T MlmeGetRequest(MacPIBAttributeId id, uint32_t index)
	{
		return *(T*)Get(id, index);
	}

	virtual void MlmeGtsRequest(GTSCharacteristics characteristics) = 0;

	virtual void MlmePollRequest(TypeId corAddrMode, uint16_t coordVPANId, Address coordAddr) = 0;

	virtual void MlmeResetRequest() = 0;

	virtual void MlmeRxEnableRequest(bool deferPermit, uint32_t rxOnTime, uint32_t rxOnDuration) = 0;

	virtual void MlmeScanRequest(ScanType scanType, LogicChannelId scanChannels, uint32_t scanDuration) = 0;

	template <class T>
	void MlmeSetRequest(MacPIBAttributeId id, uint32_t index, T values)
	{
		T v = values;
		Set(id, index, (void*)(&v));
	}

	virtual void MlmeStartRequest(uint8_t vpanId, LogicChannelId channel, uint32_t startTime,
									uint32_t bcnOrder, uint32_t supframeOrder, bool vpanCoord) = 0;
	virtual void MlmeSyncRequest(LogicChannelId channel, bool trackBeacon) = 0;

protected:
	virtual void* Get (MacPIBAttributeId id, uint32_t index) = 0;

	virtual void Set (MacPIBAttributeId id, uint32_t index, void* values) = 0;

};

template <class C>
class MlmeSpecificSapProvider : public MlmeSapProvider
{
public:
	MlmeSpecificSapProvider (C* mac)
	{
		m_mac = mac;
	}

	virtual ~MlmeSpecificSapProvider()
	{

	}

	virtual void MlmeAssocaiteResponse(Mac64Address devAddr, Mac16Address assocShortAddr, MacOpStatus status,
								MacOpStatus capNegoResponse)
	{
		m_mac->AssociateResponse (devAddr, assocShortAddr, status, capNegoResponse);
	}

	virtual void MlmeAssociateRequest(LogicChannelId channel, AddrMode coorAddrMode, uint16_t coorVPANId,
										Mac64Address coordAddr, CapabilityInfo capInfo)
	{
		m_mac->Associate (channel, coorAddrMode, coorVPANId, coordAddr, capInfo);
	}

	virtual void MlmeDisassociateRequest(TypeId devAddrMode, uint16_t devVPANId, Address devAddr,
											DisassocReason reason, bool txIndirect)
	{
		m_mac->Disassociate (devAddrMode, devVPANId, devAddr, reason, txIndirect);
	}

	virtual void MlmeGtsRequest(GTSCharacteristics characteristics)
	{
		m_mac->GtsRequest (characteristics);
	}

	virtual void MlmePollRequest(TypeId corAddrMode, uint16_t coordVPANId, Address coordAddr)
	{
		m_mac->Poll (corAddrMode, coordVPANId, coordAddr);
	}

	virtual void MlmeResetRequest()
	{
		m_mac->Reset ();
	}

	virtual void MlmeRxEnableRequest(bool deferPermit, uint32_t rxOnTime, uint32_t rxOnDuration)
	{
		m_mac->RxEnable (deferPermit, rxOnTime, rxOnDuration);
	}

	virtual void MlmeScanRequest(ScanType scanType, LogicChannelId channel, uint32_t scanDuration)
	{
		m_mac->ScanChannel (scanType, channel, scanDuration);
	}

	virtual void MlmeStartRequest(uint8_t vpanId, LogicChannelId channel, uint32_t startTime,
										uint32_t bcnOrder, uint32_t supframeOrder, bool vpanCoord)
	{
		m_mac->StartVPAN (vpanId, channel, startTime, bcnOrder, supframeOrder, vpanCoord);
	}

	virtual void MlmeSyncRequest(LogicChannelId channel, bool trackBeacon)
	{
		m_mac->Synchronize (channel, trackBeacon);
	}

protected:
	virtual void* Get (MacPIBAttributeId id, uint32_t index)
	{
		return m_mac->GetMacPibAttributes()->GetAttributes (id);
	}

	virtual void Set (MacPIBAttributeId id, uint32_t index, void* values)
	{
		m_mac->GetMacPibAttributes()->SetAttributes (id, values);
	}

private:
	MlmeSpecificSapProvider ();

	C* m_mac;
};

} /* namespace ns3 */



#endif /* MLME_SAP_PROVIDER_H_ */
