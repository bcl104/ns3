/*
 * lifi-phy.cc
 *
 *  Created on: 2014年4月4日
 *      Author: will
 */

#include "lifi-phy.h"
#include "ns3/log.h"


NS_LOG_COMPONENT_DEFINE("LifiPhy");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(LifiPhy);

LifiPhy::LifiPhy() {
	NS_LOG_FUNCTION(this);
	NS_FATAL_ERROR ("This constructor should not be called");
}

LifiPhy::~LifiPhy() {
	NS_LOG_FUNCTION(this);
}

LifiPhy::LifiPhy(Ptr<LifiSpectrumPhy> spectrum) {
	NS_LOG_FUNCTION(this);
	m_csTh = 1.0;
	m_edTh = 1.0;
	m_cellMode = false;
	m_trxMode = MULTIPLEX;
	m_txPower = 1;
	duration = 1;
	m_trxStatus = TRX_OFF;
	m_pdSapProvider = Create<PdSpecificSapProvider<LifiPhy> > (this);
	m_plmeSapProvider = Create<PlmeSpecificSapProvider<LifiPhy> > (this);
	m_spectrumPhy = spectrum;
}

LifiPhy::LifiPhy(LifiPhyPibAttribute attributes, Ptr<LifiSpectrumPhy> spectrum) {
	NS_LOG_FUNCTION(this);
	m_csTh = 1.0;
	m_edTh = 1.0;
	m_cellMode = false;
	m_trxMode = MULTIPLEX;
	m_txPower = 1;
	duration = 1;
	m_trxStatus = TRX_OFF;
	m_pdSapProvider = Create<PdSpecificSapProvider<LifiPhy> > (this);
	m_plmeSapProvider = Create<PlmeSpecificSapProvider<LifiPhy> > (this);
	m_attributes = attributes;
	m_spectrumPhy = spectrum;
}

LifiPhy::LifiPhy(std::vector< Ptr<LifiCell> > cellList){
	NS_LOG_FUNCTION(this);
	m_csTh = 1.0;
	m_edTh = 1.0;
	m_cellMode = false;
	m_trxMode = MULTIPLEX;
	m_txPower = 1;
	duration = 1;
	m_trxStatus = TRX_OFF;
	m_pdSapProvider = Create<PdSpecificSapProvider<LifiPhy> > (this);
	m_plmeSapProvider = Create<PlmeSpecificSapProvider<LifiPhy> > (this);
	m_cellList = cellList;
}

TypeId LifiPhy::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiPhy")
						.SetParent<Object>()
						.AddConstructor<LifiPhy>();
	return tid;
}

Ptr<LifiPhyPibAttribute> LifiPhy::GetPhyPibAttributes() {
	NS_LOG_FUNCTION(this);
	return &m_attributes;
}

bool LifiPhy::DoCca() {
	NS_LOG_FUNCTION(this);
	return false;
}

uint8_t LifiPhy::DoCca(uint8_t band) {
	NS_LOG_FUNCTION(this);
	return 0;
}

void LifiPhy::SetDeviceAttribute(PhyOpStatus status) {
	NS_LOG_FUNCTION(this);

}

void LifiPhy::Transmit(uint32_t size, Ptr<Packet> pb, uint8_t band) {
	NS_LOG_FUNCTION(this);
	if(m_trxStatus == TX_ON){
		m_spectrumPhy->GetChannel()->AddTx((uint8_t)m_attributes.GetAttributes(PHY_CURRENT_CHANNEL),m_spectrumPhy);
		double rate = GetRate (m_phyheader->GetMcsId());
		duration = Seconds(m_phyheader->GetSerializedSize()/200000 + pb->GetSize()/(1000*rate));
		pb->AddHeader (*m_phyheader);
		StartTx (pb);
	}
	else
		EndTx (m_trxStatus);
}

void LifiPhy::Receive(Ptr<LifiSpectrumSignalParameters> param) {
	NS_LOG_FUNCTION(this);
}

void LifiPhy::SetTRxState(PhyOpStatus state) {
	NS_LOG_FUNCTION(this);
	m_trxStatus = state;
		if(m_trxStatus == RX_ON)
			m_spectrumPhy->GetChannel()->AddRx((uint8_t)m_attributes.GetAttributes(PHY_CURRENT_CHANNEL),m_spectrumPhy);
		else if(m_trxStatus == TRX_OFF){
			m_spectrumPhy->GetChannel()->DeleteRx((uint8_t)m_attributes.GetAttributes(PHY_CURRENT_CHANNEL),m_spectrumPhy);
			m_spectrumPhy->GetChannel()->DeleteTx((uint8_t)m_attributes.GetAttributes(PHY_CURRENT_CHANNEL),m_spectrumPhy);
		}
		else{

		}
}

double LifiPhy::GetTxPower() {
	NS_LOG_FUNCTION(this);
	return m_txPower;
}

double LifiPhy::GetEdThreadhold() {
	NS_LOG_FUNCTION(this);
	return m_edTh;
}

double LifiPhy::GetCsThreadhold() {
	NS_LOG_FUNCTION(this);
	return m_csTh;
}
void LifiPhy::SetcellMode(bool cellmode){
	NS_LOG_FUNCTION(this);
	m_cellMode = cellmode;
}
bool LifiPhy::GetcellMode(){
	NS_LOG_FUNCTION(this);
	return m_cellMode;
}

void LifiPhy::DoSetCellId(uint16_t cellId) {
	NS_LOG_FUNCTION(this);
}
void LifiPhy::SetcellId(uint8_t cellId){
	NS_LOG_FUNCTION(this);
	m_cellId = cellId;
}

uint8_t LifiPhy::GetcellId(){
	NS_LOG_FUNCTION(this);
	return m_cellId;
}

void LifiPhy::SettrxId(uint8_t trxid){
	NS_LOG_FUNCTION(this);
	m_trxid = trxid;

}

uint8_t LifiPhy::GettrxId(){
	NS_LOG_FUNCTION(this);
	return m_trxid;

}

void LifiPhy::StartTx(Ptr<Packet> pb) {
	NS_LOG_FUNCTION(this);
//	m_spectrumPhy->Send()
	if(!m_cellMode){
		m_spectrumPhy->Send(pb,pb->GetSize(),(uint8_t)m_attributes.GetAttributes(PHY_CURRENT_CHANNEL),m_cellMode,m_cellId,m_trxid,m_txPower,duration );
		m_endTxEvent = Simulator::Schedule (duration, &LifiPhy::EndTx, TX_ON);
	}
	else{

	}




}

void LifiPhy::EndTx(PhyOpStatus m_trxStatus) {
	NS_LOG_FUNCTION(this);
	if(m_trxStatus == TX_ON){
		SetTRxState(TRX_OFF);
	}
	m_pdSapUser->PdDataConfirm (m_trxStatus);
}

void LifiPhy::Switch(bool** sw_bit_map, bool dir) {
	NS_LOG_FUNCTION(this);
}

Ptr<PdSapProvider> LifiPhy::GetPdSapProvider() {
	NS_LOG_FUNCTION(this);
	return m_pdSapProvider;
}

Ptr<PlmeSapProvider> LifiPhy::GetPlmeSapProvider() {
	NS_LOG_FUNCTION(this);
	return m_plmeSapProvider;
}

void LifiPhy::SetPdSapUser(Ptr<PdSapUser> u) {
	NS_LOG_FUNCTION(this);
	m_pdSapUser = u;
}

void LifiPhy::SetPlmeSapUser(Ptr<PlmeSapUser> u) {
	NS_LOG_FUNCTION(this);
	m_PlmeSapUser = u;
}

Ptr<LifiNetDevice> LifiPhy::GetLifiNetDevice() {
	NS_LOG_FUNCTION(this);
	return m_device;
}

void LifiPhy::SetLifiNetDevice(Ptr<LifiNetDevice> device) {
	NS_LOG_FUNCTION(this);
	m_device = device;
}

Ptr<LifiSpectrumPhy>  LifiPhy::GetSpectrumPhy () {
	NS_LOG_FUNCTION(this);
	return m_spectrumPhy;
}

//void LifiPhy::AddLifiSpectrumPhy(Ptr<LifiSpectrumPhy> spectrum) {
//	NS_LOG_FUNCTION(this);
//	m_spectrumPhyList.push_back(spectrum);
//}

double LifiPhy::GetRate(uint8_t mcsId) {
	NS_LOG_FUNCTION(this);
	switch(mcsId){
	/*PHY I*/
	case 0:
		return 11.67;break;
	case 1:
		return 22.44;break;
	case 2:
		return 48.89;break;
	case 3:
		return 73.3;break;
	case 4:
		return 100;break;
	case 5:
		return 35.56;break;
	case 6:
		return 71.11;break;
	case 7:
		return 124.4;break;
	case 8:
		return 266.6;break;
	/*PHY II*/
	case 16:
		return 1250;break;
	case 17:
		return 2000;break;
	case 18:
		return 2500;break;
	case 19:
		return 4000;break;
	case 20:
		return 5000;break;
	case 21:
		return 6000;break;
	case 22:
		return 9600;break;
	case 23:
		return 12000;break;
	case 24:
		return 18000;break;
	case 25:
		return 24000;break;
	case 26:
		return 38400;break;
	case 27:
		return 48000;break;
	case 28:
		return 76800;break;
	case 29:
		return 96000;break;
	/*PHY III*/
	case 32:
		return 12000;break;
	case 33:
		return 18000;break;
	case 34:
		return 24000;break;
	case 35:
		return 36000;break;
	case 36:
		return 48000;break;
	case 37:
		return 72000;break;
	default:
		return 96000;break;
	}
	return -1;
}
void LifiPhy::SetLifiPhyHeader (Ptr<LifiPhyHeader> lifiphyheader){
	NS_LOG_FUNCTION(this);
	m_phyheader = lifiphyheader ;
}
Ptr<LifiPhyHeader> LifiPhy::GetLifiPhyHeader(){
	NS_LOG_FUNCTION(this);
	return m_phyheader;
}
} /* namespace ns3 */

