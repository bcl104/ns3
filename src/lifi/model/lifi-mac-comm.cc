/*
 * lifi-mac-comm.cc
 *
 *  Created on: 2014年3月31日
 *      Author: will
 */

#include "lifi-mac-comm.h"


namespace ns3 {

LifiMacComm::LifiMacComm() {

}

LifiMacComm::~LifiMacComm() {
}

Ptr<Packet> LifiMacComm::GetPacket(){
	Buffer b = Serialize();
	Ptr<Packet> p = Create<Packet> (b.PeekData(), b.GetSize());
	return p;
}

/* LifiMacComm Get and Set Function  */
uint8_t LifiMacComm::GetCommId () const{
	return m_commId;
}

void LifiMacComm::SetCommId(uint8_t commid){
	NS_ASSERT((commid >= 0) && (commid <= 15));
	m_commId = (CommId)commid;
}

/* AssocRqstComm Get and Set Function  */
bool AssocRqstComm::GetPowerSource () const{
	return m_capabilityInfo.powerSource;
}

void AssocRqstComm::SetPowerSource(bool powersour){
	m_capabilityInfo.powerSource = powersour;
}

uint8_t AssocRqstComm::GetCapInfo_BatInfo () const{
	return m_capabilityInfo.batteryInfo;
}

void AssocRqstComm::SetCapInfo_BatInfo(uint8_t batinfo){
	m_capabilityInfo.batteryInfo = batinfo;
}


bool AssocRqstComm::GetSecCapabi () const{
	return m_capabilityInfo.SecCapabi;
}

void AssocRqstComm::SetSecCapabi(bool seccapab){
	m_capabilityInfo.SecCapabi = seccapab;
}


bool AssocRqstComm::GetCoordCapab () const{
	return m_capabilityInfo.CoordCapabi;
}

void AssocRqstComm::SetCoordCapab(bool coordcapa){
	m_capabilityInfo.CoordCapabi = coordcapa;
}


bool AssocRqstComm::GetTrafficSup() const{
	return m_capabilityInfo.TrafficSup;
}

void AssocRqstComm::SetTrafficSup(bool traffsup){
	m_capabilityInfo.TrafficSup = traffsup;
}

/* GtsResponseComm Get and Set Function  */
uint8_t GtsResponseComm::GetgtsLenth () const{
	return m_characteristic.gtsLength;
}

void GtsResponseComm::SetgtsLenth(uint8_t gtslenth){
		NS_ASSERT((gtslenth >= 0) && (gtslenth <= 15));
		m_characteristic.gtsLength = gtslenth;
	}


bool GtsResponseComm::GetDirection () const{
	return m_characteristic.gtsDirection;
}

void GtsResponseComm::SetgtsDirection(bool gtsdirection){
	m_characteristic.gtsDirection = (GTSDirection)gtsdirection;
}


bool GtsResponseComm::GetgtsCharType () const{
	return m_characteristic.charType;

}

void GtsResponseComm::SetgtsCharType(bool gtschartype){
	m_characteristic.charType = (GTSCharType)gtschartype;
}


uint8_t GtsResponseComm::Getgtsstartslot () const{
	return m_startingSlot;
}

void GtsResponseComm::Setgtsstartslot(uint8_t startslot){
	NS_ASSERT (startslot>=0 && startslot <=255);
	m_startingSlot = startslot;
}

/*  DisassocNotificationComm Get and Set Function  */
uint8_t DisassocNotificationComm::GetDissassocReason () const{
	return (uint8_t)reason;
}

void DisassocNotificationComm::SetDissassocReason(uint8_t disassreason){
	NS_ASSERT (disassreason == 0x00 || disassreason == 0x01|| disassreason == 0x03);
	reason = (Reason)disassreason;
}

/*  MultiChannelAssignComm Get and Set Function  */
uint8_t MultiChannelAssignComm::GetMultiChannel () const{
	return m_multiChannel;
}

void MultiChannelAssignComm::SetMultiChannel(uint8_t multichannel){
	NS_ASSERT (multichannel >= 0 && multichannel <= 255);
	m_multiChannel = multichannel;
}

/* GtsRequestComm Get and Set Function  */
uint8_t GtsRequestComm::GetgtsLenth () const{
	return m_characteristic.gtsLength;
}

void GtsRequestComm::SetgtsLenth(uint8_t gtslenth){
		NS_ASSERT((gtslenth >= 0) && (gtslenth <= 15));
		m_characteristic.gtsLength = gtslenth;
	}


bool GtsRequestComm::GetDirection () const{
	return m_characteristic.gtsDirection;
}

void GtsRequestComm::SetgtsDirection(bool gtsdirection){
	m_characteristic.gtsDirection = (GTSDirection)gtsdirection;
}


bool GtsRequestComm::GetgtsCharType () const{
	return m_characteristic.charType;

}

void GtsRequestComm::SetgtsCharType(bool gtschartype){
	m_characteristic.charType = (GTSCharType)gtschartype;
}

/*  AssocResponseComm Get and Set Function  */
uint16_t AssocResponseComm::GetShortAddr () const{
     return m_shortAddr;
}

void AssocResponseComm::SetShortAddr(uint16_t shortaddr){
	NS_ASSERT(shortaddr != 0xffff && shortaddr != 0xfffe);
	m_shortAddr = shortaddr;
}

uint8_t AssocResponseComm::GetAssocStatus () const{
	return m_assocStatus;
}

void AssocResponseComm::SetAssocStatus(uint8_t assocstatus){
	NS_ASSERT (assocstatus ==0x00 || assocstatus == 0x01|| assocstatus == 0x03);
	m_assocStatus = (MacAssocStatus)assocstatus;
}

uint8_t AssocResponseComm::GetCapNegoResponse () const{
	return m_capabilityNegoResponse;
}

void AssocResponseComm::SetCapNegoResponse(uint8_t capnegores){
	m_capabilityNegoResponse = capnegores;
}



AssocRqstComm::AssocRqstComm() {
	m_commId = ASSOC_REQUEST;
}

AssocRqstComm::AssocRqstComm(Ptr<Packet> p) {
	uint32_t size = p->GetSize();
	uint8_t* b = (uint8_t*) malloc (size);
	p->CopyData(b, size);
	Deserialize(b, size);
	free (b);
}

AssocRqstComm::~AssocRqstComm() {
}

void AssocRqstComm::Deserialize(const uint8_t *data, uint32_t size) {
	uint8_t comm_id = *data ;
	NS_ASSERT (((uint8_t)ASSOC_RESPONSE) == comm_id);
}

AssocRqstComm::AssocRqstComm(CapabilityInfo& info) {

}

AssocRqstComm& AssocRqstComm::Construct(Ptr<Packet> p) {
	static AssocRqstComm comm;
	comm = AssocRqstComm (p);
	return comm;
}

Buffer AssocRqstComm::Serialize() {
	Buffer b;
	b.AddAtStart(3);
	Buffer::Iterator it = b.Begin();
	it.WriteU8((uint8_t)m_commId);
	uint8_t payload_length = 1;
	it.WriteU8(payload_length);
	return b;
}

GtsResponseComm::GtsResponseComm() {
	m_commId = GTS_RESPONSE;
	m_startingSlot = 0xf1;
}

GtsResponseComm::~GtsResponseComm() {
}

void GtsResponseComm::Deserialize(const uint8_t *data, uint32_t size) {
	m_commId = (CommId)*data;
	data+=2;
	NS_ASSERT(m_commId == ((uint8_t)GTS_RESPONSE));
	m_characteristic.gtsLength = ((*data) & 0xf0)>> 4;
	m_characteristic.gtsDirection = (GTSDirection)(((*data) & 0x08) >> 3);
	m_characteristic.charType = (GTSCharType)(((*data) & 0x04) >> 2);
	data++;
	m_startingSlot = *data;
}

GtsResponseComm::GtsResponseComm(GTSCharacteristics c, uint8_t slot) {
	m_characteristic = c;
	NS_ASSERT(m_characteristic.gtsLength >= 0 && m_characteristic.gtsLength <= 15);
	m_startingSlot = slot;
	NS_ASSERT(m_startingSlot >= 0 && m_startingSlot <= 255);
}

GtsResponseComm& GtsResponseComm::Construct(Ptr<Packet> p) {
	static GtsResponseComm comm;
	comm = GtsResponseComm (p);
	return comm;
}

GtsResponseComm::GtsResponseComm(Ptr<Packet> p) {
	uint32_t size = p->GetSize();
	uint8_t* b = (uint8_t*) malloc (size);
	p->CopyData(b, size);
	Deserialize(b, size);
	free (b);
}

Buffer GtsResponseComm::Serialize() {
	Buffer data;
	data.AddAtStart(4);
	Buffer::Iterator it = data.Begin();
	it.WriteU8((uint8_t)m_commId);
	it.WriteU8(2);
	uint8_t gts_character = 0;
	NS_ASSERT (m_characteristic.gtsLength >= 0 && m_characteristic.gtsLength <= 15);
	gts_character = m_characteristic.gtsLength <<4
			         | m_characteristic.gtsDirection << 3
					 | m_characteristic.charType << 2 ;
	it.WriteU8(gts_character);
	NS_ASSERT(m_startingSlot >= 0 && m_startingSlot <= 255);
	it.WriteU8(m_startingSlot);
	return data;
}

DisassocNotificationComm::DisassocNotificationComm() {
	m_commId = DISASSOC_NOTIFICATION;
	reason = DISASSOC_COORD;
}

DisassocNotificationComm::~DisassocNotificationComm() {
}

void DisassocNotificationComm::Deserialize(const uint8_t *data, uint32_t size) {
	m_commId = (CommId)*data;
	data += 2;
	NS_ASSERT(m_commId == ((uint8_t)GTS_RESPONSE));
	reason = (Reason)*data;
}

DisassocNotificationComm& DisassocNotificationComm::Construct(Ptr<Packet> p) {
	static DisassocNotificationComm comm;
	comm = DisassocNotificationComm (p);
	return comm;
}

DisassocNotificationComm::DisassocNotificationComm(Ptr<Packet> p) {
	uint32_t size = p->GetSize();
	uint8_t* b = (uint8_t*) malloc (size);
	p->CopyData(b, size);
	Deserialize(b, size);
	free (b);
}

Buffer DisassocNotificationComm::Serialize() {
	Buffer data;
	data.AddAtStart(3);
	Buffer::Iterator it = data.Begin();
	it.WriteU8((uint8_t)m_commId);
	uint8_t payload_length = 1;
	it.WriteU8(payload_length);
	it.WriteU8((uint8_t)reason);
	return data;
}

BeaconRequestComm::BeaconRequestComm() {
	m_commId = BEACON_REQUEST;
}

BeaconRequestComm::BeaconRequestComm(Ptr<Packet> p) {
	uint32_t size = p->GetSize();
	uint8_t* data = (uint8_t*) malloc (size);
	p->CopyData(data, size);
	Deserialize(data, size);
	free (data);
}

BeaconRequestComm::~BeaconRequestComm() {
}

void BeaconRequestComm::Deserialize(const uint8_t *data, uint32_t size) {
	uint8_t comm_id = *data;
	NS_ASSERT (comm_id == ((uint8_t)BEACON_REQUEST));
}

BeaconRequestComm& BeaconRequestComm::Construct(Ptr<Packet> p) {
	static BeaconRequestComm comm;
	comm = BeaconRequestComm (p);
	return comm;
}

Buffer BeaconRequestComm::Serialize() {
	Buffer data;
	data.AddAtStart(2);
	Buffer::Iterator it = data.Begin();
	it.WriteU8((uint8_t)m_commId);
	uint8_t payload_length = 1;
	it.WriteU8(payload_length);
	return data;
}

MultiChannelAssignComm::MultiChannelAssignComm() {
	m_commId = MULTI_CHANNEL_ASSIGNMENT;
	m_multiChannel = 1;
}

MultiChannelAssignComm::MultiChannelAssignComm(Ptr<Packet> p) {
	uint32_t size = p->GetSize();
	uint8_t *data = (uint8_t*) malloc (size);
	p->CopyData(data, size);
	Deserialize(data, size);
	free (data);
}

MultiChannelAssignComm::~MultiChannelAssignComm() {
}

void MultiChannelAssignComm::Deserialize(const uint8_t *data, uint32_t size) {
	m_commId = (CommId)*data;
	NS_ASSERT(m_commId == ((uint8_t)MULTI_CHANNEL_ASSIGNMENT));
	data += 2;
	m_multiChannel = *data;
}

MultiChannelAssignComm::MultiChannelAssignComm(uint8_t multiChannel) {
	m_multiChannel = multiChannel;
}

MultiChannelAssignComm& MultiChannelAssignComm::Construct(Ptr<Packet> p) {
	static MultiChannelAssignComm comm;
	comm = MultiChannelAssignComm (p);
	return comm;
}

Buffer MultiChannelAssignComm::Serialize() {
	Buffer data;
	data.AddAtStart(3);
	Buffer::Iterator it = data.Begin();
	it.WriteU8(m_commId);
	uint8_t payload_length = 1;
	it.WriteU8(payload_length);
	it.WriteU8(m_multiChannel);
	return data;
}

InfoElementComm::InfoElementComm() {
	m_commId = INFORMANTION_ELEMENT;
    m_elementId = 0x01;
    m_lenth = 0x01;
    m_playload = 0x01;
}

InfoElementComm::InfoElementComm(Ptr<Packet> p) {
	uint32_t size = p->GetSize();
	uint8_t* b = (uint8_t*) malloc (size);
	p->CopyData(b, size);
	Deserialize(b, size);
	free (b);
}

InfoElementComm::~InfoElementComm() {
}

void InfoElementComm::Deserialize(const uint8_t *data, uint32_t size) {
}

InfoElementComm::InfoElementComm(uint8_t id, uint8_t lenth, uint8_t payload) {
	    m_elementId = id;
	    m_lenth = lenth;
	    m_playload = payload;
}

InfoElementComm& InfoElementComm::Construct(Ptr<Packet> p) {
	static InfoElementComm comm;
	comm = InfoElementComm (p);
	return comm;
}

Buffer InfoElementComm::Serialize() {
	return 0;
}

GtsRequestComm::GtsRequestComm() {
	m_commId = GTS_REQUEST;
}

GtsRequestComm::GtsRequestComm(Ptr<Packet> p) {
	uint32_t size = p->GetSize();
	uint8_t *data = (uint8_t*) malloc (size);
	p->CopyData(data, size);
	Deserialize(data, size);
	free (data);
}

GtsRequestComm::~GtsRequestComm() {
}

void GtsRequestComm::Deserialize(const uint8_t *data, uint32_t size) {
	uint8_t comm_id = *data;
	NS_ASSERT(comm_id == ((uint8_t)GTS_REQUEST));
	data += 2;
	m_characteristic.gtsLength = (*data) & 0x0f;
	m_characteristic.gtsDirection = (GTSDirection)(((*data) & 0x10) >> 4);
	m_characteristic.charType = (GTSCharType)(((*data) & 0x20) >> 5);
}

GtsRequestComm::GtsRequestComm(GTSCharacteristics c) {
	m_characteristic = c;
}

GtsRequestComm& GtsRequestComm::Construct(Ptr<Packet> p) {
	static GtsRequestComm comm;
	comm = GtsRequestComm (p);
	return comm;
}

Buffer GtsRequestComm::Serialize() {
	Buffer data;
	data.AddAtStart(3);
	Buffer::Iterator it = data.Begin();
	it.WriteU8((uint8_t)m_commId);
	uint8_t payload_length = 1;
	it.WriteU8(payload_length);
	uint8_t gts_character = 0;
	NS_ASSERT (m_characteristic.gtsLength >= 0 && m_characteristic.gtsLength <= 15);
	gts_character = m_characteristic.gtsLength | m_characteristic.gtsDirection << 4
					|m_characteristic.charType << 5;
	it.WriteU8(gts_character);
	return data;
}

AssocResponseComm::AssocResponseComm() {
	m_commId = ASSOC_RESPONSE;
    m_assocStatus = VPAN_CAPACITY;
	m_capabilityNegoResponse = 0x01;
	m_shortAddr = 0x0f0f;
}

AssocResponseComm::AssocResponseComm(Ptr<Packet> p) {
	uint32_t size = p->GetSize();
	uint8_t* data = (uint8_t*) malloc (size);
	p->CopyData(data, size);
	Deserialize(data, size);
	free (data);
}
AssocResponseComm::~AssocResponseComm() {
}

void AssocResponseComm::Deserialize(const uint8_t *data, uint32_t size) {
	m_commId = (CommId)*data ;
	NS_ASSERT (((uint8_t)ASSOC_RESPONSE) == m_commId);
	data += 2;
	m_shortAddr = *data++ ;
	m_shortAddr = (m_shortAddr<<8)|(*data);
	data++;
	m_assocStatus =(MacAssocStatus)(*data);
	data++;
	m_capabilityNegoResponse= (uint8_t)(((*data) & 0xc0) >> 6) ;
}

AssocResponseComm::AssocResponseComm(MacAssocStatus assoc, uint8_t cap,
		uint16_t address) {
	 m_assocStatus = assoc;
	 m_capabilityNegoResponse = cap;
	 m_shortAddr = address;
}

AssocResponseComm& AssocResponseComm::Construct(Ptr<Packet> p) {
	static AssocResponseComm comm;
	comm = AssocResponseComm (p);
	return comm;
}

Buffer AssocResponseComm::Serialize() {
	Buffer b;
	b.AddAtStart(6);
	Buffer::Iterator it = b.Begin();
	it.WriteU8((uint8_t)m_commId);
	it.WriteU8(2);
	it.WriteU16((uint16_t)m_shortAddr);
	it.WriteU8((uint8_t)m_assocStatus);
	it.WriteU8(((m_capabilityNegoResponse)<<6));
	return b;
}

MobilityNotifiComm::MobilityNotifiComm() {
	m_commId = MOBILITY_NOTIFICATION;
}

MobilityNotifiComm::MobilityNotifiComm(Ptr<Packet> p) {
	uint32_t size = p->GetSize();
	uint8_t* data = (uint8_t*) malloc (size);
	p->CopyData(data, size);
	Deserialize(data, size);
	free (data);
}
MobilityNotifiComm::~MobilityNotifiComm() {
}

void MobilityNotifiComm::Deserialize(const uint8_t *data, uint32_t size) {
	m_commId = (CommId)*data;
	NS_ASSERT (((uint8_t)MOBILITY_NOTIFICATION) == m_commId);
	data +=2;
}

MobilityNotifiComm& MobilityNotifiComm::Construct(Ptr<Packet> p) {
	static MobilityNotifiComm comm;
	comm = MobilityNotifiComm (p);
	return comm;
}

Buffer MobilityNotifiComm::Serialize() {
	Buffer b;
	b.AddAtStart(2);
	Buffer::Iterator it = b.Begin();
	it.WriteU8((uint8_t)m_commId);
	it.WriteU8(2);
	return b;
}

DataRequestComm::DataRequestComm() {
	m_commId = DATA_REQUEST;
}

DataRequestComm::~DataRequestComm() {
}

void DataRequestComm::Deserialize(const uint8_t *data, uint32_t size) {
	m_commId = (CommId)*data++;
	NS_ASSERT (((uint8_t)DATA_REQUEST) == m_commId);
	data++;
}

DataRequestComm& DataRequestComm::Construct(Ptr<Packet> p) {
	static DataRequestComm comm;
	comm = DataRequestComm (p);
	return comm;
}

DataRequestComm::DataRequestComm(Ptr<Packet> p) {
	uint32_t size = p->GetSize();
	uint8_t* b = (uint8_t*) malloc (size);
	p->CopyData(b, size);
	Deserialize(b, size);
	free (b);
}

Buffer DataRequestComm::Serialize() {
	return 0;
}

} /* namespace ns3 */


