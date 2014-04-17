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

class LifiMacImpl : public Object
{

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
	virtual void Associate(LogicChannelId channel, TypeId coordAddrMode, uint16_t coordVPANId,
							Address coordAddr, CapabilityInfo info);

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
	virtual void Disassociate(TypeId devAddrMode, uint16_t devVPANId, Address devAddr,
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
									MacOpStatus status, MacOpStatus capResponse);

	/*
	 * 	This method allows a device to send a request to the coordinator to allocate a new GTS or to
	 * 	deallocate an existing GTS. It is also used by the coordinator to initiate a GTS deallocation.
	 *
	 * 	\param characteristic The characteristic of the GTS request.
	 * */
	virtual void GtsRequest(GTSCharacteristics characteristic);

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
	virtual void Receive(Ptr<PacketBurst> pb);

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
	virtual void Scan(ScanType scanType, uint8_t channel, uint32_t scanDuration);

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
	virtual void SendData(TypeId srcAddrMode, TypeId dstAddrMode, uint16_t dstVPANId, Address dstAddr,
							uint32_t size, Ptr<Packet> msdu, uint8_t handle, TxOption option,
							DataRateId rate);

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

protected:

	/*
	 * 	This method will call the correspond physics-layer primitive to activate the carrier sensing
	 * 	procedure .
	 * */
	bool DoChannelClearAccessment();

	/*
	 * 	This method pass a packet-burst to physics layer.
	 *
	 * 	\param 	size	The size of the packet-burst.
	 * 	\param	pb		Mpdu
	 * 	\param	band	The band plan identifier of the channel on which the data being transmitted.
	 * */
	void DoTransmitData(uint32_t size, Ptr<PacketBurst> pb, uint8_t band);

	/*
	 * 	This method will be invoked while receiving an acknowledgment frame.
	 * */
	virtual void onAcknowledge(uint32_t timestamp, Ptr<Packet> msdu);

	/*
	 * 	This method will be invoked when a coordinator receive an association
	 * 	request command from a device.
	 * */
	virtual void onAssocRequest(uint32_t timestamp, Ptr<Packet> msdu);

	/*
	 * 	This method will be invoked when a device receive an association response
	 * 	command from the coordinator.
	 * */
	virtual void onAssocResponse(uint32_t timestamp, Ptr<Packet> msdu);

	/*
	 * 	Not implemented.
	 * */
	virtual void onBandHopping(uint32_t timestamp, Ptr<Packet> msdu);

	/*
	 * 	This method will be invoked when a device receive beacon from the coordinator.
	 * */
	virtual void onBeacon(uint32_t timestamp, Ptr<Packet> msdu);

	/*
	 * 	This method will be invoked when a coordinator receive an beacon request command
	 * 	from a device.
	 * */
	virtual void onBeaconRequest(uint32_t timestamp, Ptr<Packet> msdu);

	/*
	 *	This method will be invoked when a device or coordinator receive a data frame.
	 * */
	virtual void onData(uint32_t timestamp, Ptr<Packet> msdu);

	/*
	 * 	This method will be invoked by physics layer through PD-DATA.confirm primitive to notify
	 * 	the result of the transmission.
	 * */
	virtual void onDataConfirm(PhyOpStatus status);

	/*
	 * 	This method will be invoked when the coordinator receive a data reqeust command frome a
	 * 	device.
	 * */
	virtual void onDataRequest(uint32_t timestamp, Ptr<Packet> msdu);

	/*
	 * 	This method will be invoked when the device or coordinator receive a disassociation
	 * 	notification command.
	 * */
	virtual void onDisassocNotification(uint32_t timestamp, Ptr<Packet> msdu);

	/*
	 * 	This method will be invoked when a device or coordinator receive a GTS request command.
	 * */
	virtual void onGTSRequest(uint32_t timestamp, Ptr<Packet> msdu);

	/*
	 * 	This method will be invoked when a device or coordinator receive a GTS response command.
	 * */
	virtual void onGTSResponse(uint32_t timestamp, Ptr<Packet> msdu);

	/*
	 * 	This method will be invoked when a device or coordinator receive an information element
	 * 	appending to any mac command frame.
	 * */
	virtual void onInformationElement(uint32_t timestamp, Ptr<Packet> msdu);

	/*
	 * 	This method will be invoked when a device receive a multiple channel assignment from the
	 * 	coordinator.
	 * */
	virtual void onMultipleChannelAssignment(uint32_t timestamp, Ptr<Packet> msdu);

	Ptr<LifiMac> m_mac;
	LifiMacPibAttribute* m_attributes;

};

} /* namespace ns3 */

#endif /* LIFI_MAC_IMPL_H_ */