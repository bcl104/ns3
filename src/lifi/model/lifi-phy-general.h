/*
 * phy-op-status.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef PHY_OP_STATUS_H_
#define PHY_OP_STATUS_H_
#include "ns3/core-module.h"
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
	DEFAULT,
};

enum PhyPIBAttributeId
{
	PHY_CURRENT_CHANNEL = 0X00,
	PHY_CCA_MODE = 0X01,
	PHY_DIM = 0X02,
	PHY_USE_EXTENDE_MODE = 0X03,
	PHY_COLOR_FUNCTION = 0X04,
	PHY_BLINK_NOTIFICATION_FREQUENCY = 0X05,
	PHY_MCSID = 0X06
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
/*
 * unit:kb/s
 */
//double GetRate(uint8_t mcsId){
////	NS_LOG_FUNCTION(this);
//		switch(mcsId){
//		/*PHY I*/
//		case 0:
//			return 11.67;break;
//		case 1:
//			return 22.44;break;
//		case 2:
//			return 48.89;break;
//		case 3:
//			return 73.3;break;
//		case 4:
//			return 100;break;
//		case 5:
//			return 35.56;break;
//		case 6:
//			return 71.11;break;
//		case 7:
//			return 124.4;break;
//		case 8:
//			return 266.6;break;
//		/*PHY II*/
//		case 16:
//			return 1250;break;
//		case 17:
//			return 2000;break;
//		case 18:
//			return 2500;break;
//		case 19:
//			return 4000;break;
//		case 20:
//			return 5000;break;
//		case 21:
//			return 6000;break;
//		case 22:
//			return 9600;break;
//		case 23:
//			return 12000;break;
//		case 24:
//			return 18000;break;
//		case 25:
//			return 24000;break;
//		case 26:
//			return 38400;break;
//		case 27:
//			return 48000;break;
//		case 28:
//			return 76800;break;
//		case 29:
//			return 96000;break;
//		/*PHY III*/
//		case 32:
//			return 12000;break;
//		case 33:
//			return 18000;break;
//		case 34:
//			return 24000;break;
//		case 35:
//			return 36000;break;
//		case 36:
//			return 48000;break;
//		case 37:
//			return 72000;break;
//		default:
//			return 96000;break;
//		}
//		return -1;
//}

} /* namespace ns3 */

#endif /* PHY_OP_STATUS_H_ */
