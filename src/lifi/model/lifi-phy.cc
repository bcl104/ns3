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
	m_trxMode = MULTIPLEX;
	m_txPower = 1;
	m_trxStatus = TRX_OFF;
	m_pdSapProvider = Create<PdSpecificSapProvider<LifiPhy> > (this);
	m_plmeSapProvider = Create<PlmeSpecificSapProvider<LifiPhy> > (this);
	m_spectrumPhy = spectrum;
}

LifiPhy::LifiPhy(LifiPhyPibAttribute attributes, Ptr<LifiSpectrumPhy> spectrum) {
	NS_LOG_FUNCTION(this);
	m_csTh = 1.0;
	m_edTh = 1.0;
	m_trxMode = MULTIPLEX;
	m_txPower = 1;
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
	m_trxMode = MULTIPLEX;
	m_txPower = 1;
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

void LifiPhy::Transmit(uint32_t size, Ptr<PacketBurst> pb, uint8_t band) {
	NS_LOG_FUNCTION(this);
}

void LifiPhy::Receive(Ptr<LifiSpectrumSignalParameters> param) {
	NS_LOG_FUNCTION(this);
}

void LifiPhy::SetTRxState(PhyOpStatus state) {
	NS_LOG_FUNCTION(this);
}

double LifiPhy::GetTxPower() {
	NS_LOG_FUNCTION(this);
	return 0.0;
}

double LifiPhy::GetEdThreadhold() {
	NS_LOG_FUNCTION(this);
	return 0.0;
}

double LifiPhy::GetCsThreadhold() {
	NS_LOG_FUNCTION(this);
	return 0.0;
}

void LifiPhy::DoSetCellId(uint16_t cellId) {
	NS_LOG_FUNCTION(this);
}

void LifiPhy::StartTx(Ptr<PacketBurst> pb) {
	NS_LOG_FUNCTION(this);
}

void LifiPhy::EndTx() {
	NS_LOG_FUNCTION(this);
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

double LifiPhy::GetRate(char mcsID) {
	NS_LOG_FUNCTION(this);
	return 0.0;
}


} /* namespace ns3 */

