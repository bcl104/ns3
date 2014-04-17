 /* lifi-spectrum-phy.cc*/

#include "lifi-spectrum-phy.h"

namespace ns3 {

LifiSpectrumPhy::LifiSpectrumPhy() {
	m_band = 0;
}

LifiSpectrumPhy::LifiSpectrumPhy(Ptr<NetDevice> device) {
	LifiSpectrumPhy();
	m_device = device;
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

void LifiSpectrumPhy::Send(Ptr<PacketBurst> pb, uint32_t size, uint8_t band,  bool isCellMode, uint8_t cellId , uint8_t trxid) {
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

void LifiSpectrumPhy::EndRx(Ptr<SpectrumSignalParameters> params) {
}

void LifiSpectrumPhy::SetErrorModel(Ptr<LifiSpectrumErrorModel> e){
	if(m_interference){
		m_interference->SetErrorModel(e);
	}
}

} /* namespace ns3 */
