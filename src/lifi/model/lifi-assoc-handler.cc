/*
 * lifi-assoc-handler.cc
 *
 *  Created on: 2014年4月25日
 *      Author: will
 */

#include "lifi-assoc-handler.h"

namespace ns3
{

LifiAssocHandler::LifiAssocHandler()
{

}

LifiAssocHandler::~LifiAssocHandler()
{
}

TypeId LifiAssocHandler::GetTypeId()
{
	static TypeId tid = TypeId ("ns3::LifiAssocHandler")
						.AddConstructor<LifiAssocHandler> ()
						.SetParent<LifiMacHandler> ();
	return tid;
}

void LifiAssocHandler::TxResultNotification(MacOpStatus status)
{
}

} /* namespace ns3 */
