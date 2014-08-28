/*
 * lifi-mac-pib-attributes.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef LIFI_MAC_PIB_ATTRIBUTES_H_
#define LIFI_MAC_PIB_ATTRIBUTES_H_
#include <map>
#include "ns3/core-module.h"
#include "lifi-pib-attributes.h"
#include "lifi-mac-general.h"
//#include "lifi-mac-coord-impl.h"
//#include "lifi-mac-dev-impl.h"

namespace ns3 {

/**
 * This is the base class of medium access control entity of both coordinator and
 * device.
 */
class LifiMacPibAttribute : public LifiPIBAttribute
{
	friend class LifiMacImpl;
	friend class LifiMacCoordImpl;
	friend class LifiMacDevImpl;
	friend class LifiChannelScanHandler;
	friend class LifiAssocHandler;
	friend class LifiDevAssocHandler;
	friend class LifiTrxHandler;
	friend class LifiCoordTrxHandler;
	friend class LifiDevTrxHandler;
	friend class LifiTransactionHandler;
	friend class LifiDisassocDevHandler;
	friend class LifiDisassocCoordHandler;
	friend class LifiGtsDevHandler;
	friend class LifiGtsCoordHandler;
	friend class LifiDataDevHandler;
	friend class LifiDataCoordHandler;

public:
	LifiMacPibAttribute();

	virtual ~LifiMacPibAttribute();

	void* GetAttributes (MacPIBAttributeId id)
	{
		std::map<uint8_t, void*>::iterator it = m_attributes.find((uint8_t)id);
		NS_ASSERT (it != m_attributes.end());
		return it->second;
	}

	void SetAttributes (MacPIBAttributeId id, void* value);

	void Reset ();

private:

	/**
	 * The maximum number of optical clocks to wait for an acknowledgment frame to
	 * arrive following a transmitted data frame. This value is dependent on the
	 * supported PHY, which determines both the selected logical channel. The calculated
	 * value is the time to commence transmitting the ACK plus the length of the ACK frame.
	 * The commencement time is described in 5.1.7.4.2.
	 */
	uint32_t macAckWaitDuration;

	/**
	 * Indication of whether the device is associated to the VPAN through the coordinator.
	 * A value of TRUE indicates the device has associated through the coordinator. Otherwise,
	 * the value is set to FALSE.
	 */
	bool macAssociatedVPANCoord;

	/**
	 * Indication of whether a coordinator is currently allowing association. A value of TRUE
	 * indicates that association is permitted.
	 */
	bool macAssociationPermit;

	/**
	 * Indication of whether a device automatically sends a data request command if its address
	 * is listed in the beacon frame. A value of TRUE indicates that the data request command
	 * is automatically sent. This attribute also affects the generation of the
	 * MLME-BEACON-NOTIFY.indication primitive (see 6.3.3.1.1).
	 */
	bool macAutoRequest;

	/**
	 * Specification of how often the coordinator transmits its beacon. If BO= 15, the coordinator
	 * will not transmit a periodic beacon. Refer to 5.1.1.1 for an explanation of the relationship
	 * between the beacon order and the beacon interval. Range:0–15
	 */
	uint32_t macBeaconOrder;

	/**
	 * The contents of the beacon payload.
	 */
	Buffer macBeaconPayload;

	/**
	 * The length, in octets, of the beacon payload.Range:0 –aMaxBeaconPayloadLength
	 */
	uint32_t macBeaconPayloadLength;

	/**
	 * The time that the device transmitted its last beacon frame, in symbol periods. The
	 * measurement shall be taken at the same symbol boundary within every transmitted
	 * beacon frame, the location of which is implementation specific. This is a 24-bit value,
	 * and the precision of this value shall be a minimum of 20 bits, with the lowest four bits
	 * being the least significant. Range:0x000000–0xffffff
	 */
	uint32_t macBeaconTxTime;

	/**
	 * The sequence number added to the transmitted beacon frame.
	 */
	uint8_t macBSN;

	/*
	 * The 64-bit address of the coordinator through which the device is associated.
	 * */
	Mac64Address macCoordExtendedAddress;

	/**
	 * The 16-bit short address assigned to the coordinator through which the device is
	 * associated. A value of 0xfffe indicates that the coordinator is only using its
	 * 64-bit extended address. A value of 0xffff indicates that this value is unknown.
	 * Range:0x0000–0xffff
	 */
	Mac16Address macCoordShortAddress;

	/**
	 * The sequence number added to the transmitted data or MAC command frame.
	 */
	uint32_t macDSN;

	/**
	 * TRUE if the coordinator is to accept GTS requests. FALSE otherwise.
	 */
	bool macGTSPermit;

	/**
	 * A timer initiated when the link recovery procedure is triggered. If the timer expires
	 * while the device has not received any fast link recovery response (FLR RSP) signal since
	 * the fast link recovery procedure is triggered, the device assumes that the link is broken
	 * and cannot be recovered. The range for macLinkTimeOut is defined in terms of the number
	 * of superframes.Range:0-255
	 */
	uint32_t macLinkTimeOut;

	/**
	 * The maximum value of the backoff exponent, BE, in the unslotted random access algorithm.
	 * Refer to 5.1.1.3 for a detailed explanation of the backoff exponent.Range:3–15
	 */
	uint32_t macMaxBE;

	/**
	 * The maximum number of backoffs the unslotted random access algorithm will attempt before
	 * declaring a channel access failure.Range:0-5
	 */
	uint32_t macMaxCSMABackoffs;

	/**
	 * The maximum number of retries allowed after a transmission failure.Range:0–7
	 */
	uint32_t macMaxFrameRetries;

	/**
	 * The maximum number of optical clocks in a beacon-enabled VPAN, or in a non-beacon-enabled
	 * VPAN, to wait either for a frame intended as a response to a data request frame or for a
	 * broadcast frame following a beacon with the frame pending subfield set to one. This attribute,
	 * which shall only be set by the next higher layer, is dependent upon macMinBE, macMaxBE,
	 * macMaxCSMABackoffs and the number of optical clocks per octet. Refer to 6.4.2 for the formula
	 * relating the attributes.Range:Refer to Equation (2)
	 */
	uint32_t macMaxFrameTotalWaitTime;

	/**
	 * The minimum value of the backoff exponent (BE) in the unslotted random access algorithm. Refer
	 * to 5.1.1.3 for a detailed explanation of the backoff exponent. Range: 0–macMaxBE
	 */
	uint32_t macMinBE;

	/**
	 * The minimum number of optical clocks forming a LIFS period.Range:As defined in Table 77 in
	 * 8.3.4
	 */
	uint32_t macMinLIFSPeriod;

	/**
	 * The minimum number of optical clocks forming a SIFS period. Range: As defined in Table 77 in
	 * 8.3.4
	 */
	uint32_t macMinSIFSPeriod;

	/**
	 * Maximum number of times not receiving ACKs to trigger fast link recovery procedure. Range: 0–15
	 */
	uint32_t macNumAcks;

	/**
	 * The maximum time, in multiples of aBaseSuperframeDuration, a device shall wait for a response
	 * command frame to be available following a request command frame.
	 */
	uint32_t macResponseWaitTime;

	/**
	 * Indication of whether the MAC sublayer is to enable its receiver during idle periods. For a
	 * beacon-enabled VPAN, this attribute is relevant only during the CAP of the incoming superframe.
	 * For a non-beacon-enabled VPAN, this attribute is relevant at all times.
	 */
	bool macRxOnWhenIdle;

	/**
	 * The 16-bit address that the device uses to communicate in the VPAN. If the device is the coordinator,
	 * this value shall be chosen before a VPAN is started. Otherwise, the address is allocated by a coordinator
	 * during association. A value of 0xfffe indicates that the device has associated but has not been
	 * allocated an address. A value of 0xffff indicates that the device does not have a short address.
	 * Range: 0x0000–0xffff
	 */
	uint16_t macShortAddress;

	/**
	 * The length of the active portion of the outgoing superframe, including the beacon frame. If superframe
	 * order, SO, = 15, the superframe will not be active following the beacon. Refer to 5.1.1.1 for
	 * an explanation of the relationship between the superframe order and the superframe duration.
	 * Range: 0–15
	 */
	uint32_t macSuperframeOrder;

	/**
	 * The location of the time stamp after the end of the preamble in optical clocks.
	 */
	unsigned char macTimeStampOffset;

	/**
	 * Indication of whether the MAC sublayer supports the optional time stamping feature for
	 * incoming and outgoing data frames.
	 */
	bool macTimestampSupported;

	/**
	 * The maximum time (in unit periods) that a transaction is stored by a coordinator and
	 * indicated in its beacon. The unit period is governed by macBeaconOrder, BO, as follows:
	 * For 0 BO 14, the unit period will beaBaseSuperframeDuration* 2* BO.For BO = 15, the
	 * unit period will beaBaseSuperframeDuration. Range: 0x0000-0xffff
	 */
	uint32_t macTransactionPersistenceTime;

	/**
	 * The 16-bit identifier of the VPAN on which the device is operating. If this value is 0xffff,
	 * the device is not associated. Range: 0x0000–0xffff
	 */
	uint16_t macVPANId;

};

} /* namespace ns3 */

#endif /* LIFI_MAC_PIB_ATTRIBUTES_H_ */
