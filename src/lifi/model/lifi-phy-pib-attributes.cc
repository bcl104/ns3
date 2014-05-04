/*
 * lifi-phy-pib-attributes.cc
 *
 *  Created on: 2014年4月3日
 *      Author: will
 */

#include "lifi-phy-pib-attributes.h"

namespace ns3 {

LifiPhyPibAttribute::LifiPhyPibAttribute() {
	LOAD_PIB_ATTRIBUTE (PHY_CURRENT_CHANNEL,phyCurrentChannel,CHANNEL1);
	LOAD_PIB_ATTRIBUTE (PHY_CCA_MODE,phyCCAMode,MODE_1);
	LOAD_PIB_ATTRIBUTE (PHY_DIM,phyDim,10);
	LOAD_PIB_ATTRIBUTE (PHY_USE_EXTENDE_MODE,phyUseExtendedMode,true);
//	LOAD_PIB_ATTRIBUTE (PHY_COLOR_FUNCTION,phyColorFunction);
	LOAD_PIB_ATTRIBUTE (PHY_BLINK_NOTIFICATION_FREQUENCY,phyBlinkingNotificationFrequency,0);
	LOAD_PIB_ATTRIBUTE (PHY_MCSID,m_mcsid,0);

}

LifiPhyPibAttribute::~LifiPhyPibAttribute() {
	// TODO Auto-generated destructor stub
}

void LifiPhyPibAttribute::SetAttributes(PhyPIBAttributeId id, void* value) {

	std::map<uint8_t, void*>::iterator it = m_attributes.find((uint8_t)id);
	NS_ASSERT (it != m_attributes.end());
	TRY_SET_PIB_ATTRIBUTES(PHY_CURRENT_CHANNEL,LogicChannelId);
	TRY_SET_PIB_ATTRIBUTES(PHY_CCA_MODE,CcaMode);
	TRY_SET_PIB_ATTRIBUTES(PHY_BLINK_NOTIFICATION_FREQUENCY,uint32_t);
	TRY_SET_PIB_ATTRIBUTES(PHY_USE_EXTENDE_MODE,bool);
	TRY_SET_PIB_ATTRIBUTES(PHY_DIM,uint32_t);

}

} /* namespace ns3 */

