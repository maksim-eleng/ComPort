#include "Nmea.h"
#include <iostream>


/************************************************************/
bool NMEA::checkCfgEEPROM(nmeaCfgEEPROM_t& cfgEEPROM, uint8_t unprogValue)
{
	for (auto& chCfg : cfgEEPROM.chCfg) {
		for (auto& cmd : chCfg.cmdPermits) {
			if (cmd != unprogValue)
				return true;
		}
		if (chCfg.BautRate != unprogValue || chCfg.TimeInterval != unprogValue ||
				chCfg.TIDPriorityPermis != unprogValue ||
				chCfg.ioUsedForUART.in != unprogValue || chCfg.ioUsedForUART.out != unprogValue) {
			return true;
		}
	}
	if (cfgEEPROM.numOfTerminalChannel != unprogValue) {
		return true;
	}
	return false;
}

/**
 * @brief 
 * @param cfg 
 * @return 
*/

NMEA::NMEA(nmeaCfgEEPROM_t& nmeaCfgEEPROM, std::vector<ComPort>& com, TimeBase& sysClk)
	:m_com(com)
{
	// check setting in EEPROM and config as default if nessessary
	if (checkCfgEEPROM(nmeaCfgEEPROM, 0x00) == false || checkCfgEEPROM(nmeaCfgEEPROM, 0xFF) == false) {
		setCfgDefault(nmeaCfgEEPROM);
	}
	// config NMEA cfg 
	for (int ch = 0; ch < SysConst::maxUARTChannel; ++ch) {
		chFlags_t& chFl = m_flags[ch];
		for (int cmd = NO; cmd < MAX_NMEACmdStrIndex; cmd++) {
			chFl.cmd[cmd].permits = (cmdPermits_t)nmeaCfgEEPROM.chCfg[ch].cmdPermits[cmd];
			chFl.cmd[cmd].counter = TIMEOUT;
		} // end for cmd
		chFl.BautRate = nmeaCfgEEPROM.chCfg[ch].BautRate;
		chFl.TimeInterval = nmeaCfgEEPROM.chCfg[ch].TimeInterval;
		chFl.TIDPriorityPermis = nmeaCfgEEPROM.chCfg[ch].TIDPriorityPermis;
		chFl.ioUsedForUART.in = nmeaCfgEEPROM.chCfg[ch].ioUsedForUART.in;
		chFl.ioUsedForUART.out = nmeaCfgEEPROM.chCfg[ch].ioUsedForUART.in;
		//	nmeaFl[ch].PermisTx = 0;
		//	nmeaFl[ch].ChechSumErrFlag = Off;
	}
	m_numOfTerminalChannel = nmeaCfgEEPROM.numOfTerminalChannel;

	sysClk.addObserver(*this, sysClk.EVT_1S);

	// Creates of ports
	comCfg_t cfg;
	cfg.byteSize = 8;
	cfg.parity = ComPort::P_NO;
	cfg.stopBits = ComPort::SBIT_ONE;
	cfg.evtChar = '\n';
	unsigned ch = 0;
	unsigned startComNum = 0;
	 
	m_com.reserve(SysConst::maxUARTChannel);
	for (unsigned i=0; i < SysConst::maxUARTChannel; ++i) {
		comEvtMsk_t events;
		m_com.emplace_back(sysClk);
		events = m_com[ch].openFirstFree(startComNum, m_flags[i].BautRate);
		if (ComPort::EVT_NO == events) {
			events = m_com[ch].setParam(cfg);
			m_com[ch].addObserver(*this, ComPort::EVT_RX_USER_CHAR);
			++ch;
		}
		else {
			m_com.pop_back();
			break;
		}
	}
	m_com.shrink_to_fit();
}

/************************************************************/
void NMEA::setCfgDefault(nmeaCfgEEPROM_t& cfgEEPROM, char ch)
{
	nmeaCfgEEPROM_t::CFG_CHANNEL_EEPROM_STRUCT& chCfg = cfgEEPROM.chCfg[ch];
	bool isTerminalCh = cfgEEPROM.numOfTerminalChannel == ch;
	// set baud 38400 for terminal channel and 4800 for another
	if (isTerminalCh) 
		//chCfg.BautRate = ComPort::B_38400;
		chCfg.BautRate = ComPort::B_4800; // временно
	else 
		chCfg.BautRate = ComPort::B_4800;

	chCfg.TimeInterval = T_1S;
	chCfg.TIDPriorityPermis = On;
	chCfg.ioUsedForUART.in = On;	// used as uart
	chCfg.ioUsedForUART.out = On;	// used as uart
	// permis in, out only for all command for terminal channel
	// and in only for rest
	for (int cmd = NO; cmd < MAX_NMEACmdStrIndex; ++cmd) {
		if (!isTerminalCh)
			chCfg.cmdPermits[cmd] = In_E;
		else
			chCfg.cmdPermits[cmd] = (cmdPermitsEEPROM_t)(In_E | Out_E);
	}
}

/************************************************************/
void NMEA::setCfgDefault(nmeaCfgEEPROM_t& cfgEEPROM)
{
	cfgEEPROM.numOfTerminalChannel = 0;
	for (int ch = 0; ch < SysConst::maxUARTChannel; ++ch) {
		setCfgDefault(cfgEEPROM, ch);
	}
}

bool NMEA::checkCmdPermits(chFlags_t& chFl, cmdName_t cmd, cmdPermits_t permitMsk)
{
	return chFl.cmd[cmd].permits & permitMsk;
}

//void NMEA::setPermits(const cmdName_t& cmd, const cmdPermit_t& permitMsk, const uint8_t ch)
//{
//	auto tmp = m_nmeaFl.chCfg[ch].cmd[cmd].permit | permitMsk;
//	m_nmeaFl.chCfg[ch].cmd[cmd].permit = (cmdPermit_t)tmp;
//}
//
//void NMEA::resetPermits(const cmdName_t& cmd, const cmdPermit_t& permitMsk, const uint8_t ch)
//{
//	auto tmp = m_nmeaFl.chCfg[ch].cmd[cmd].permit & ~permitMsk;
//	m_nmeaFl.chCfg[ch].cmd[cmd].permit = (cmdPermit_t)tmp;
//}

void NMEA::formingGGAStr()
{
	//count++;
}

void NMEA::formingGNSStr()
{
	//count--;
}

void NMEA::formingGLLStr()
{
}

void NMEA::formingRMCStr()
{
}

void NMEA::formingVTGStr()
{
}

void NMEA::formingZDAStr()
{
}

void NMEA::formingDTMStr()
{
}

void NMEA::formingHDTStr()
{
}

void NMEA::formingHDMStr()
{
}

void NMEA::formingVBWStr()
{
}

void NMEA::formingVHWStr()
{
}

void NMEA::formingStr()
{
}


/**************************************************************/
void NMEA::handleEvent(TimeBase& ref, TimeBase::tBaseEvtMsk_t evtMsk)
{
	if (evtMsk & ref.EVT_1S) {
		m_com[m_numOfTerminalChannel].print("$GPRMC,101530.32,A,2726.68,S,15307.56,E,012,032,050620,11,E*63\r\n");
		m_com[m_numOfTerminalChannel].print("$GPGGA,101525.83,2726.68,S,15307.56,E,1,4,002,+15,M,046,M,,*71\r\n");
		m_com[m_numOfTerminalChannel].print("$GPGNS,104757.19,2726.68,S,15307.56,E,,6,002,15,046,,*73\r\n");
	}
}

/**************************************************************/
void NMEA::handleEvent(ComPort& com, comEvtMsk_t evtMask)
{
	// 
	if (evtMask & com.EVT_RX_USER_CHAR) {
		while (com.getUserCharReceivedCounter()) {
			char str[SysConst::nmeaMaxStrLen];
			com.getRxStr(str, sizeof(str));
			std::cout << str;
		}
	}
}