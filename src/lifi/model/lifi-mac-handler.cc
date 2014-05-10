/*
 * lifi-mac-handler.cc
 *
 *  Created on: 2014年4月24日
 *      Author: will
 */

#include "lifi-mac-handler.h"
#include "ns3/log.h"
#include "lifi-assoc-handler.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("LifiMacHandler");

LifiMacHandler::LifiMacHandler()
{
}

LifiMacHandler::~LifiMacHandler()
{
}

TypeId LifiMacHandler::GetTypeId()
{
	static TypeId tid = TypeId ("ns3::LifiMacHandler")
						.AddConstructor<LifiMacHandler> ()
						.SetParent<Object> ();
	return tid;
}

void LifiMacHandler::SetMacPibAttributes(LifiMacPibAttribute* attrubutes)
{
	m_attributes = attrubutes;
}

} /* namespace ns3 */


