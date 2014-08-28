/*
 * lifi-mac-comm.h
 *
 *  Created on: 2014年3月31日
 *      Author: will
 */

#ifndef LIFI_MAC_COMM_H_
#define LIFI_MAC_COMM_H_

#include "ns3/core-module.h"
#include "lifi-mac-general.h"

namespace ns3 {

struct LifiMacComm
{

public:
	LifiMacComm();
	virtual ~LifiMacComm();

	Ptr<Packet> GetPacket();

	uint8_t GetCommId () const;

	void SetCommId(uint8_t commid);
protected:
	CommId m_commId;

	virtual void Deserialize(const uint8_t *data, uint32_t size) = 0;
	virtual Buffer Serialize() = 0;

};

struct AssocRqstComm : public LifiMacComm
{

public:
	AssocRqstComm ();

	AssocRqstComm (CapabilityInfo &info);

	virtual ~AssocRqstComm();

	static AssocRqstComm& Construct (Ptr<Packet> p);

	bool GetPowerSource () const;

	void SetPowerSource(bool powersour);


	uint8_t GetCapInfo_BatInfo () const;

	void SetCapInfo_BatInfo(uint8_t batinfo);


	bool GetSecCapabi () const;

	void SetSecCapabi(bool seccapab);


	bool GetCoordCapab () const;

	void SetCoordCapab(bool coordcapa);


	bool GetTrafficSup() const;

	void SetTrafficSup(bool traffsup);

private:
	AssocRqstComm(Ptr<Packet> p);
	virtual void Deserialize(const uint8_t *data, uint32_t size);
	virtual Buffer Serialize();

	CapabilityInfo m_capabilityInfo;
};

struct GtsResponseComm : public LifiMacComm
{

public:
	GtsResponseComm ();

	GtsResponseComm (GTSCharacteristics c, uint8_t slot);

	virtual ~GtsResponseComm();

	static GtsResponseComm& Construct (Ptr<Packet> p);


	uint8_t GetgtsLenth () const;

	void SetgtsLenth(uint8_t gtslenth);


	bool GetDirection () const;

	void SetgtsDirection(bool gtsdirection);


	bool GetgtsCharType () const;

	void SetgtsCharType(bool gtschartype);


	uint8_t Getgtsstartslot () const;

	void Setgtsstartslot(uint8_t startslot);


private:
	GtsResponseComm (Ptr<Packet> p);
	virtual void Deserialize(const uint8_t *data, uint32_t size);
	virtual Buffer Serialize();

	GTSCharacteristics m_characteristic;
	uint8_t m_startingSlot;
};

struct DisassocNotificationComm : public LifiMacComm
{

public:

	DisassocNotificationComm();

	virtual ~DisassocNotificationComm();

	static DisassocNotificationComm& Construct (Ptr<Packet> p);


	DisassocReason GetDissassocReason () const;

	void SetDissassocReason(DisassocReason disassreason);


private:
	DisassocNotificationComm(Ptr<Packet> p);
	virtual void Deserialize(const uint8_t *data, uint32_t size);
	virtual Buffer Serialize();

	DisassocReason reason;
};

struct BeaconRequestComm : public LifiMacComm
{

public:
	BeaconRequestComm ();

	virtual ~BeaconRequestComm ();

	static BeaconRequestComm& Construct (Ptr<Packet> p);

private:
	BeaconRequestComm (Ptr<Packet> p);
	virtual void Deserialize(const uint8_t *data, uint32_t size);
	virtual Buffer Serialize();

};

struct MultiChannelAssignComm : public LifiMacComm
{

public:
	MultiChannelAssignComm ();

	MultiChannelAssignComm (uint8_t multiChannel);

	virtual ~MultiChannelAssignComm();

	static MultiChannelAssignComm& Construct (Ptr<Packet> p);

	/**
	 * This is a bit map to indicate the available channels.
	 * ex: 0000000: No multiple channel mode
	 * ex: 0000001: using channel “Band 7”
	 * ex: 0000101: using channel “Band 5” and “Band 7”
	 */

	uint8_t GetMultiChannel () const;

	void SetMultiChannel(uint8_t multichannel);


private:
	MultiChannelAssignComm (Ptr<Packet> p);
	virtual void Deserialize(const uint8_t *data, uint32_t size);
	virtual Buffer Serialize();

	uint8_t m_multiChannel;
};

struct InfoElementComm : public LifiMacComm
{

public:
	InfoElementComm ();

	InfoElementComm (uint8_t id, uint8_t lenth, uint8_t *payload);

	virtual ~InfoElementComm();

	static InfoElementComm& Construct (Ptr<Packet> p);


	uint8_t GetElementId () const;

	void SetElementId(uint8_t elementid);

	uint8_t GetElementLenth () const;

	void SetElementLenth(uint8_t lenth);

private:
	InfoElementComm (Ptr<Packet> p);
	virtual void Deserialize(const uint8_t *data, uint32_t size);
	virtual Buffer Serialize();


	ElementId m_elementId;
	uint8_t m_lenth;
	uint8_t m_payload[256];

};

struct GtsRequestComm : public LifiMacComm
{

public:
	GtsRequestComm ();
	GtsRequestComm (GTSCharacteristics c);

	virtual ~GtsRequestComm ();

	static GtsRequestComm& Construct (Ptr<Packet> p);

	uint8_t GetgtsLenth () const;

	void SetgtsLenth(uint8_t gtslenth);


	bool GetDirection () const;

	void SetgtsDirection(bool gtsdirection);


	bool GetgtsCharType () const;

	void SetgtsCharType(bool gtschartype);


private:
	GtsRequestComm (Ptr<Packet> p);
	virtual void Deserialize(const uint8_t *data, uint32_t size);
	virtual Buffer Serialize();

	GTSCharacteristics m_characteristic;
};

struct AssocResponseComm : public LifiMacComm
{

public:

	AssocResponseComm();

	AssocResponseComm(MacOpStatus assoc, uint8_t cap, uint16_t address);

	virtual ~AssocResponseComm();

	static AssocResponseComm& Construct (Ptr<Packet> p);


    uint16_t GetShortAddr () const;

    void SetShortAddr(Mac16Address shortaddr);

    MacOpStatus GetAssocStatus () const;

    void SetAssocStatus(MacOpStatus assocstatus);

    uint8_t GetCapNegoResponse () const;

    void SetCapNegoResponse(uint8_t capnegores);


private:
	AssocResponseComm(Ptr<Packet> p);
	virtual void Deserialize(const uint8_t *data, uint32_t size);
	virtual Buffer Serialize();

	uint16_t m_shortAddr;
	MacOpStatus m_assocStatus;
	uint8_t m_capabilityNegoResponse;

};

struct MobilityNotifiComm : public LifiMacComm
{

public:
	MobilityNotifiComm();
	virtual ~MobilityNotifiComm();

	static MobilityNotifiComm& Construct (Ptr<Packet> p);

private:
	MobilityNotifiComm(Ptr<Packet> p);
	virtual void Deserialize(const uint8_t *data, uint32_t size);
	virtual Buffer Serialize();

};

struct DataRequestComm : public LifiMacComm
{

public:
	DataRequestComm();
	virtual ~DataRequestComm();

	static DataRequestComm& Construct (Ptr<Packet> p);

private:
	DataRequestComm(Ptr<Packet> p);
	void Deserialize(const uint8_t *data, uint32_t size);
	Buffer Serialize();

};

} /* namespace ns3 */

#endif /* LIFI_MAC_COMM_H_ */
