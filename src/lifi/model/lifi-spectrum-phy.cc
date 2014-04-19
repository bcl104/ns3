 /* lifi-spectrum-phy.cc*/

#include "lifi-spectrum-phy.h"
#include "ns3/log.h"

namespace ns3 {

LifiSpectrumPhy::LifiSpectrumPhy() {
	NS_LOG_FUNCTION(this);
	m_band = 0;
	m_cellId = 0;
}

LifiSpectrumPhy::LifiSpectrumPhy(Ptr<NetDevice> device) {
	NS_LOG_FUNCTION(this);
	LifiSpectrumPhy();
	m_device = device;
	m_cellId = 0;
	m_band = 0;
}

LifiSpectrumPhy::~LifiSpectrumPhy() {
	NS_LOG_FUNCTION(this);
}

TypeId LifiSpectrumPhy::GetTypeId() {
	NS_LOG_FUNCTION(this);
	static TypeId tid = TypeId ("ns3::LifiSpectrumPhy")
						.SetParent<SpectrumPhy>()
						.AddConstructor<LifiSpectrumPhy>();
	return tid;
}

void LifiSpectrumPhy::SetNodeSpectrum(uint8_t channel) {
	NS_LOG_FUNCTION(this);
}

void LifiSpectrumPhy::Send(Ptr<Packet>pb,uint32_t size,uint8_t band,  bool isCellMode,  uint8_t cellId,  uint8_t trxid , double txPower,Time duration) {
	NS_LOG_FUNCTION(this);
	m_SignalParameters->band = band;
	m_SignalParameters->cellId = cellId;
	m_SignalParameters->cellMode = isCellMode;
	m_SignalParameters->duration = duration;
	m_SignalParameters->pb = pb;
	m_SignalParameters->trxId = trxid;
	m_SignalParameters->txPower = txPower;
	m_SignalParameters->txPhy = this;
	m_channel->StartTx(m_SignalParameters);
}

bool LifiSpectrumPhy::CarrierSense() {
	NS_LOG_FUNCTION(this);
	return false;
}

uint8_t LifiSpectrumPhy::CarrierSense(uint8_t band) {
	NS_LOG_FUNCTION(this);
	return 0;
}

void LifiSpectrumPhy::SetDevice(Ptr<NetDevice> device) {
	NS_LOG_FUNCTION(this);
	m_device = device;
}

Ptr<NetDevice> LifiSpectrumPhy::GetDevice() {
	NS_LOG_FUNCTION(this);
	return m_device;
}

void LifiSpectrumPhy::SetMobility(Ptr<MobilityModel> m) {
	NS_LOG_FUNCTION(this);
	m_mobility = m;
}

Ptr<MobilityModel> LifiSpectrumPhy::GetMobility() {
	NS_LOG_FUNCTION(this);
	return m_mobility;
}

void LifiSpectrumPhy::SetChannel(Ptr<SpectrumChannel> c) {
	NS_LOG_FUNCTION(this);
	m_channel = c;
}

Ptr<LifiSpectrumChannel> LifiSpectrumPhy::GetChannel(){
	NS_LOG_FUNCTION(this);
	return m_channel;
}

Ptr<const SpectrumModel> LifiSpectrumPhy::GetRxSpectrumModel() const{
	NS_LOG_FUNCTION(this);
	return 0;
}

Ptr<AntennaModel> LifiSpectrumPhy::GetRxAntenna() {
	NS_LOG_FUNCTION(this);
	return 0;
}

void LifiSpectrumPhy::StartRx(Ptr<SpectrumSignalParameters> params) {
	NS_LOG_FUNCTION(this);
}

Ptr<AntennaModel> LifiSpectrumPhy::GetAntennaModel() {
	NS_LOG_FUNCTION(this);
	return 0;
}

void LifiSpectrumPhy::SetAntennaModel(Ptr<AntennaModel> antenna) {
	NS_LOG_FUNCTION(this);
}

void LifiSpectrumPhy::SetCellId(uint16_t id){
	NS_LOG_FUNCTION(this);
	m_cellId = id;
}

uint16_t LifiSpectrumPhy::GetCellId(){
	NS_LOG_FUNCTION(this);
	return m_cellId;
}

void LifiSpectrumPhy::SetSpectrumSignalParameters (Ptr<LifiSpectrumSignalParameters> param){
	NS_LOG_FUNCTION(this);
	m_SignalParameters = param;
}

Ptr<LifiSpectrumSignalParameters> LifiSpectrumPhy::GetSpectrumSignalParameters(){
	NS_LOG_FUNCTION(this);
	return m_SignalParameters;
}
void LifiSpectrumPhy::EndRx(Ptr<SpectrumSignalParameters> params) {
	NS_LOG_FUNCTION(this);
}

void LifiSpectrumPhy::SetErrorModel(Ptr<LifiSpectrumErrorModel> e){
	NS_LOG_FUNCTION(this);
	if(m_interference){
		m_interference->SetErrorModel(e);
	}
}

} /* namespace ns3 */
