/*
// * lifi-tx-handler.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef LIFI_TX_HANDLER_H_
#define LIFI_TX_HANDLER_H_

#include "data-service.h"
#include "lifi-mac-general.h"
#include "lifi-phy-general.h"
#include "op-status-callback.h"
#include "lifi-mac-impl.h"

namespace ns3 {

//class LifiMacImpl;

class LifiTxHandler : public Object, public DataService
{

public:
	enum State
	{
		TX,
		BACKOFF,
		IDLE
	};

	LifiTxHandler();
	virtual ~LifiTxHandler();

	virtual void Confirm(MacOpStatus status);
	LifiTxHandler::State GetStatus();
	void Reset();
	virtual void Run();
	virtual void Send(uint32_t size, Ptr<PacketBurst> pb, uint8_t band, bool contention);
	void SetCallback(OpStatusCallback* cb);
	OpStatusCallback* m_opStatusCallback;

protected:
	OpStatusCallback* m_result;

	virtual bool ClearChannelAccessment();
	virtual void DoTransmitData(uint32_t size, Ptr<PacketBurst> pb, uint8_t band);

private:
	Timer m_backoffTimer;
	LifiMacImpl* m_impl;
	LifiTxHandler::State m_state;

};

} /* namespace ns3 */

#endif /* LIFI_TX_HANDLER_H_ */
