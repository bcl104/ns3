/*
aaaa b * lifi-phy.cc
 *
 *  Created on: 2014年4月4日
 *      Author: will
 */

#include "lifi-phy.h"
#include "lifi-net-device.h"
//#include "lifi-spectrum-channel.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE("LifiPhy");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(LifiPhy);

//class LifiSpectrumChannel;
//struct LifiSpectrumSignalParameters;

LifiPhy::LifiPhy() {
	NS_LOG_FUNCTION(this);
	NS_FATAL_ERROR ("This constructor should not be called");
	m_cellMode = false;
	m_trxid = 1;
	m_cellId = 0;
	m_trxStatus = TRX_OFF;
	m_csTh = 1.0;
	m_edTh = 1.0;
	m_trxMode = MULTIPLEX;
	m_txPower = 1;
	m_duration = Seconds(1);
	m_band = 1;
	m_burstMode = false;
	m_mcsId = 0x00;
	m_ookDim = false;
	m_Time = Simulator::Now();
	m_reservedFields = 0x00;
	m_PsduSize = 0;
	m_subBandsNum = 1;
	m_opticClock = NanoSeconds(5000);
	m_opticClockHz = 0;
	m_plmeSapProvider = Create<PlmeSpecificSapProvider<LifiPhy> > (this);
	m_pdSapProvider = Create<PdSpecificSapProvider<LifiPhy> > (this);
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
	m_band = 1;
	m_duration = Seconds(1);
	m_trxStatus = TRX_OFF;
	m_pdSapProvider = Create<PdSpecificSapProvider<LifiPhy> > (this);
	m_plmeSapProvider = Create<PlmeSpecificSapProvider<LifiPhy> > (this);
	m_spectrumPhy = spectrum;
	m_burstMode = false;
	m_mcsId = 0x00;
	m_ookDim = false;
	m_reservedFields = 0x00;
	m_PsduSize = 0;
	m_Time = Simulator::Now();
	m_subBandsNum = 1;
	m_opticClock = NanoSeconds(5000);
}

LifiPhy::LifiPhy(LifiPhyPibAttribute attributes, Ptr<LifiSpectrumPhy> spectrum) {
	NS_LOG_FUNCTION(this);
	m_csTh = 1.0;
	m_edTh = 1.0;
	m_cellMode = false;
	m_trxMode = MULTIPLEX;
	m_txPower = 1;
	m_band = 1;
	m_duration = Seconds(1);
	m_trxStatus = TRX_OFF;
	m_pdSapProvider = Create<PdSpecificSapProvider<LifiPhy> > (this);
	m_plmeSapProvider = Create<PlmeSpecificSapProvider<LifiPhy> > (this);
	m_attributes = attributes;
	m_spectrumPhy = spectrum;
	m_burstMode = false;
	m_mcsId = 0x00;
	m_reservedFields = 0x00;
	m_PsduSize = 0;
	m_ookDim = false;
	m_subBandsNum = 1;
}

LifiPhy::LifiPhy(std::vector< Ptr<LifiCell> > cellList){
	NS_LOG_FUNCTION(this);
	m_csTh = 1.0;
	m_edTh = 1.0;
	m_band = 1;
	m_cellMode = false;
	m_trxMode = MULTIPLEX;
	m_txPower = 1;
	m_duration = Seconds(1);
	m_trxStatus = TRX_OFF;
	m_pdSapProvider = Create<PdSpecificSapProvider<LifiPhy> > (this);
	m_plmeSapProvider = Create<PlmeSpecificSapProvider<LifiPhy> > (this);
	m_cellList = cellList;
	m_burstMode = false;
	m_mcsId = 0x00;
	m_PsduSize = 0;
	m_ookDim = false;
	m_reservedFields = 0x00;
	m_subBandsNum = 1;
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
	NS_ASSERT(band < 7);
	NS_ASSERT(m_trxStatus != TX_BUSY);
	bool ccaResult = m_spectrumPhy->CarrierSense(band,m_edTh);
	std::cout<<"cca result:"<<ccaResult<<std::endl;
	uint8_t result = 0;
	if(ccaResult == true){
		result = 1 << band;
	}
	return result;
}

//void LifiPhy::SetDeviceAttribute(PhyOpStatus status) {
//	NS_LOG_FUNCTION(this);
//
//}

void LifiPhy::SetOokDim(bool dim){
	NS_LOG_FUNCTION(this);
	m_ookDim = dim;
}

bool LifiPhy::GetOokDim(void){
	NS_LOG_FUNCTION(this);
	return m_ookDim;
}

void LifiPhy::SetBurstMode(bool burst){
	NS_LOG_FUNCTION(this);
	m_burstMode = burst;
}

bool LifiPhy::GetBurstMode(void){
	NS_LOG_FUNCTION(this);
	return m_burstMode;
}

void LifiPhy::SetMcsId(uint8_t mcsid){
	NS_LOG_FUNCTION(this);
	m_mcsId = mcsid;
	m_opticClockHz = SearchOpticClock(mcsid);
	m_opticClock = NanoSeconds(SearchOpticClock(mcsid));
	m_attributes.m_mcsid = mcsid;
}

const double* LifiPhy::GetOpticClockHz(void){
	NS_LOG_FUNCTION(this);
	return &m_opticClockHz;
}

uint8_t LifiPhy::GetMcsId(void){
	NS_LOG_FUNCTION(this);
	return m_mcsId;
}

void LifiPhy::SetReservedFields(uint8_t fields){
	NS_LOG_FUNCTION(this);
	m_reservedFields = fields;

}

uint8_t LifiPhy::GetReservedFields(void){
	NS_LOG_FUNCTION(this);
	return m_reservedFields;
}

void LifiPhy::Transmit(uint32_t size, Ptr<Packet> pb, uint8_t band) {
	NS_LOG_FUNCTION(this);
	if(m_trxStatus == TX_ON){
		m_trxStatus = TX_BUSY;
		m_PlmeSapUser->PlemStateIndication(m_trxStatus);
		uint8_t* channel = (uint8_t*)(m_attributes.GetAttributes(PHY_CURRENT_CHANNEL));
		m_band = band;
//		double fb,fe,fc;
//		GetbandsInfo(fb,fe,fc,m_band);
		Bands psdBand;
//		if(m_band == 7){
//		psdBand = GetFullBands();
//		}
//		else{
//		psdBand = GetBands(fb,fc,fe);
//		}
		psdBand = GetFullBands();
		m_psd = CalculatetxPsd(m_txPower,psdBand,m_band,0)->Copy();//modulation way is not used yet.
		double headerrate = GetHeaderRate(m_mcsId);
		LifiPhyHeader header = SetLifiPhyHeader(m_burstMode,*channel,m_mcsId,size,m_ookDim,0x00);
		pb->AddHeader(header);
//		DynamicCast<LifiSpectrumChannel>(m_spectrumPhy->GetChannel())->AddTx(m_spectrumPhy);
		double rate = GetRate (m_mcsId);
		m_duration = Seconds(header.GetSerializedSize()/headerrate + pb->GetSize()/(1000*rate));
		StartTx (pb);
	}
	else
		EndTx (m_trxStatus);
}

void LifiPhy::Receive(Ptr<LifiSpectrumSignalParameters> param,uint8_t wqi) {
	NS_LOG_FUNCTION(this);
	Ptr<Packet> pb = param->pb;
//	uint8_t txChannel = param->band;
//	Ptr<LifiSpectrumPhy> txSpectrumPhy = DynamicCast<LifiSpectrumPhy>(param->txPhy);
//	Ptr<LifiNetDevice> txDevice = DynamicCast<LifiNetDevice>(txSpectrumPhy->GetDevice());
//	Ptr<LifiPhy> txPhy = txDevice->GetPhy();
//	bool txBurstMode = param->burstMode;
//	bool txOokdim = param->ookDim;
//	uint8_t txMcsid = param->mcsId;
//	uint32_t size = param->PsduSize;
//	LifiPhyHeader header = SetLifiPhyHeader(txBurstMode,txChannel,txMcsid,size,txOokdim,0x00);
	LifiPhyHeader header;
	pb->RemoveHeader(header);
	uint32_t size = pb->GetSize();
	m_pdSapUser->PdDataIndication(size,pb,0);
	uint8_t *buffer=new uint8_t;
//////	*buffer=56;
	std::cout<<"receive packet size:"<<size<<std::endl;
	pb->CopyData(buffer,size);
	std::cout<<"receive buffer:"<<(int)*buffer<<std::endl;
//	std::ofstream oso("abcd.txt");
//	pb->Print(oso);
//	std::cout<<pb<<std::endl;
	m_trxStatus = RX_ON;
	m_PlmeSapUser->PlemStateIndication(m_trxStatus);

}

PhyOpStatus LifiPhy::SetTRxState(PhyOpStatus state) {
	NS_LOG_FUNCTION(this);
//	m_trxStatus = state;
	PhyOpStatus tempState = m_trxStatus;
	if(tempState == RX_BUSY){
		switch (state){
//		case RX_ON :{
//			m_PlmeSapUser->PlemStateIndication(RX_ON_BUSY);
//			break;
//		}
		case FORCE_TRX_OFF:{
			m_PlmeSapUser->PlemStateIndication(PHY_SUCCESS);
			m_trxStatus = TRX_OFF;
//			DynamicCast<LifiSpectrumChannel>(m_spectrumPhy->GetChannel())->DeleteRx(m_spectrumPhy);
//			DynamicCast<LifiSpectrumChannel>(m_spectrumPhy->GetChannel())->DeleteTx(m_spectrumPhy);
			 break;
		}
		default :{
			m_PlmeSapUser->PlemStateIndication(RX_BUSY);
			break;
		}
		}
	}
	else if(tempState == TX_BUSY){
		switch(state){
//			case TX_ON:{
//				m_PlmeSapUser->PlemStateIndication(TX_ON_BUSY);
//				break;
//			}
			case FORCE_TRX_OFF:{
						m_PlmeSapUser->PlemStateIndication(PHY_SUCCESS);
						m_trxStatus = TRX_OFF;
//						DynamicCast<LifiSpectrumChannel>(m_spectrumPhy->GetChannel())->DeleteRx(m_spectrumPhy);
//						DynamicCast<LifiSpectrumChannel>(m_spectrumPhy->GetChannel())->DeleteTx(m_spectrumPhy);
						 break;
			}
			default :{
						m_PlmeSapUser->PlemStateIndication(TX_BUSY);
						break;
			}
		}
	}
	else if(tempState == RX_ON){
		switch(state){
		case RX_ON:{
			m_trxStatus = RX_ON;
			m_PlmeSapUser->PlemStateIndication(PHY_SUCCESS);
//			m_spectrumPhy->GetChannel()->AddRx(m_spectrumPhy);
			break;
			}
		case TRX_OFF:{
			m_trxStatus = state;
			m_PlmeSapUser->PlemStateIndication(PHY_SUCCESS);
			DynamicCast<LifiSpectrumChannel>(m_spectrumPhy->GetChannel())->DeleteRx(m_spectrumPhy);
			break;
		}
		case TX_ON:{
			m_trxStatus = TX_ON;
			m_PlmeSapUser->PlemStateIndication(PHY_SUCCESS);
			break;
		}
		case FORCE_TRX_OFF:{
			m_trxStatus = TRX_OFF;
			m_PlmeSapUser->PlemStateIndication(PHY_SUCCESS);
//			DynamicCast<LifiSpectrumChannel>(m_spectrumPhy->GetChannel())->DeleteRx(m_spectrumPhy);
//			DynamicCast<LifiSpectrumChannel>(m_spectrumPhy->GetChannel())->DeleteTx(m_spectrumPhy);
			break;
		}
		case RX_BUSY:{
			m_trxStatus = RX_BUSY;
			m_PlmeSapUser->PlemStateIndication(PHY_SUCCESS);
			break;
		}
		default:{
			NS_LOG_WARN("illegality TRxstate!");
		}
		}
	}
	else {
		switch(state){
		case RX_ON:{
			m_trxStatus = RX_ON;
			m_PlmeSapUser->PlemStateIndication(PHY_SUCCESS);
			m_spectrumPhy->GetSpectrumSignalParameters()->band = 7;
			m_spectrumPhy->GetChannel()->AddRx(m_spectrumPhy);
			break;
			}
		case TRX_OFF:{
			m_trxStatus = state;
			m_PlmeSapUser->PlemStateIndication(PHY_SUCCESS);
			DynamicCast<LifiSpectrumChannel>(m_spectrumPhy->GetChannel())->DeleteRx(m_spectrumPhy);
			break;
		}
		case TX_ON:{
			m_trxStatus = TX_ON;
			m_PlmeSapUser->PlemStateIndication(PHY_SUCCESS);
			break;
		}
		case TX_BUSY:{
			m_trxStatus = TX_BUSY;
			m_PlmeSapUser->PlemStateIndication(PHY_SUCCESS);
			break;
		}
		case FORCE_TRX_OFF:{
			m_trxStatus = TRX_OFF;
			m_PlmeSapUser->PlemStateIndication(PHY_SUCCESS);
//			DynamicCast<LifiSpectrumChannel>(m_spectrumPhy->GetChannel())->DeleteRx(m_spectrumPhy);
//			DynamicCast<LifiSpectrumChannel>(m_spectrumPhy->GetChannel())->DeleteTx(m_spectrumPhy);
			break;
		}
		default:{
			NS_LOG_WARN("illegality TRxstate!");
		}
		}
	}
//	 if(state == RX_ON){
////			DynamicCast<LifiSpectrumChannel>(m_spectrumPhy->GetChannel())->AddRx(m_band,m_spectrumPhy);
//		m_trxStatus = state;
//		m_spectrumPhy->GetChannel()->AddRx(m_spectrumPhy);
//	}
//		else if(state == TRX_OFF){
//			m_trxStatus = state;
//			DynamicCast<LifiSpectrumChannel>(m_spectrumPhy->GetChannel())->DeleteRx(m_spectrumPhy);
////			DynamicCast<LifiSpectrumChannel>(m_spectrumPhy->GetChannel())->DeleteTx(m_band,m_spectrumPhy);
//		}
//		else if(state == DEFAULT){
////	 		return tempState;
//	 	}
//		else if(state == FORCE_TRX_OFF){
//			m_trxStatus = state;
//		}
//		else{
//			NS_LOG_WARN("illegality TRxstate!");
//		}
	 return tempState;
}

Ptr<PlmeSapUser> LifiPhy::GetPlmeSapUser(){
	NS_LOG_FUNCTION(this);
	return m_PlmeSapUser;
}

void LifiPhy::SetTxPower(double txPower){
	NS_LOG_FUNCTION(this);
	m_txPower = txPower;
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

uint8_t LifiPhy::GetbandId(){
	NS_LOG_FUNCTION(this);
	return m_band;
}

void LifiPhy::SetbandId(uint8_t band){
	NS_LOG_FUNCTION(this);
	m_band = band;
}

void LifiPhy::StartTx(Ptr<Packet> pb) {
	NS_LOG_FUNCTION(this);
	//	m_spectrumPhy->Send()
	if(!m_cellMode){
		m_spectrumPhy->Send(pb,pb->GetSize(),m_band,m_cellMode,m_cellId,m_trxid,m_txPower,m_duration,m_psd->Copy(),m_Time);
//		m_endTxEvent = Simulator::Schedule (duration, &LifiPhy::EndTx, PHY_SUCCESS);
		m_trxStatus = TX_BUSY;
		m_PlmeSapUser->PlemStateIndication(m_trxStatus);
		Simulator::Schedule (m_duration, &LifiPhy::EndTx, this,  PHY_SUCCESS);
	}
	else{

	}
}

void LifiPhy::EndTx(PhyOpStatus trxStatus) {
	NS_LOG_FUNCTION(this);
	if(trxStatus == PHY_SUCCESS){
//		SetTRxState(TRX_OFF);
		m_trxStatus = TX_ON;
		m_PlmeSapUser->PlemStateIndication(m_trxStatus);
		DynamicCast<LifiSpectrumChannel>(m_spectrumPhy->GetChannel())->DeleteTx(m_spectrumPhy);
		m_pdSapUser->PdDataConfirm (trxStatus);
	}
	else if(trxStatus == RX_ON || trxStatus == RX_BUSY)
		m_pdSapUser->PdDataConfirm (RX_ON);
	else if(trxStatus == TRX_OFF)
		m_pdSapUser->PdDataConfirm (TRX_OFF);
	else{
		m_pdSapUser->PdDataConfirm (TX_BUSY);
	}
}

void LifiPhy::Switch(bool** sw_bit_map, bool dir) {
	NS_LOG_FUNCTION(this);
}

void LifiPhy::SetTxTime(Time time){
	NS_LOG_FUNCTION(this);
	m_Time = time;
}

Time LifiPhy::GetTxTime(void){
	NS_LOG_FUNCTION(this);
	return m_Time;
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
/*
 * unit:kb/s
 */
double LifiPhy::GetRate(uint8_t mcsId) {
//	NS_LOG_FUNCTION(this);
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

void LifiPhy::SetSpectrumValue(Ptr<SpectrumValue> value){
	NS_LOG_FUNCTION(this);
	m_psd = value;
}

Ptr<SpectrumValue> LifiPhy::GetSpectrumValue(void){
	NS_LOG_FUNCTION(this);
	return m_psd;
}

LifiPhyHeader LifiPhy::SetLifiPhyHeader (bool isBurstMode,uint8_t channelNum,uint8_t mcsId,uint16_t psduLength,bool ookDimmed,uint8_t reservedFields){
	NS_LOG_FUNCTION(this);
	LifiPhyHeader header;
	header.SetBurstMode(isBurstMode);
	header.SetChannelNum(channelNum);
	header.SetMcsId(mcsId);
	header.SetOokDimmed(ookDimmed);
	header.SetPsduLength(psduLength);
	header.SetReservedFields(reservedFields);
	return header;
}
//LifiPhyHeader LifiPhy::GetLifiPhyHeader(){
//	NS_LOG_FUNCTION(this);
//	return m_phyheader;
//}


Ptr<SpectrumValue> LifiPhy::CalculatetxPsd(double txPowerDbm,Bands band,uint8_t bandid,uint8_t Modulation){
	NS_LOG_FUNCTION(this);
	NS_ASSERT(bandid < 7);
	Ptr<SpectrumModel> model = Create<SpectrumModel>(band);
	Ptr<SpectrumValue> txPsd = Create<SpectrumValue>(model);
//	double fb,fe,fc;
//	GetbandsInfo(fb,fe,fc,bandid);
	double powerTxW = std::pow (10., (txPowerDbm - 30) / 10);//turn the unit Dbm to w.
//	double bandwith = (--(band.end()))->fh - band.begin()->fl;
	double bandwith = (band.begin()+(7-bandid)*m_subBandsNum )->fl-(band.begin()+(6-bandid)*m_subBandsNum )->fl;//MHZ
//	double bandwith = 0.72059e8;
	double txPowerDensity = powerTxW / bandwith;
//	std::cout<<"txPower:"<<txPowerDensity<<std::endl;
	//judge modulation way
	NS_ASSERT(bandid < 7);
	Values::iterator beg = txPsd->ValuesBegin()+(double)(6-bandid)*m_subBandsNum;
//	Values::iterator end = txPsd->ValuesEnd();
	Values::iterator end =beg + m_subBandsNum;
	while(beg != end){
		*beg = txPowerDensity;
		beg++;
	}
//	Values::iterator testbeg = txPsd->ValuesBegin();
//	Values::iterator testend = txPsd->ValuesEnd();
//	while(testbeg!=testend){
//		std::cout<<*testbeg<<std::endl;
//		testbeg++;
//	}
//	std::cout<<"m_uid:"<<model->GetUid()<<std::endl;
	return txPsd;
}

void LifiPhy::SetSubBandsNum(uint8_t num){
	NS_LOG_FUNCTION(this);
	m_subBandsNum = num;
}

uint8_t LifiPhy::GetSunBandsNum(void){
	NS_LOG_FUNCTION(this);
	return m_subBandsNum;
}

Bands LifiPhy::GetBands(double fb,double f0,double fe){
	NS_LOG_FUNCTION(this);
	double resolution = (fe-fb)/m_subBandsNum;
	Bands band;
	for(double i = 0;i<m_subBandsNum;i++){
		BandInfo info;
		info.fl = fb+resolution*i;
		info.fh = fb+resolution*(i+1);
		info.fc = info.fl+resolution/2;
		band.push_back(info);
	}
	return band;
}

Bands LifiPhy::GetFullBands(void){
	NS_LOG_FUNCTION(this);
	Bands allBand;
	for(uint8_t i = 7;i > 0;i--){
		double fb,fc,fe;
		GetbandsInfo(fb,fc,fe,i-1);
		Bands band = GetBands(fb,fc,fe);
		allBand.insert(allBand.end(),band.begin(),band.end());
	}
	return allBand;
}
/*fb fc and fe unit:MHZ
 * f = c/l f:hz,c:3.0x10^8m/s,l:nm
 * f = (3000/l)*10^8MHZ
 */
void LifiPhy::GetbandsInfo(double &fb,double &fc,double &fe,uint8_t band){
	NS_LOG_FUNCTION(this);
	double templ;
	double temph;
	switch(band){
	case 0 :    temph = (double)3000/380;
				templ = (double)3000/478;
				fb = templ*1.0e8;
				fe = temph*1.0e8;
				fc = (fe-fb)/2.0;
				break;
	case 1 :	temph = (double)3000/478;
				templ = (double)3000/540;
				fb = templ*1.0e8;
				fe = temph*1.0e8;
				fc = (fe-fb)/2.0;
				break;
	case 2 :	temph = (double)3000/540;
				templ = (double)3000/588;
				fb = templ*1.0e8;
				fe = temph*1.0e8;
				fc = (fe-fb)/2.0;
				break;
	case 3 :	temph = (double)3000/588;
				templ = (double)3000/633;
				fb = templ*1.0e8;
				fe = temph*1.0e8;
				fc = (fe-fb)/2.0;
				break;
	case 4 :	temph = (double)3000/633;
				templ = (double)3000/679;
				fb = templ*1.0e8;
				fe = temph*1.0e8;
				fc = (fe-fb)/2.0;
				break;
	case 5 :	temph = (double)3000/679;
				templ = (double)3000/726;
				fb = templ*1.0e8;
				fe = temph*1.0e8;
				fc = (fe-fb)/2.0;
				break;
	case 6 :	temph = (double)3000/726;
				templ = (double)3000/780;
				fb = templ*1.0e8;
				fe = temph*1.0e8;
				fc = (fe-fb)/2.0;
				break;
	case 7 :	temph = (double)3000/478;
				templ = (double)3000/780;
				fb = templ*1.0e8;
				fe = temph*1.0e8;
				fc = (fe-fb)/2.0;
				break;
	default:NS_LOG_WARN("the bandId:"<<band<<"is not using");
			break;
	}
}
/*
 * 2014 04 25 10:13
 * byst125475466
 */

const Time* LifiPhy::GetOpticClock(void){
	NS_LOG_FUNCTION(this);
	return &m_opticClock;
}
/*
 * 2014 04 25 10:13
 * byst125475466
 * unit:hz
 */
double LifiPhy::SearchOpticClock(uint8_t mcsid){
	NS_LOG_FUNCTION(this);
	switch(mcsid){
		/*PHY I*/
		case 0:
		case 1:
		case 2:
		case 4:
			return 2.0e5;break;
		case 5:
		case 6:
		case 7:
		case 8:
			return 4.0e5;break;
		/*PHY II*/
		case 16:
		case 17:
			return 3.75e6;break;
		case 18:
		case 19:
		case 20:
			return 7.5e6;break;
		case 21:
		case 22:
			return 15.0e6;break;
		case 23:
		case 24:
			return 30.0e6;break;
		case 25:
		case 26:
			return 60.0e6;break;
		case 27:
		case 28:
		case 29:
			return 120.0e6;break;
		/*PHY III*/
		case 32:
		case 33:
			return 12.0e6;break;
		case 34:
		case 35:
		case 36:
		case 37:
		default:
			return 24.0e6;break;
		}
	NS_LOG_WARN("mcsid:"<<mcsid<<" is not supported ");
	return 0;
}
/*
 * 2014 04 25 10:43
 * byst125475466
 * unit:b/s
 */
double LifiPhy::GetHeaderRate(uint8_t mcsid){
	NS_LOG_FUNCTION(this);
		switch(mcsid){
			/*PHY I*/
			case 0:
			case 1:
			case 2:
			case 4:
				return 11.67e3;break;
			case 5:
			case 6:
			case 7:
			case 8:
				return 35.56e3;break;
			/*PHY II*/
			case 16:
			case 17:
				return 1.25e6;break;
			case 18:
			case 19:
			case 20:
				return 2.5e6;break;
			case 21:
			case 22:
				return 6.0e6;break;
			case 23:
			case 24:
				return 12.0e6;break;
			case 25:
			case 26:
				return 24.0e6;break;
			case 27:
			case 28:
			case 29:
				return 48.0e6;break;
			/*PHY III*/
			case 32:
			case 33:
				return 12.0e6;break;
			case 34:
			case 35:
			case 36:
			case 37:
			default:
				return 24.0e6;break;
			}
		NS_LOG_WARN("mcsid:"<<mcsid<<" is not supported ");
		return 0;
}

} /* namespace ns3 */

