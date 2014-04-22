/*
 * phy-op-status.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef PHY_OP_STATUS_H_
#define PHY_OP_STATUS_H_

namespace ns3 {

enum PhyOpStatus {
	CHANNEL_BUSY,
	TX_BUSY,
	RX_BUSY,
	FORCE_TRX_OFF,
	IDLE,
	PHY_INVALID_PARAMETER,
	RX_ON,
	PHY_SUCCESS,
	TRX_OFF,
	TX_ON,
	UNSUPPORTED_ATTRIBUTE,
	FAIL,
};

enum PhyPIBAttributeId
{
	PHY_CURRENT_CHANNEL = 0X00,
	PHY_CCA_MODE = 0X01,
	PHY_DIM = 0X02,
	PHY_USE_EXTENDE_MODE = 0X03,
	PHY_COLOR_FUNCTION = 0X04,
	PHY_BLINK_NOTIFICATION_FREQUENCY = 0X05
};

enum CcaMode
{
	MODE_1 = 0x01,
	MODE_2 = 0x02,
	MODE_3 = 0x04,
};

enum TRxMode
{
	MULTIPLEX,
	DIVERSITY
};

} /* namespace ns3 */

#endif /* PHY_OP_STATUS_H_ */
