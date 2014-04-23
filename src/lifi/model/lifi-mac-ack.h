/*
 * lifi-mac-ack.h
 *
 *  Created on: 2014年4月20日
 *      Author: wanghongqiao
 */

#ifndef LIFI_MAC_ACK_H_
#define LIFI_MAC_ACK_H_

#include "ns3/core-module.h"
#include "lifi-mac-general.h"

namespace ns3 {

class LifiMacAck
{
public:
	LifiMacAck();
	virtual ~LifiMacAck();

	Ptr<Packet> GetPacket();

	static LifiMacAck& Construct (Ptr<Packet> p);

private:
	LifiMacAck(Ptr<Packet> p);
	void Deserialize(const uint8_t *data, uint32_t size);
	Buffer Serialize();

	uint16_t m_BufferSize;
	uint8_t m_FrameCount;
	uint16_t m_SequenceControl;
	uint8_t m_bitmap[512];

};

}/* namespace ns3 */

#endif /* LIFI_MAC_COMM_H_ */
