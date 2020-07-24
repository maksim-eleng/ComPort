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
NMEA::NMEA(nmeaCfgEEPROM_t& cfgEEPROM, std::vector<ComPort>& com, TimeBase& sysClk)
	:m_com(com)
{
	// check setting in EEPROM and config as default if nessessary
	if (checkCfgEEPROM(cfgEEPROM, 0x00) == false || checkCfgEEPROM(cfgEEPROM, 0xFF) == false) {
		setCfgDefault(cfgEEPROM);
	}
	// config NMEA cfg 
	for (int ch = 0; ch < SysConst::maxUARTChannel; ++ch) {
		chFlags_t& chFl = m_flags[ch];
		for (int cmd = NO; cmd < MAX_NMEACmdStrIndex; cmd++) {
			chFl.cmd[cmd].permits = (cmdPermits_t)cfgEEPROM.chCfg[ch].cmdPermits[cmd];
			chFl.cmd[cmd].counter = TIMEOUT;
		} // end for cmd
		chFl.BautRate = cfgEEPROM.chCfg[ch].BautRate;
		chFl.TimeInterval = cfgEEPROM.chCfg[ch].TimeInterval;
		chFl.TIDPriorityPermis = cfgEEPROM.chCfg[ch].TIDPriorityPermis;
		chFl.ioUsedForUART.in = cfgEEPROM.chCfg[ch].ioUsedForUART.in;
		chFl.ioUsedForUART.out = cfgEEPROM.chCfg[ch].ioUsedForUART.in;
		//	nmeaFl[ch].PermisTx = 0;
		//	nmeaFl[ch].ChechSumErrFlag = Off;
	}
	m_numOfTerminalChannel = cfgEEPROM.numOfTerminalChannel;

	sysClk.addObserver(*this, sysClk.EVT_1S);
	// Creates of ports
	for (unsigned ch = 0; ch < SysConst::maxUARTChannel; ++ch) {
		comEvtMsk_t events;
		m_com.push_back(ComPort(sysClk));
		m_com[ch].addObserver(*this);
		events = m_com[ch].open(ch, m_flags[ch].BautRate);
		if (ComPort::EVT_NO == events) {
			comCfg_t cfg;
			cfg.byteSize = 8;
			cfg.parity = ComPort::P_NO;
			cfg.stopBits = ComPort::SBIT_ONE;
			cfg.evtChar = '\n';
			events = m_com[ch].setParam(cfg);
			if (ComPort::EVT_NO == events) {
			}
		}
		else {
//			m_com.erase(m_com.begin() + ch);
		}
	}

}

/************************************************************/
void NMEA::setCfgDefault(nmeaCfgEEPROM_t& cfgEEPROM, char ch)
{
	nmeaCfgEEPROM_t::CFG_CHANNEL_EEPROM_STRUCT& chCfg = cfgEEPROM.chCfg[ch];
	bool isTerminalCh = cfgEEPROM.numOfTerminalChannel == ch;
	// set baud 38400 for terminal channel and 4800 for another
	if (isTerminalCh) 
		//chCfg.BautRate = ComPort::B_38400;
		chCfg.BautRate = ComPort::B_4800;
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
		ComPort* terminal = &m_com[m_numOfTerminalChannel +1];
		std::cout << '\n' << ref.getTime() << '\t' << ref.getDate() << '\n';
		terminal->print("$GPRMC,101530.32,A,2726.68,S,15307.56,E,012,032,050620,11,E*63\r\n");
		terminal->print("$GPGGA,101525.83,2726.68,S,15307.56,E,1,4,002,+15,M,046,M,,*71\r\n");
		terminal->print("$GPGNS,104757.19,2726.68,S,15307.56,E,,6,002,15,046,,*73\r\n");
	}
}

/**************************************************************/
void NMEA::handleEvent(ComPort& com, comEvtMsk_t evtMask)
{
	// 
	if (evtMask & com.EVT_RX_USER_CHAR) {
		while (com.userCharGetReceivedCounter()) {
			char str[SysConst::nmeaMaxStrLen];
			com.getRxStr(str, sizeof(str));
			std::cout << str;
			com.userCharHandled();
		}
	}
}