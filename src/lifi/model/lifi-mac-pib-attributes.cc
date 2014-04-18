/*
 * lifi-mac-pib-attributes.cc
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#include "lifi-mac-pib-attributes.h"

namespace ns3 {

LifiMacPibAttribute::LifiMacPibAttribute() {
	LOAD_PIB_ATTRIBUTE (MAC_ACK_WAIT_DURATION,macAckWaitDuration,0);
//	LOAD_PIB_ATTRIBUTE (MAC_ASSOC_VPAN_COORD,macAssociatedVPANCoord);
//	LOAD_PIB_ATTRIBUTE (MAC_ASSOC_PERMIT,macAssociationPermit);
	LOAD_PIB_ATTRIBUTE (MAC_AUTO_REQEST,macAutoRequest, true);
//	LOAD_PIB_ATTRIBUTE (MAC_BEACON_ORDER,macBeaconOrder);
//	LOAD_PIB_ATTRIBUTE (MAC_BEACON_PAYLOAD,macBeaconPayload);
//	LOAD_PIB_ATTRIBUTE (MAC_BEACON_PAYLOAD_LENGTH,macBeaconPayloadLength);
//	LOAD_PIB_ATTRIBUTE (MAC_BEACON_TX_TIME,macBeaconTxTime);
	LOAD_PIB_ATTRIBUTE (MAC_BSN,macBSN, 0);
//	LOAD_PIB_ATTRIBUTE (MAC_COORD_EXTENDED_ADDRESS,macCoordExtendedAddress);
//	LOAD_PIB_ATTRIBUTE (MAC_COORD_SHORT_ADDRESS,macCoordShortAddress);
//	LOAD_PIB_ATTRIBUTE (MAC_DSN,macDSN);
//	LOAD_PIB_ATTRIBUTE (MAC_GTS_PERMIT,macGTSPermit);
//	LOAD_PIB_ATTRIBUTE (MAC_LINK_TIME_OUT,macLinkTimeOut);
//	LOAD_PIB_ATTRIBUTE (MAC_MAX_BE,macMaxBE);
//	LOAD_PIB_ATTRIBUTE (MAC_MAX_CSMA_BACKOFFS,macMaxCSMABackoffs);
//	LOAD_PIB_ATTRIBUTE (MAC_MAX_FRAME_RETRIES,macMaxFrameRetries);
//	LOAD_PIB_ATTRIBUTE (MAC_MAX_FRAME_TOTAL_WAIT_TIME,macMaxFrameTotalWaitTime);
//	LOAD_PIB_ATTRIBUTE (MAC_MIN_BE,macMinBE);
//	LOAD_PIB_ATTRIBUTE (MAC_MIN_LIFS_PERIOD,macMinLIFSPeriod);
//	LOAD_PIB_ATTRIBUTE (MAC_MIN_SIFS_PERIOD,macMinSIFSPeriod);
//	LOAD_PIB_ATTRIBUTE (MAC_NUM_ACKS,macNumAcks);
//	LOAD_PIB_ATTRIBUTE (MAC_RESPONSE_WAIT_TIME,macResponseWaitTime);
//	LOAD_PIB_ATTRIBUTE (MAC_RX_ON_WHEN_IDLE,macRxOnWhenIdle);
//	LOAD_PIB_ATTRIBUTE (MAC_SHORT_ADDRESS,macShortAddress);
//	LOAD_PIB_ATTRIBUTE (MAC_SUPERFRAME_ORDER,macSuperframeOrder);
//	LOAD_PIB_ATTRIBUTE (MAC_TIME_STAMP_OFFSET,macTimeStampOffset);
//	LOAD_PIB_ATTRIBUTE (MAC_TIMESTAMP_SUPPORTED,macTimestampSupported);
//	LOAD_PIB_ATTRIBUTE (MAC_TRANSACTION_PERSISTENCE_TIME,macTransactionPersistenceTime);
//	LOAD_PIB_ATTRIBUTE (MAC_VPAN_ID,macVPANId);

}

LifiMacPibAttribute::~LifiMacPibAttribute() {
}

void LifiMacPibAttribute::SetAttributes(MacPIBAttributeId id, void* value) {
	{
		std::map<uint8_t, void*>::iterator it = m_attributes.find((uint8_t)id);
		NS_ASSERT (it != m_attributes.end());
		TRY_SET_PIB_ATTRIBUTES(MAC_ACK_WAIT_DURATION,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_ASSOC_PERMIT,bool);
		TRY_SET_PIB_ATTRIBUTES(MAC_ASSOC_VPAN_COORD,bool);
		TRY_SET_PIB_ATTRIBUTES(MAC_AUTO_REQEST,bool);
		TRY_SET_PIB_ATTRIBUTES(MAC_BEACON_ORDER,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_BEACON_PAYLOAD,Buffer);
		TRY_SET_PIB_ATTRIBUTES(MAC_BEACON_PAYLOAD_LENGTH,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_BEACON_TX_TIME,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_BSN,uint8_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_COORD_EXTENDED_ADDRESS,Mac64Address);
		TRY_SET_PIB_ATTRIBUTES(MAC_COORD_SHORT_ADDRESS,Mac16Address);
		TRY_SET_PIB_ATTRIBUTES(MAC_DSN,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_GTS_PERMIT,bool);
		TRY_SET_PIB_ATTRIBUTES(MAC_LINK_TIME_OUT,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_MAX_BE,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_MAX_CSMA_BACKOFFS,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_MAX_FRAME_RETRIES,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_MAX_FRAME_TOTAL_WAIT_TIME,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_MIN_BE,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_MIN_LIFS_PERIOD,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_MIN_SIFS_PERIOD,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_NUM_ACKS,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_RESPONSE_WAIT_TIME,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_RX_ON_WHEN_IDLE,bool);
		TRY_SET_PIB_ATTRIBUTES(MAC_SHORT_ADDRESS,Mac16Address);
		TRY_SET_PIB_ATTRIBUTES(MAC_SUPERFRAME_ORDER,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_TIME_STAMP_OFFSET,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_TIMESTAMP_SUPPORTED,bool);
		TRY_SET_PIB_ATTRIBUTES(MAC_TRANSACTION_PERSISTENCE_TIME,uint32_t);
		TRY_SET_PIB_ATTRIBUTES(MAC_VPAN_ID,uint16_t);
	}
}

void LifiMacPibAttribute::Reset() {
}

} /* namespace ns3 */


