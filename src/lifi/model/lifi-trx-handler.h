/*
// * lifi-trx-handler.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef LIFI_TRX_HANDLER_H_
#define LIFI_TRX_HANDLER_H_

#include "data-service.h"
#include "lifi-mac-general.h"
#include "lifi-phy-general.h"
#include "op-status-callback.h"
#include "lifi-mac-impl.h"
#include "lifi-mac-handler.h"
#include "lifi-mac-ack.h"

namespace ns3 {

//class LifiMacImpl;
class TrxHandlerListener
{
public:
	virtual ~TrxHandlerListener () {};
	virtual void AllocNotification (Ptr<DataService> service) = 0;
	virtual void TxResultNotification (MacOpStatus status, Ptr<Packet> ack) = 0;
	virtual void ResumeNotification (Ptr<DataService> service) {};
	virtual void ReceiveAssocRequest (uint32_t timestamp, Ptr<Packet> p) {};
	virtual void ReceiveAssocResponse (uint32_t timestamp, Ptr<Packet> p) {};
	virtual void ReceiveBeacon (uint32_t timestamp, Ptr<Packet> p) {};
	virtual void ReceiveBeaconRequest (uint32_t timestamp, Ptr<Packet> p) {};
	virtual void ReceiveData (uint32_t timestamp, Ptr<Packet> p) {};
	virtual void ReceiveDataRequest (uint32_t timestamp, Ptr<Packet> p) {};
	virtual void ReceiveDisassocNotification (uint32_t timestamp, Ptr<Packet> p) {};
	virtual void ReceiveGtsRequest (uint32_t timestamp, Ptr<Packet> p) {};
	virtual void ReceiveGtsResponse (uint32_t timestamp, Ptr<Packet> p) {};
};

struct TranceiverTask
{
	TranceiverTask () { priority = 0; listener = 0; id = TX_DATA; }
	void Clear () { priority = 0; listener = 0; }
	bool Available () { return ((priority > 0) && (listener != 0)); }
	uint16_t priority;
	TranceiverTaskId id;
	TrxHandlerListener *listener;
};

typedef std::map<uint16_t, TrxHandlerListener&> TrxHandlerListeners;
typedef std::priority_queue<TranceiverTask> TrxTasks;

bool operator< (TranceiverTask t1, TranceiverTask t2) { return (t1.priority > t2.priority); }

class LifiTrxHandler : public LifiMacHandler
{
	friend class DataServiceImpl<LifiTrxHandler>;

public:

	typedef std::queue<PacketInfo> DataBuffer;

	LifiTrxHandler();

	virtual ~LifiTrxHandler();

	void AddListener (TypeId id, TrxHandlerListener &listener);

	void RemoveListener (TypeId id, TrxHandlerListener &listener);

	void ServiceRequest (TranceiverTask task);

	void Send (PacketInfo& p);

	void SetLifiMacImpl (LifiMacImpl* impl);

	LifiMacImpl* GetLifiMacImpl () const;

	void SetPlmeSapProvider (PlmeSapProvider* provider);

	PlmeSapProvider* GetPlmeSapProvider () const;

	void SetPdSapProvider (PdSapProvider* provider);

	PdSapProvider* GetPdSapProvider () const;

	void SetOpticalPeriod (const Time* oc);

	const Time* GetOpticalPeriod () const;

	void Reset();

	// External trigger.
	void TxConfirm(PhyOpStatus status);
	void ReceivePacket (uint32_t timestamp, Ptr<Packet> p);



protected:
	// Interface for upper layer handler.
	bool Transmit(PacketInfo& info);
	void Receceive (bool enable);
	void Release ();
	void Suspend ();

	// Internal method for task scheduling.
	void Fetch ();
	void ClearCurTask ();

	template<class T> void Broadcast (T function, uint32_t timestamp, Ptr<Packet> p)
	{
		TrxHandlerListeners::iterator it = m_listens.begin();
		while (it != m_listens.end())
		{
			(it->second.*function)(timestamp, p);
			it++;
		}
	}

	// External trigger implementation.
	virtual void onTxConfirm (PhyOpStatus status);
	virtual void onReceivePacket (uint32_t timestamp, Ptr<Packet> p);
	virtual void onReceiveAck (uint32_t timestamp, Ptr<Packet> ack);

	// Method for random access.
	void StartRandomAccess (PacketInfo& info);
	bool ClearChannelAccessment();
	void onBackoffTimeout ();
	void Backoff ();

	// Method for GTS transmission.
	void StartGtsTransmit (PacketInfo& task);

	virtual void AcknowledgmentTimeout ();
	virtual bool RetransmitData ();

	virtual void EndTransmission (MacOpStatus status, Ptr<Packet> ack);
	bool DoTransmitData();

	// Method for superframe structure.
	void BuildSuperframeStruct (Ptr<Packet> beacon);
	virtual void SuperframeStart ();
	virtual void ContentionAccessPeriodEnd ();
	virtual void ContentionFreePeriodStart ();
	virtual void ContentionFreePeriodEnd ();
	virtual void InactionPortionStart ();
	virtual void SuperframeEnd ();

	struct TransmissionInfo
	{
		TransmissionInfo ();
		TransmissionInfo(uint32_t* maxRt);
		uint32_t *maxPacketRetry;
		PacketInfo m_info;
		uint8_t m_sequenceNum;
		uint32_t m_packetRetry;
		LifiBackoff m_backoff;
		Time* m_opticalPeriod;
		Timer m_ackTimer;
		void IncrePacketRetry ();
		bool IsReachMaxRetry ();
		bool IsAvailable ();
		void Reset ();
	} m_curTransmission, m_suspendedTransmission;

	struct SuperframeStrcut
	{
		SuperframeStrcut ();
		bool m_contentionFreePeriod;
		bool m_inactivePortion;
		Timer m_capEnd;
		Timer m_cfpEnd;
		Timer m_supframeEnd;
		enum {
			CAP, CFP, INACTIVE, DEFAULT
		} m_state;
	} m_superframeStruct;

	enum {
		IDLE,
		TRANCEIVER_TASK,
		GTS_TRANSMISSION,
		NORMAL_DATA,
	} m_opStatus;

	TrxHandlerListeners m_listens;
	TrxTasks m_tranceiverTasks;
	DataBuffer m_raTasks;
	DataBuffer m_gtsTasks;

	TranceiverTask m_curTranceiverTask;

};

#define Broadcast(function, timestamp, p) Broadcast(&function, timestamp, p)

} /* namespace ns3 */

#endif /* LIFI_TRX_HANDLER_H_ */
