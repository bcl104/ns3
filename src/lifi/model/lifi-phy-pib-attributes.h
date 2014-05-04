/*
 * lifi-phy-pib-attributes.h
 *
 *  Created on: 2014年4月3日
 *      Author: will
 */

#ifndef LIFI_PHY_PIB_ATTRIBUTES_H_
#define LIFI_PHY_PIB_ATTRIBUTES_H_
#include <map>
#include "ns3/core-module.h"
#include "lifi-phy-general.h"
#include "lifi-mac-general.h"
#include "lifi-pib-attributes.h"


namespace ns3 {

class LifiPhyPibAttribute : public LifiPIBAttribute
{
	friend class LifiPhy;
public:
	LifiPhyPibAttribute ();

	virtual ~LifiPhyPibAttribute ();

	void* GetAttributes (PhyPIBAttributeId id)
	{
		std::map<uint8_t, void*>::iterator it = m_attributes.find((uint8_t)id);
		NS_ASSERT (it != m_attributes.end());
		return it->second;
	}

	void SetAttributes (PhyPIBAttributeId id, void* value);

private:

	/**
	 * 0-10
	 */
	uint32_t phyBlinkingNotificationFrequency;

	CcaMode phyCCAMode;

	/**
	 * A table with three columns per row. The first row is the index, the second and
	 * the third columns define the color.
	 */
//	int** phyColorFunction;

	/**
	 * 00000000, each bit represent if the corresponding channel is used.
	 */

	LogicChannelId phyCurrentChannel;

	uint32_t phyDim;

	/**
	 * 0-1,This attribute is set to a one to indicate that an extended preamble or
	 * visibility pattern is to be used. Otherwise, it is set to zero.
	 */
	bool phyUseExtendedMode;

	uint8_t m_mcsid;

};

} /* namespace ns3 */

#endif /* LIFI_PHY_PIB_ATTRIBUTES_H_ */
