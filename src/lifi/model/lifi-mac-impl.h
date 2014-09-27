/*
 * lifi-mac-impl.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef LIFI_MAC_IMPL_H_
#define LIFI_MAC_IMPL_H_

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "lifi-mac-general.h"
#include "lifi-phy-general.h"
#include "lifi-mac.h"

namespace ns3 {

class LifiMac;

class LifiMacListener
{
	virtual ~LifiMacListener () {};
	virtual void SuperframeStart (Time cap, Time cfp, Time idle) = 0;
	virtual void CapEnd () = 0;
	virtual void CfpEnd () = 0;
	virtual void SuperframeEnd (Time duration) = 0;
};

class LifiMacImpl : public Object
{
	friend class LifiTrxHandler;
	friend class LifiCoordTrxHandler;
	friend class LifiDevTrxHandler;
public:
	LifiMacImpl();
	virtual ~LifiMacImpl();
	static TypeId GetTypeId ();

	/*
	 *	This method should be implemented as lifi-device, which
	 *	activate the association process to the coordinator.
	 *
	 *	\param channel Logic-channel identifier on which to attempt associate.
	 *	\param coordAddressMode Mac address mode of the coordinator.
	 *	\param coordVPANId VPAN identifier of the coordinator to associate.
	 *	\param coorAddress Mac address of the coordinator.
	 *	\param info Specifies the operational capabilities of the associating device.
	 * */
	virtual void Associate(LogicChannelId channel, AddrMode coordAddrMode, uint16_t coordVPANId,
						   Mac64Address coordAddr, CapabilityInfo info);

	/*
	 * 	Both coordinator and device should implement this method, which activate the disassociation
	 * 	process.
	 *
	 * 	\param devAddrMode Mac address mode of the device.
	 * 	\param devVPANId VPAN identifier of the device.
	 * 	\param devAddr	Mac address of the device
	 * 	\param reason	The reason of the disassociation.
	 * 	\param txIndirect TRUE if the disassociation notification command is to be sent indirectly.
	 * */
	virtual void Disassociate(AddrMode devAddrMode, uint16_t devVPANId, Address devAddr,
								DisassocReason reason, bool txIndirect);

	/*
	 * 	This method should be implemented as lifi-coordinator, through which the upper layer of the
	 * 	coordinator send an association-response-command to the device.
	 *
	 * 	\param devAddr 64-bit extended address of the device.
	 * 	\param assocShortAddress Short address to be assigned to the device. '0xfffe' indicates that
	 * 		   device should use its 64-bit extended address to communicate with coordinator. '0xffff'
	 * 		   indicates the coordinator reject the association reqeust.
	 * 	\param status The association result.
	 * 	\param capResponse The coordinator indicates who will send color compensation information.
	 * */
	virtual void AssociateResponse(Mac64Address devAddr, Mac16Address assocShortAddr,
									MacOpStatus status, MacColorStabCapab capResponse);

	/*
	 * 	This method allows a device to send a request to the coordinator to allocate a new GTS or to
	 * 	deallocate an existing GTS. It is also used by the coordinator to initiate a GTS deallocation.
	 *
	 * 	\param characteristic The characteristic of the GTS request.
	 * 	\param dstAddr The destination address of the GTS request.
	 * */
	virtual void GtsRequest(GTSCharacteristics characteristic, Address dstAddr);

	/*
	 * 	This method prompt the device to request data from a coordinator.
	 *
	 * 	\param coordAddrMode Address mode of the coordinator.
	 * 	\param coordVPANId Coordinator VPAN identifier.
	 * 	\param coordAddr The mac address of the coordinator.
	 * */
	virtual void Polling(TypeId coordAddrMode, uint16_t coordVPANId, Address coordAddr);

	/*
	 * 	Purge a msdu from the transaction queue.
	 *
	 * 	\param handle The handle of the msdu.
	 * */
	virtual void PurgeTrancsion(uint8_t handle);

	/*
	 *	This method should be invoked by lower layer primitive to receive a packet from the physics layer.
	 *
	 *	\param pb Packet burst received from the physics layer.
	 * */
	virtual void Receive(uint32_t size, Ptr<Packet> p, uint8_t quality);

	/*
	 * 	Initialize all the parameter of the lifi-mac-impl.
	 * */
	virtual void Reset();

	/*
	 * 	Enable the receiver for a finite period or disable.
	 *
	 * 	\param deferPermit Whether the rx-enable request can be deferred to next superframe if the
	 * 					   the requsted time has passed.
	 * 	\param rxOnTime The number of optical clock measured from the start of the superframe before
	 * 					the receiver to be enable or disable.
	 * 	\param txOnDuration The number of optical clock for which the receiver to be enabled.
	 * */
	virtual void RxEnable(bool deferPermit, uint32_t rxOnTime, uint32_t rxOnDuration);

	/*
	 *	Activate the chennel scanning procedure.
	 *
	 *	\param	scanType	Active scan or passive scan.
	 *	\param	channel		Logic channel identifier of the channel on which the device attempt to scan.
	 *	\param	scanDuration	The time spent in each channel in optical clock.
	 * */
	virtual void Scan(ScanType scanType, uint32_t scanDuration);

	/*
	 * 	This method pass a msdu to the tx buffer.
	 *
	 * 	\param	srcAddrMode
	 * 	\param	dstAddrMode
	 * 	\param	dstVPANId
	 * 	\param	dstAddr
	 * 	\param	size	The size in bytes of the msdu.
	 * 	\param	msdu
	 * 	\param	handle	The handle of the specific msdu.
	 * 	\param	option
	 * 	\param	rate
	 * */
	virtual void SendData(AddrMode srcAddrMode, AddrMode dstAddrMode, uint16_t dstVPANId, Address dstAddr,
							uint32_t size, Ptr<Packet> msdu, uint8_t handle, TxOption option,
							DataRateId rate, bool burstMode);

	/*
	 * 	This method should be implemented as coordinator to start a new VPAN.
	 *
	 * 	\param	vpanId
	 * 	\param	channel
	 * 	\param	startTime
	 * 	\param	beaconOrder
	 * 	\param	superFrameOrder
	 * */
	virtual void StartVPAN(uint16_t vpanId, LogicChannelId channel, uint32_t startTime,
						uint32_t beaconOrder, uint32_t supframeOrder, bool vpanCoord);

	/*
	 * 	Start or stop a synchronization on a specific channel.
	 *
	 * 	\param	channel	Logic channel identifier of the channel on which the device track the beacon.
	 * 	\param	trackBeacon
	 * */
	virtual void Synchronize(LogicChannelId channel, bool trackBeacon);

	virtual void DataConfirm (PhyOpStatus status);

	virtual void SetLifiMac (Ptr<LifiMac> mac);

	Ptr<LifiMac> GetLifiMac () const;

	virtual void SetOpticalPeriod (const Time* oc);

	const Time* GetOpticalPeriod () const;

	LifiMacPibAttribute& GetLifiMacPibAttribute ();

	virtual void SetPdSapProvider (Ptr<PdSapProvider> p);

	virtual void SetPlmeSapProvider (Ptr<PlmeSapProvider> p);

	virtual void SetMlmeSapUser (Ptr<MlmeSapUser> u);

	virtual void SetMcpsSapUser (Ptr<McpsSapUser> u);

	virtual void AddGtsTransactionPacket (GtsTransactionInfo& gtsTransInfo);
	virtual void AddTransactionPacket (TransactionInfo& transInfo);
	virtual void PetchTransactionPacket (Mac64Address DevAddress);

	virtual void SetGtsTransmitArgument (uint16_t shortAddr, bool transmitState);
	virtual void OpenGtsDataReceive(uint16_t devAddr);
	virtual void CloseGtsDataReceive();
	virtual void EndGtsTransmit();
	virtual void SendGtsDatas();
protected:

	Ptr<LifiMac> m_mac;

	LifiMacPibAttribute m_attributes;

	const Time* m_opticalPeriod;

};

} /* namespace ns3 */

#endif /* LIFI_MAC_IMPL_H_ */
