/*
 * ifi-tx-handler.cc
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#include "lifi-tx-handler.h"

namespace ns3 {

LifiTxHandler::LifiTxHandler() {
	// TODO Auto-generated constructor stub

}

LifiTxHandler::~LifiTxHandler() {
	// TODO Auto-generated destructor stub
}

void LifiTxHandler::Confirm(PhyOpStatus status) {
}

LifiTxHandler::State LifiTxHandler::GetStatus() {
	return m_state;
}

void LifiTxHandler::Reset() {
}

void LifiTxHandler::Run() {
}

void LifiTxHandler::Send(uint32_t size, Ptr<PacketBurst> pb, uint8_t band,
		bool contention) {
}

void LifiTxHandler::SetCallback(OpStatusCallback* cb) {
}

bool LifiTxHandler::ClearChannelAccessment() {
	return false;
}

void LifiTxHandler::DoTransmitData(uint32_t size, Ptr<PacketBurst> pb,
		uint8_t band) {
}

} /* namespace ns3 */
