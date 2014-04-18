/*
 * LifiPhyHeader.cc
 *
 *  Created on: 2014年4月14日
 *      Author: pp
 */

#include "lifi-phy-header.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("LifiPhyHeader");

namespace ns3 {

LifiPhyHeader::LifiPhyHeader() {
	// TODO Auto-generated constructor stub
	m_isBurstMode = 0;
	m_channelNum = 0;
	m_mcsId = 0;
	m_psduLength = 0;
	m_ookDimmed = 0;
	m_reservedFields = 0;
	m_hcs = 0;
	m_flpSize = 0;

}

LifiPhyHeader::~LifiPhyHeader() {
	// TODO Auto-generated destructor stub
}

TypeId LifiPhyHeader::GetTypeId (void){
	static TypeId tid=TypeId("ns3::LifiPhyHeader")
			.SetParent<Header>()
			.AddConstructor<LifiPhyHeader>()
			;
	return tid;
}

TypeId LifiPhyHeader::GetInstanceTypeId (void) const{
	return GetTypeId();
}

void LifiPhyHeader::Print (std::ostream &os) const{

}

uint32_t LifiPhyHeader::GetSerializedSize (void) const{
	uint32_t PHY_headerField_Size = 4;
//	uint32_t header_size = PHY_headerField_Size;
//	return header_size;//by shuangxing
	return PHY_headerField_Size;//modified by pp on 2014.04.18
}

void LifiPhyHeader::Serialize (Buffer::Iterator start) const{
	Buffer::Iterator it = start;
	uint32_t PHY_headerField =  ((uint8_t)m_isBurstMode & 0x01)
			                    | ((m_channelNum & 0x07 ) << 1)
			                    | ((m_mcsId & 0x3F) << 4)
			                    | ((m_psduLength & 0xFFFF) << 10)
			                    | ((((uint8_t)m_ookDimmed & 0x01)) << 26)
			                    | ((m_reservedFields & 0x1F) << 27);
	it.WriteU32(PHY_headerField);

}

uint32_t LifiPhyHeader::Deserialize (Buffer::Iterator start){
	Buffer::Iterator it = start;
	uint32_t PHY_headerField = it.ReadU32();
	m_isBurstMode = (bool) (PHY_headerField & 0x000001);
	m_channelNum = (uint8_t) ((PHY_headerField >> 1) & 0x000007);
	m_mcsId = (uint8_t) ((PHY_headerField >> 4) & 0x00003F);
	m_psduLength = (uint16_t) ((PHY_headerField >> 10) & 0x00FFFF);
//	m_ookDimmed = (bool) ((PHY_headerField << 26) & 0x000001);//by shuangxing
	m_ookDimmed = (bool) ((PHY_headerField >> 26) & 0x000001);//modified by pp on 2014.04.18
	m_reservedFields = (uint8_t) ((PHY_headerField >> 27) & 0x00001F);
	return it.GetDistanceFrom(start);
}

void LifiPhyHeader::SetBurstMode(bool isBurstMode){
	m_isBurstMode = isBurstMode;
}

bool LifiPhyHeader::GetBurstMode(){
	return m_isBurstMode;
}

void LifiPhyHeader::SetChannelNum(uint8_t channelNum){
	m_channelNum = channelNum;
}

uint8_t LifiPhyHeader::GetChannelNum(){
	return m_channelNum;
}

void LifiPhyHeader::SetMcsId(uint8_t mcsId){
	m_mcsId = mcsId;
}

uint8_t LifiPhyHeader::GetMcsId(){
	return m_mcsId;
}

void LifiPhyHeader::SetPsduLength(uint16_t length){
	m_psduLength = length;
}

uint16_t LifiPhyHeader::GetPsduLength(){
	return m_psduLength;
}

void LifiPhyHeader::SetOokDimmed(bool isOok){
	m_ookDimmed = isOok;
}

bool LifiPhyHeader::GetOokDimmed(){
	return m_ookDimmed;
}

void LifiPhyHeader::SetReservedFields(uint8_t field){
	m_reservedFields = field;
}

uint8_t LifiPhyHeader::GetReservedFields(){
	return m_reservedFields;
}

void LifiPhyHeader::SetPreamble(SHR s) {
	m_shr = s;
}

SHR LifiPhyHeader::GetPreamble() {
	return m_shr;
}

void LifiPhyHeader::SetFlpSize(uint64_t size) {
	m_flpSize = size;
}

uint64_t LifiPhyHeader::GetFlpSize() {
	return m_flpSize;
}

void LifiPhyHeader::SetHcs(uint16_t hcs) {
	m_hcs = hcs;
}

uint16_t LifiPhyHeader::GetHcs() {
	return m_hcs;
}

} /* namespace ns3 */
