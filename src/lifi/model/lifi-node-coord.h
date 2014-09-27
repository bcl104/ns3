/*
 * lifi-node-coord.h
 *
 *  Created on: 2014年9月16日
 *      Author: wanghongqiao
 */

#ifndef LIFI_NODE_COORD_H_
#define LIFI_NODE_COORD_H_

#include "lifi-node.h"

namespace ns3{

class LifiNodeCoord : public LifiNode {

public:
	LifiNodeCoord();
	virtual ~LifiNodeCoord();
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

	virtual void SendData(SendDataInfo dataInfo);
	virtual void GtsRequest(GTSCharacteristics characteristics, Address dstAddr);
	virtual void DisassocRequst(AddrMode devAddrMode, uint16_t devVPANId,
							    Address devAddr, DisassocReason reason, bool txIndirect);

protected:
	Ptr<LifiNode> m_sscs;
};

}

#endif /* LIFI_NODE_COORD_H_ */
