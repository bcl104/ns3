/*
 * lifi-mac-ack.cc
 *
 *  Created on: 2014年4月20日
 *      Author: wanghongqiao
 */

#include "lifi-mac-ack.h"
#include "ns3/log.h"

namespace ns3 {

LifiMacAck::LifiMacAck() {

}

LifiMacAck::~LifiMacAck() {
}


LifiMacAck::LifiMacAck(Ptr<Packet> p){
	uint32_t size = p->GetSize();
	uint8_t *data = (uint8_t*) malloc (size);
	p->CopyData(data, size);
	Deserialize(data, size);
	free (data);
}

Ptr<Packet> LifiMacAck::GetPacket(){
	Buffer b = Serialize();
	Ptr<Packet> p = Create<Packet> (b.PeekData(), b.GetSize());
	return p;
}

LifiMacAck& LifiMacAck::Construct(Ptr<Packet> p) {
	static LifiMacAck ack;
	memset(&ack, 0, sizeof (LifiMacAck));
	ack = LifiMacAck (p);
	return ack;
}

Buffer LifiMacAck::Serialize() {
//	uint32_t bufferSizeSize = 2;
//	uint32_t frameCountSize = 1;
//	uint32_t sequenseControlSize = 2;
//	uint32_t bitmapSize = 512;
//	uint32_t size = bufferSizeSize + frameCountSize +
//			        sequenseControlSize + bitmapSize + 1;
//	Buffer data;
//	data.AddAtStart(size);
//	Buffer::Iterator it = data.Begin();
//	it.WriteU16(m_BufferSize);
//	it.WriteU8(m_FrameCount);
//	uint8_t reseved = 1;
//	it.WriteU8(reseved);
//	it.WriteU16(m_SequenceControl);
//	it.Write(m_bitmap, 512);     //ACK is so long that ACKWaitTime is timeout!!
	Buffer data;
	uint32_t size = 1;
	data.AddAtStart(size);
	return data;
}

void LifiMacAck::Deserialize(const uint8_t *data, uint32_t size) {
//	m_BufferSize =*data++;
//	m_BufferSize =(m_BufferSize <<8) | *data;
//	data ++;
//	m_FrameCount = *data;
//	data += 2;
//	m_SequenceControl = *data++;
//	m_SequenceControl = (m_SequenceControl << 8) | *data;
//	data ++;
//	memcpy(m_bitmap, data, 512);

}


} /* namespace ns3 */
