/*
 * op-status-callback.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef OP_STATUS_CALLBACK_H_
#define OP_STATUS_CALLBACK_H_

#include "lifi-mac-general.h"
#include "lifi-phy-general.h"

namespace ns3 {

class OpStatusCallback
{

public:
	OpStatusCallback();

	virtual ~OpStatusCallback();

	virtual void TxResultNotification(PhyOpStatus status) = 0;

};

} /* namespace ns3 */

#endif /* OP_STATUS_CALLBACK_H_ */
