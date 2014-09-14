/*
 * lifi-general.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef LIFI_GENERAL_H_
#define LIFI_GENERAL_H_
#include "ns3/core-module.h"
#include "ns3/network-module.h"

#define MAC_SCAN_VPANID 0xffff
#define MAX_PENDING_ADDRESS 7
#define MAX_GTS_DESCRIPTOR 7
#define MAX_BEACON_PER_CHANNEL 5
#define MAX_SCAN_DURATION 14

namespace ns3 {

typedef std::vector<Address> AddrList;

enum MacPIBAttributeId
{
	MAC_ACK_WAIT_DURATION = 0x40,
	MAC_ASSOC_VPAN_COORD = 0x41,
	MAC_ASSOC_PERMIT = 0x42,
	MAC_AUTO_REQEST = 0x43,
	MAC_BEACON_PAYLOAD = 0x44,
	MAC_BEACON_PAYLOAD_LENGTH = 0x45,
	MAC_BEACON_ORDER = 0x46,
	MAC_BEACON_TX_TIME = 0x47,
	MAC_BSN = 0x48,
	MAC_COORD_EXTENDED_ADDRESS = 0x49,
	MAC_COORD_SHORT_ADDRESS = 0x4a,
	MAC_DSN = 0x4b,
	MAC_GTS_PERMIT = 0x4c,
	MAC_MAX_BE = 0x4d,
	MAC_MAX_CSMA_BACKOFFS = 0x4e,
	MAC_MAX_FRAME_TOTAL_WAIT_TIME = 0x4f,
	MAC_MAX_FRAME_RETRIES = 0x50,
	MAC_MIN_BE = 0x51,
	MAC_MIN_LIFS_PERIOD = 0x52,
	MAC_MIN_SIFS_PERIOD = 0x53,
	MAC_VPAN_ID = 0x54,
	MAC_RESPONSE_WAIT_TIME = 0x55,
	MAC_RX_ON_WHEN_IDLE = 0x56,
	MAC_SHORT_ADDRESS = 0x58,
	MAC_SUPERFRAME_ORDER = 0x59,
	MAC_TIMESTAMP_SUPPORTED = 0x5a,
	MAC_TRANSACTION_PERSISTENCE_TIME = 0x5b,
	MAC_NUM_ACKS = 0x5d,
	MAC_LINK_TIME_OUT = 0x5e,
	MAC_TIME_STAMP_OFFSET = 0x6e
};

enum DataRateId
{
	PHY_I_11_67_KBPS = 0x00,
	PHY_I_24_44_KBPS = 0x01,
	PHY_I_48_89_KBPS = 0x02,
	PHY_I_73_30_KBPS = 0x03,
	PHY_I_100_00_KBPS = 0x04,
	PHY_I_35_56_KBPS = 0x05,
	PHY_I_71_11_KBPS = 0x06,
	PHY_I_124_40_KBPS = 0x07,
	PHY_I_266_60_KBPS = 0x08,
	PHY_II_1_25_MBPS = 0x10,
	PHY_II_2_00_MBPS = 0x11,
	PHY_II_2_50_MBPS = 0x12,
	PHY_II_4_00_MBPS = 0x13,
	PHY_II_5_00_MBPS = 0x14,
	PHY_II_6_00_MBPS = 0x15,
	PHY_II_9_60_MBPS = 0x16,
	PHY_II_12_00_MBPS = 0x17,
	PHY_II_19_20_MBPS = 0x18,
	PHY_II_24_00_MBPS = 0x19,
	PHY_II_38_40_MBPS = 0x1A,
	PHY_II_48_00_MBPS = 0x1B,
	PHY_II_76_80_MBPS = 0x1C,
	PHY_II_96_00_MBPS = 0x1D,
	PHY_III_12_00_MBPS = 0x20,
	PHY_III_18_00_MBPS = 0x21,
	PHY_III_24_00_MBPS = 0x22,
	PHY_III_36_00_MBPS = 0x23,
	PHY_III_48_00_MBPS = 0x24,
	PHY_III_72_00_MBPS = 0x25,
	PHY_III_96_00_MBPS = 0x26
};

enum MacOpStatus
{
	MAC_SUCCESS,
	CHANNEL_ACCESS_FAILURE,
	NO_ACK,
	NO_DATA,
	COUNTER_ERROR,
	FRAME_TOO_LONG,
	MAC_INVALID_PARAMETER,
	MAC_INVALID_GTS,
	TRANSACTION_OVERFLOW,
	TRANSACTION_EXPIRED,
	LIMIT_REACHED,
	NO_BEACON,
	SCAN_IN_PROGRESS,
	DENIED,
	NO_SHORT_ADDRESS,
	ON_TIME_TOO_LONG,
	PAST_TIME,
	VPAN_ID_CONFLICT,
	BEACON_LOST,
};

enum MacColorStabCapab{
	NO_COLOR_STABI = 0X00,
	FROM_DEVICE_TO_COORD = 0X01,
	FROM_COORD_TO_DEVICE = 0X02,
	EITHER_DIRECTION = 0X03,
};

enum DisassocReason
{
	COORD = 0x01,
	DEVICE = 0x02,
	CAN_NOT_SUPPORT = 0x03,
};

enum GTSCharType
{
	ALLOCATION = 0x01,
	DEALLOCATION = 0x00
};

enum ScanType
{
	ACTIVE_SCAN,
	INACTIVE_SCAN
};

enum DisassocState
{
	WAIT_FOR_ACK,
	WAIT_FOR_EXTRACT
};

enum GTSDirection
{
	Transmit_GTS = 0x00,
	Receive_GTS = 0x01,
};

enum LogicChannelId
{
	CHANNEL_UNAVAILABLE,
	CHANNEL1 = 0x01,
	CHANNEL2 = 0x02,
	CHANNEL3 = 0x04,
	CHANNEL4 = 0x08,
	CHANNEL5 = 0x10,
	CHANNEL6 = 0x20,
	CHANNEL7 = 0x40
};

enum CommId
{
	ASSOC_REQUEST = 0x01,
	ASSOC_RESPONSE = 0x02,
	DISASSOC_NOTIFICATION = 0x03,
	DATA_REQUEST = 0x04,
	BEACON_REQUEST = 0x06,
	GTS_REQUEST = 0x08,
	MOBILITY_NOTIFICATION = 0x0C,
	GTS_RESPONSE = 0x0D,
	MULTI_CHANNEL_ASSIGNMENT = 0x0F,
	BAND_HOPPING = 0x10,
	INFORMANTION_ELEMENT = 0x14
};

enum ElementId{
	CAPABILITY = 0X01,
	WAVE_QUA_INDI = 0X02,
};

enum AddrMode
{
	SHORT = 0x02,
	EXTENDED = 0x03,
	ERROR = 0xff,
};

enum FrameType{
	LIFI_BEACON = 0x00,
	LIFI_DATA = 0x01,
	LIFI_ACK = 0x02,
	LIFI_COMMAND = 0x03,
	LIFI_CVD = 0x04,
	LIFI_RESERVED = 0x05,
};

enum TranceiverTaskId
{
	TRX_BEACON,
	TX_ACK,
	TX_DATA,
	TX_COMMAND,
	RX_ASSOC_RESPONSE,
	RX_DISASSOC_NOTIFICATION,
};

uint16_t GetTrxTaskPriority (TranceiverTaskId task);

struct SupframeSpec
{

public:
	bool assocPermit;
	uint8_t beaconOrder;
	uint8_t finalCapSlot;
	uint8_t supframeOrder;
	bool vpanCoord;

};

struct VPANDescriptor
{

public:
	Address coordAddr;
	AddrMode coordAddrMode;
	uint16_t coordVPANId;
	bool gtsPermit;
	LogicChannelId logicChannel;
	SupframeSpec supframeSpec;
	uint32_t timestamp;
	bool operator() (VPANDescriptor& d)
	{
		return ((coordAddr == d.coordAddr) && (coordVPANId == d.coordVPANId));
	}

};

struct TxOption
{

public:
	bool ackTx;
	bool gtsTx;
	bool indirectTx;

};

struct DataDescriptor{
public:
	AddrMode DstAddrMode;
	AddrMode SrcAddrMode;
	Address DstAddr;
	uint16_t DstVPANId;
	Ptr<Packet> Msdu;
	uint16_t MsduLenth;
	uint8_t MsduHandle;
	TxOption Options;
	DataRateId Rate;
	bool BurstMode;
};

struct DataIndicaDescriptor{
public:
	AddrMode DstAddrMode;
	AddrMode SrcAddrMode;
	Address DstAddr;
	Address SrcAddr;
	uint16_t DstVPANId;
	uint16_t SrcVPANId;
	Ptr<Packet> Msdu;
	uint16_t MsduLenth;
    uint8_t DSN;
    uint32_t TimeStamp;
};

struct DisassocDescriptor{
	AddrMode DeviceAddrMode;
	Address DeviceAddr;
	uint16_t DeviceVPANId;
	DisassocReason DisassociationReason;
	bool TxIndirect;
};

typedef std::vector<VPANDescriptor> VpanDescriptors;

struct GTSCharacteristics
{

public:
	GTSCharacteristics ()
	{
		charType = ALLOCATION;
		gtsDirection = Transmit_GTS;
		gtsLength = 0;
	}
	GTSCharType charType;
	GTSDirection gtsDirection;
	uint8_t gtsLength;
	bool operator== (GTSCharacteristics& d)
	{
		return ((d.gtsDirection == gtsDirection) && (d.gtsLength == gtsLength));
	}

};

struct CapabilityInfo
{

public:
	bool powerSource;
	uint8_t batteryInfo;
	bool SecCapabi;
	bool CoordCapabi;
	bool TrafficSup;

};

struct MacCapabilityInfo
{

public:
	bool powerSource;
	uint8_t batteryInfo;
	bool SecCapabi;
	bool CoordCapabi;
	bool TrafficSup;
	uint8_t TopoSupport;
	uint8_t DeviceType;
	bool BeaconSupport;
	bool DimSupport;
	bool ContinuVisiTrans;
	bool CvdSopport;
	bool AllocateAddr;

};

struct PendAddrSpec
{

public:
	uint8_t numOfExtendedAddr;
	uint8_t numOfShortAddr;

};

struct GtsSpecification{
public:
	uint8_t GtsDescriptorCount;
	bool GtsPermit;
};

struct  GtsDirection{
public:
	uint8_t GTSDirctionMark;
};

struct  GtsDescriptor{
public:
	uint16_t deviceShortAddr;
	uint8_t gtsStartSlot;
	uint8_t gtsLenth;

	GtsDescriptor ()
	{
		deviceShortAddr = 0;
		gtsStartSlot = 0;
		gtsLenth = 0;
	}

	GtsDescriptor (uint16_t address, uint8_t slot, uint8_t lenth)
	{
		deviceShortAddr = address;
		gtsStartSlot = slot;
		gtsLenth = lenth;
	}
	bool operator== (GtsDescriptor& d)
		{
			return ((d.deviceShortAddr == deviceShortAddr) && (d.gtsLenth == gtsLenth)/* && (d.gtsStartSlot = gtsStartSlot)*/);
		}

	bool operator() (GtsDescriptor& d)
	{
		return ((d.deviceShortAddr == deviceShortAddr) && (d.gtsLenth == gtsLenth) /*&& (d.gtsStartSlot == gtsStartSlot)*/);
	}

};

struct GTSDesInfo{
public:
	GTSDesInfo(){
		counter = 0;
		gtsDirection = Transmit_GTS;
	}
	GtsDescriptor gtsDescriptor;
	uint16_t counter;
	GTSDirection gtsDirection;

	bool operator== (GTSDesInfo& d)
			{
				return ((d.counter == counter) && (d.gtsDescriptor == gtsDescriptor) && (d.gtsDirection == gtsDirection));
			}
	bool operator() (GTSDesInfo& g)
		{
			return ((g.gtsDescriptor == gtsDescriptor)
				&& (g.gtsDirection == gtsDirection));/* && (d.gtsLenth == gtsLenth) && (d.gtsStartSlot == gtsStartSlot)*/

		}
};

typedef std::vector<GtsDescriptor> GtsList;

//struct GtsInforField{
//public:
//	 GtsSpecification gtsspeci;
//	 GtsDirction gtsdirc;
//	 GtsDescr gtslist;
//
//};

struct GtsStructure{
	Timer gtsStartTime;
	uint16_t shortAddr;
	GTSDirection gtsdirc;
};

typedef std::vector<GtsStructure> GtsTimer;

struct PendAddrField{
public:
	PendAddrSpec pendAddr;
	AddrList addrList;
};

struct LifiBackoff
{
	LifiBackoff ();
	virtual ~LifiBackoff ();
	uint32_t* minBackoffExponential;
	uint32_t* maxBackoffExponential;
	uint32_t* maxBackoffRetry;
	uint32_t m_backoffExponential;
	uint32_t m_backoffRetries;
	Timer m_backoffTimer;
//	static Time LocateBackoffBoundary (Time capEnd);
	void IncreBackoffExp ();
	void IncreNumRetries ();
	bool IsReachMaxRetry ();
	int64_t GetBackoffTime ();
	void Reset ();
};

class TrxHandlerListener;

struct PacketInfo
{
	uint8_t m_handle;
	LogicChannelId m_band;
	TxOption m_option;
	DataRateId m_rate;
	uint32_t m_msduSize;
	bool m_bust;
	bool m_force;
	bool m_isAck;
	Ptr<Packet> m_packet;
	TrxHandlerListener* m_listener;
	void Reset ();
	bool Available ();
};

struct AssocDevAddress{
	Mac64Address m_extendedAddress;
	Mac16Address m_shortAddress;
};

//typedef std::vector<Mac64Address> AssocDevAddrs;

struct TransactionInfo
{
	uint16_t m_handle;
	Mac64Address m_extendDevAddress;
//	Timer m_timer;
	PacketInfo m_packetInfo;
	TrxHandlerListener* m_listener;
	EventId m_eventId;
};

typedef std::deque< std::pair<Mac64Address, TransactionInfo> > Transactions;

struct GtsTransactionInfo
{
	uint16_t m_handle;
	uint16_t m_ShortAddress;
	PacketInfo m_packetInfo;
	TrxHandlerListener* m_listener;
	EventId m_eventId;
};

typedef std::multimap<uint16_t, GtsTransactionInfo> GtsTransactions;

} /* namespace ns3 */

#endif /* LIFI_GENERAL_H_ */
