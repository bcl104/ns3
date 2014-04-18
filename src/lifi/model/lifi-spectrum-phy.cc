 /* lifi-spectrum-phy.cc*/

#include "lifi-spectrum-phy.h"

namespace ns3 {

LifiSpectrumPhy::LifiSpectrumPhy() {
	m_band = 0;
	m_cellId = 0;
}

LifiSpectrumPhy::LifiSpectrumPhy(Ptr<NetDevice> device) {
	LifiSpectrumPhy();
	m_device = device;
	m_cellId = 0;
	m_band = 0;
}

LifiSpectrumPhy::~LifiSpectrumPhy() {
}

TypeId LifiSpectrumPhy::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiSpectrumPhy")
						.SetParent<SpectrumPhy>()
						.AddConstructor<LifiSpectrumPhy>();
	return tid;
}

void LifiSpectrumPhy::SetNodeSpectrum(uint8_t channel) {
}

void LifiSpectrumPhy::Send(Ptr<Packet>pb,uint32_t size,uint8_t band,  bool isCellMode,  uint8_t cellId,  uint8_t trxid , double txPower,Time duration) {
	m_SignalParameters->band = band;
	m_SignalParameters->cellId = cellId;
	m_SignalParameters->cellMode = isCellMode;
	m_SignalParameters->duration = duration;
	m_SignalParameters->pb = pb;
	m_SignalParameters->trxId = trxid;
	m_SignalParameters->txPower = txPower;
	m_channel->StartTx(m_SignalParameters);
}

bool LifiSpectrumPhy::CarrierSense() {
	return false;
}

uint8_t LifiSpectrumPhy::CarrierSense(uint8_t band) {
	return 0;
}

void LifiSpectrumPhy::SetDevice(Ptr<NetDevice> device) {
}

Ptr<NetDevice> LifiSpectrumPhy::GetDevice() {
	return 0;
}

void LifiSpectrumPhy::SetMobility(Ptr<MobilityModel> m) {
}

Ptr<MobilityModel> LifiSpectrumPhy::GetMobility() {
	return 0;
}

void LifiSpectrumPhy::SetChannel(Ptr<SpectrumChannel> c) {
	m_channel = c;
}

Ptr<LifiSpectrumChannel> LifiSpectrumPhy::GetChannel(){
	return m_channel;
}

Ptr<const SpectrumModel> LifiSpectrumPhy::GetRxSpectrumModel() const{
	return 0;
}

Ptr<AntennaModel> LifiSpectrumPhy::GetRxAntenna() {
	return 0;
}

void LifiSpectrumPhy::StartRx(Ptr<SpectrumSignalParameters> params) {
}

Ptr<AntennaModel> LifiSpectrumPhy::GetAntennaModel() {
	return 0;
}

void LifiSpectrumPhy::SetAntennaModel(Ptr<AntennaModel> antenna) {
}

void LifiSpectrumPhy::SetCellId(uint16_t id){
	m_cellId = id;
}

uint16_t LifiSpectrumPhy::GetCellId(){
	return m_cellId;
}

void LifiSpectrumPhy::SetSpectrumSignalParameters (Ptr<LifiSpectrumSignalParameters> param){
	m_SignalParameters = param;
}

Ptr<LifiSpectrumSignalParameters> LifiSpectrumPhy::GetSpectrumSignalParameters(){
	return m_SignalParameters;
}
void LifiSpectrumPhy::EndRx(Ptr<SpectrumSignalParameters> params) {
}

void LifiSpectrumPhy::SetErrorModel(Ptr<LifiSpectrumErrorModel> e){
	if(m_interference){
		m_interference->SetErrorModel(e);
	}
}

} /* namespace ns3 */
