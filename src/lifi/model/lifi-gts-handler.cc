/*
 * lifi-gts-handler.cc
 *
 *  Created on: 2014年3月31日
 *      Author: will
 */
#include "ns3/log.h"
#include "lifi-gts-handler.h"

namespace ns3 {

LifiGtsHandler::LifiGtsHandler() {
}

LifiGtsHandler::~LifiGtsHandler() {
}

TypeId LifiGtsHandler::GetTypeId()
{
	TypeId tid = TypeId ("ns3::LifiGtsHandler")
			.SetParent<Object> ()
			.AddConstructor<LifiGtsHandler> ();
	return tid;
}

bool LifiGtsHandler::GetGtsPermission()
{
	return false;
}

uint8_t LifiGtsHandler::GetGtsCount()
{
	return 0;
}

uint8_t LifiGtsHandler::GetGtsDirMask()
{
	return 0x00;
}

GtsDescriptors LifiGtsHandler::GetGtsDescritors() const
{
	return m_descriptors;
}

void LifiGtsHandler::ReportTransmission(MacOpStatus status)
{
}

} /* namespace ns3 */
