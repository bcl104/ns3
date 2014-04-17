/*
 * lifi-asso-chandler.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef LIFI_ASSOC_HANDLER_H_
#define LIFI_ASSOC_HANDLER_H_
#include "ns3/object.h"
#include "lifi-mac-general.h"
#include "op-status-callback.h"
#include "data-service.h"
#include "lifi-mac-impl.h"

namespace ns3 {

class LifiAssocHandler : public Object, public OpStatusCallback
{

public:
	LifiAssocHandler();
	virtual ~LifiAssocHandler();

	static TypeId GetTypeId ();

	virtual void TxResultNotification(PhyOpStatus status);

protected:
	DataService* m_dataService;
	LifiMacImpl* m_impl;

private:
	TypeId m_addrMode;
	Address m_devAddr;
	MacOpStatus m_reson;
	bool m_txIndirect;
	uint8_t m_VPANId;

};

} /* namespace ns3 */

#endif /* LIFI_ASSO_CHANDLER_H_ */
