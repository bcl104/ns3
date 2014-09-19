/*
 * lifi-node.h
 *
 *  Created on: 2014年9月16日
 *      Author: wanghongqiao
 */

#ifndef LIFI_NODE_H_
#define LIFI_NODE_H_

#include "ns3/core-module.h"
#include "ns3/object.h"
#include "mcps-sap-provider.h"
#include "mcps-sap-user.h"
#include "mlme-sap-provider.h"
#include "mlme-sap-user.h"
#include "lifi-mac.h"

namespace ns3{

class LifiNode : public Object {

friend class McpsSpecificSapUser<LifiNode>;
friend class MlmeSpecificSapUser<LifiNode>;

public:

	LifiNode();
	virtual ~LifiNode();
	static TypeId GetTypeId ();
	virtual void Start(uint8_t vpanId, LogicChannelId channel, uint32_t startTime,	uint32_t bcnOrder,
					   uint32_t supframeOrder, bool vpanCoord);

	virtual void DataConfirm(uint8_t msduHandle, MacOpStatus status, uint32_t timestamp);
	virtual void DataIndication(DataIndicaDescriptor dataDesc);
	virtual void AssociateConfirm(Mac16Address assocShortAddr, MacOpStatus status, MacColorStabCapab capNigoResponse);
	virtual void AssociateIndication(Mac64Address devAddr, CapabilityInfo capInfo);
	virtual void BeaconNotify(uint8_t bsn, VPANDescriptor vpanDiscriptor, PendAddrSpec pendAddrSpec, AddrList addrList, uint32_t payloadSize, uint8_t* payload);
	virtual void CommStatusIndication(uint16_t vpanId, AddrMode srcAddrMode, Address srcAddr, AddrMode dsAddrMode, Address dstAddr, MacOpStatus status);
	virtual void DisassociateConfirm(MacOpStatus status, AddrMode devAddrMode, uint16_t devVPANId, Address devAddr);
	virtual void DisassociateIndication(Mac64Address devAddr, DisassocReason disassociateReason);
	virtual void GtsConfirm(GTSCharacteristics characteristics, MacOpStatus status);
	virtual void GtsIndication(Mac16Address devAddr, GTSCharacteristics charateristic);
	virtual void PollConfirm(MacOpStatus status);
	virtual void ResetConfirm(MacOpStatus status);
	virtual void RxEnableConfirm(MacOpStatus status);
	virtual void ScanConfirm(MacOpStatus status, ScanType scanType, uint8_t unscanChannels, uint32_t resultListSize, VpanDescriptors &descriptors);
	virtual void SetConfirm(MacOpStatus ststus, MacPIBAttributeId id, uint32_t index);
	virtual void StartConfirm(MacOpStatus status);
	virtual void SyncLossIndication(MacOpStatus lossReason, uint16_t vpanId, LogicChannelId channel);

	virtual void DataConfirmMcps(uint8_t msduHandle, MacOpStatus status, uint32_t timestamp);
	virtual void DataIndicationMcps(TypeId srcAddrMode, uint16_t srcVPANId, Address srcAddr,
									TypeId dstAddrMode, Address dstAddr, uint32_t msduLength,
									Ptr<Packet> msdu, uint8_t mpduLinkQuality, uint8_t dsn,
									uint32_t timestamp, DataRateId dataRate, bool burstMode);
	virtual void PurgeConfirmMcps(uint8_t msduHandle, MacOpStatus status);

protected:

	Ptr<McpsSapProvider> m_mcpsSapProvider;
	Ptr<McpsSapUser> m_mcpsSapUser;
	Ptr<MlmeSapProvider> m_mlmeSapProvider;
	Ptr<MlmeSapUser> m_mlmeSapUser;
	Ptr<LifiMac> m_lifiMac;
};

}

#endif /* LIFI_NODE_H_ */
