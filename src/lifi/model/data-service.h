/*
 * data-service.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef DATA_SERVICE_H_
#define DATA_SERVICE_H_

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "op-status-callback.h"
#include "lifi-mac-general.h"

namespace ns3 {

template <class T> class DataServiceImpl;

class DataServiceImplBase
{
public:
	DataServiceImplBase () {};
	virtual ~DataServiceImplBase () {};
	virtual bool Transmit(PacketInfo& info) = 0;
	virtual void Receceive (bool enable) = 0;
//	virtual PhyOpStatus SetTrxState (PhyOpStatus s) = 0;
	virtual void Suspend () = 0;
	virtual void Release () = 0;
};

class DataService : public SimpleRefCount<DataService>
{
	friend class TrxHandlerListener;

public:

	DataService ();

	virtual ~DataService();

	template <class T> static Ptr<DataService> Create (T t)
	{
		return Create<DataService> (t);
	}

	bool Transmit(PacketInfo& info)
	{
		NS_ASSERT (m_connected);
		return m_base->Transmit(info);
	}

	void Receceive (bool enable)
	{
		NS_ASSERT (m_connected);
		m_base->Receceive(enable);
	}

//	PhyOpStatus SetTrxState (PhyOpStatus s)
//	{
//		NS_ASSERT (m_connected);
//		return m_base->SetTrxState(s);
//	}

	void Suspend ()
	{
		NS_ASSERT (m_connected);
		m_base->Suspend();
		Disconnect();
	}

	void Release ()
	{
		NS_ASSERT (m_connected);
		m_base->Release();
		Disconnect();
	}
private:
	template<class T> DataService(T *impl)
	{
		m_base = new DataServiceImpl<T> (impl);
		m_connected = true;
	}

	void Disconnect ()
	{
		m_connected = false;
	}

	bool m_connected;
	DataServiceImplBase *m_base;
};

template <class T>
class DataServiceImpl : public DataServiceImplBase
{
public:
	DataServiceImpl ()
	{
	}
	DataServiceImpl (T *impl) : m_impl(impl)
	{
	}

	virtual ~DataServiceImpl () { if (m_impl != 0) free (m_impl); };

	virtual bool Transmit(PacketInfo& info)
	{
		return m_impl->Transmit (info);
	}

	virtual void Receceive (bool enable)
	{
		m_impl->Receceive (enable);
	}

//	virtual PhyOpStatus SetTrxState (PhyOpStatus s)
//	{
//		return m_impl->SetTrxState(s);
//	}

	virtual void Suspend ()
	{
		m_impl->Suspend ();
	}

	virtual void Release ()
	{
		m_impl->Release ();
	}
private:
	T *m_impl;
};

class TrxHandlerListener
{
public:
	virtual void AllocNotification (Ptr<DataService> service) {};
	virtual void TxResultNotification (MacOpStatus status, PacketInfo info, Ptr<Packet> ack) {};
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


} /* namespace ns3 */

#endif /* DATA_SERVICE_H_ */
