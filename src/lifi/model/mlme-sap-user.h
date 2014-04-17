/*
 * MlmeSapUser.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef MLME_SAP_USER_H_
#define MLME_SAP_USER_H_

#include "ns3/simple-ref-count.h"
#include "ns3/lifi-mac-general.h"

namespace ns3 {

class MlmeSapUser : public SimpleRefCount<MlmeSapUser>
{

public:
	MlmeSapUser();
	virtual ~MlmeSapUser();

	virtual void MlmeAssociateConfirm(Mac16Address assocShortAddr, MacOpStatus status, MacOpStatus capNigoResponse) = 0;
	virtual void MlmeAssociateIndication(Mac64Address devAddr, CapabilityInfo capInfo) = 0;
	virtual void MlmeBeaconNotify(uint8_t bsn, VPANDescriptor vpanDiscriptor, PendAddrSpec pendAddrSpec, AddrList addrList, uint32_t size, uint8_t* payload) = 0;
	virtual void MlmeCommStatusIndication(uint16_t vpanId, TypeId srcAddrMode, Address srcAddr, TypeId dsAddrMode, Address dstAddr, MacOpStatus status) = 0;
	virtual void MlmeDisassociateConfirm(MacOpStatus status, TypeId devAddrMode, uint16_t devVPANId, Address devAddr) = 0;
	virtual void MlmeDisassociateIndication(Mac64Address devAddr, MacOpStatus disassociateReason) = 0;
	virtual void MlmeGtsConfirm(GTSCharacteristics characteristics, MacOpStatus status) = 0;
	virtual void MlmeGtsIndication(Mac16Address devAddr, GTSCharacteristics charateristic) = 0;
	virtual void MlmePollConfirm(MacOpStatus status) = 0;
	virtual void MlmeResetConfirm(MacOpStatus status) = 0;
	virtual void MlmeRxEnableConfirm(MacOpStatus status) = 0;
	virtual void MlmeScanConfirm(MacOpStatus status, ScanType scanType, uint8_t unscanChannels, uint32_t resultListSize, std::map<uint8_t, VPANDescriptor>& descriptors) = 0;
	virtual void MlmeSetConfirm(MacOpStatus ststus, MacPIBAttributeId id, uint32_t index) = 0;
	virtual void MlmeStartConfirm(MacOpStatus status) = 0;
	virtual void MlmeSyncLossIndication(MacOpStatus lossReason, uint16_t vpanId, LogicChannelId channel) = 0;

};

template <class C>
class MlmeSpecificSapUser : public MlmeSapUser
{
public:
	MlmeSpecificSapUser (C* sscs)
	{
		m_sscs = sscs;
	}

	virtual ~MlmeSpecificSapUser ();

	virtual void MlmeAssociateConfirm(Mac16Address assocShortAddr, MacOpStatus status,
										MacOpStatus capNigoResponse)
	{
		m_sscs->AssociateConfirm (assocShortAddr, status, capNigoResponse);
	}

	virtual void MlmeAssociateIndication(Mac64Address devAddr, CapabilityInfo capInfo)
	{
		m_sscs->AssociateIndication (devAddr, capInfo);
	}

	virtual void MlmeBeaconNotify(uint8_t bsn, VPANDescriptor vpanDiscriptor,
									PendAddrSpec pendAddrSpec, AddrList addrList,
									uint32_t size, uint8_t *payload)
	{
		m_sscs->BeaconNotification (bsn, vpanDiscriptor, pendAddrSpec, addrList, size, payload);
	}

	virtual void MlmeCommStatusIndication(uint16_t vpanId, TypeId srcAddrMode, Address srcAddr,
											TypeId dstAddrMode, Address dstAddr, MacOpStatus status)
	{
		m_sscs->CommStatusIndication (vpanId, srcAddrMode, srcAddr, dstAddrMode, dstAddr, status);
	}

	virtual void MlmeDisassociateConfirm(MacOpStatus status, TypeId devAddrMode, uint16_t devVPANId,
											Address devAddr)
	{
		m_sscs->DisassociateConfirm (status, devAddrMode, devVPANId, devAddr);
	}

	virtual void MlmeDisassociateIndication(Mac64Address devAddr, MacOpStatus reason)
	{
		m_sscs->DisassociateIndication (devAddr, reason);
	}

	virtual void MlmeGtsConfirm(GTSCharacteristics characteristics, MacOpStatus status)
	{
		m_sscs->GtsConfirm (characteristics, status);
	}

	virtual void MlmeGtsIndication(Mac16Address devAddr, GTSCharacteristics charateristic)
	{
		m_sscs->GtsIndication (devAddr, charateristic);
	}

	virtual void MlmePollConfirm(MacOpStatus status)
	{
		m_sscs->PollConfirm (status);
	}

	virtual void MlmeResetConfirm(MacOpStatus status)
	{
		NS_ASSERT_MSG(false, "MlmeResetConfirm should not be invoked.");
	}

	virtual void MlmeRxEnableConfirm(MacOpStatus status)
	{
		NS_ASSERT_MSG(false, "MlmeRxEnableConfirm should not be invoked.");
	}
	virtual void MlmeScanConfirm(MacOpStatus status, ScanType scanType, uint8_t unscanChannels,
									uint32_t size, std::map<uint8_t, VPANDescriptor>& descriptors)
	{
		m_sscs->ScanConfirm (status, scanType, unscanChannels, size, descriptors);
	}

	virtual void MlmeSetConfirm(MacOpStatus ststus, MacPIBAttributeId id, uint32_t index)
	{
		NS_ASSERT_MSG(false, "MlmeSetConfirm should not be invoked.");
	}
	virtual void MlmeStartConfirm(MacOpStatus status)
	{
		NS_ASSERT_MSG(false, "MlmeStartConfirm should not be invoked.");
	}
	virtual void MlmeSyncLossIndication(MacOpStatus lossReason, uint16_t vpanId, LogicChannelId channel)
	{
		m_sscs->SyncLossIndication (lossReason, vpanId, channel);
	}

private:
	MlmeSpecificSapUser ();

	C* m_sscs;
};

} /* namespace ns3 */

#endif /* MLME_SAP_USER_H_ */