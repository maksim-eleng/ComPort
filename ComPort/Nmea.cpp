#include "Nmea.h"

/**
 * @brief 
 * @param cfg 
 * @return 
*/
NMEA::NMEA(nmeaCfgEEPROM_t& cfgExternal)
{
	/*******************************************************/
// Инициализация переменных для NMEA в памяти, EEPROM
// настройка буфера под работу NMEA
// При начальной инициализации нового контроллера в EEPROM заносим TimeInterval=T_1S, UARTBautRate=B_4800.
// Запрет конвертации для команд, для которых не может быть конвертации
// Разрешение входа для команды PRG
// Формирование Cmd как будто нет команд. Остальные поля - с EEPROM
// Инициализация указателей на ф-ии формирования соответствующими адресами
// Инициализация NMEAFl
// Инициализация массива структур TID, как будто Talker нет нигде (TIMEOUT)
// Инициализация буферов приема/передачи
// Настройка необходимого железа
/*******************************************************/
	for (int ch = 0; ch < SysConst::UARTChannel; ch++) {
		m_nmeaFl.chCfg[ch].cmd[GGA].permit = In;

		cfgExternal.chCfg[ch].cmd[GGA] = In | Out;
		// инициализация структуры NMEAFl
		nmeaFl[ch].StateFrame = NoFrame;
		nmeaFl[ch].Command = NO;
		nmeaFl[ch].DelayedCounter = 0;
		nmeaFl[ch].NewStrInPrg = 0;
		nmeaFl[ch].PermisTx = 0;
		nmeaFl[ch].ChechSumErrFlag = Off;
		nmeaFl[ch].TimeInterval = eeSet.nmea[ch].TimeInterval;
		nmeaFl[ch].CmdPoint = nmeaCmd[ch];
		// заносим указатели на функции для формирвоания строки
		nmeaFl[ch].CmdPoint[GGA].FormingStr = nmeaFormingGGAStr;
		nmeaFl[ch].CmdPoint[GNS].FormingStr = nmeaFormingGNSStr;
		nmeaFl[ch].CmdPoint[GLL].FormingStr = nmeaFormingGLLStr;
		nmeaFl[ch].CmdPoint[RMC].FormingStr = nmeaFormingRMCStr;
		nmeaFl[ch].CmdPoint[VTG].FormingStr = nmeaFormingVTGStr;
		nmeaFl[ch].CmdPoint[ZDA].FormingStr = nmeaFormingZDAStr;
		nmeaFl[ch].CmdPoint[DTM].FormingStr = nmeaFormingDTMStr;
		nmeaFl[ch].CmdPoint[HDT].FormingStr = nmeaFormingHDTStr;
		nmeaFl[ch].CmdPoint[HDG].FormingStr = nmeaFormingNullFunction;
		nmeaFl[ch].CmdPoint[HDM].FormingStr = nmeaFormingHDMStr;
		nmeaFl[ch].CmdPoint[VBW].FormingStr = nmeaFormingVBWStr;
		nmeaFl[ch].CmdPoint[VHW].FormingStr = nmeaFormingVHWStr;
		nmeaFl[ch].CmdPoint[VDO].FormingStr = nmeaFormingNullFunction;
		nmeaFl[ch].CmdPoint[ALR].FormingStr = (void (*)(u08))stimerLoop;
		nmeaFl[ch].CmdPoint[SYS].FormingStr = nmeaFormingNullFunction;
		for (uint8_t cmd = 0; cmd < MAX_nmeaCommandStr_INDEX; cmd++) {
			nmeaFl[ch].CmdPoint[cmd].All = eeSet.nmea[ch].cmd[cmd].All;
			nmeaFl[ch].CmdPoint[cmd].Counter = TIMEOUT;
			nmeaFl[ch].CmdPoint[cmd].TimeOut = NG;
		} // end for cmd
		nmeaFl[ch].TIDPriorityPermis = eeSet.nmea[ch].TIDPriorityPermis;
	} // end for ch
	for (uint8_t entry = 0; entry < ENTRY_TID_MAX; entry++) {
		TID[entry].Name[0] = TID[entry].Name[1] = 0;
		TID[entry].Channel = 0;
		TID[entry].Counter = TIMEOUT;
	}
	// разр на канале UART_ESP8266 для команды SYS вход/выход
	nmeaFl[ESP8266_UART].CmdPoint[SYS].All = (1 << In) | (1 << Out);
	stimerCreate(&sTimerALRMes, 0, ALR_MES_PERIOD, SYS_CLK_PERIOD,
		STOP, (void (*)(int))nmeaFormingCourseALRStr, 0);


}

bool NMEA::setCfgDefault(nmeaCfg_t& cfg, int ch, bool fSave)
{
	// для канала eeSet.uartTerminalNum выставляем скорость 38400
	// остальные  - 4800
	if (ch == cfg.numOfTerminalChannel) { 
		cfg.channel[ch].BautRate = ComPort::B_38400; 
	}
	else { 
		cfg.channel[ch].BautRate = ComPort::B_4800; 
	}
	cfg.channel[ch].TimeInterval = T_1S;
	cfg.channel[ch].TIDPriorityPermis = On;
	cfg.channel[ch].ioUsedForUART.in = On;	// used as uart
	cfg.channel[ch].ioUsedForUART.out = On;	// used as uart
	for (int cmd = NO; cmd < MAX_NMEACommandStrIndex; cmd++) {
		if (ch == cfg.numOfTerminalChannel) { 
			cfg.channel[ch].cmd[cmd].All = IN_E | OUT_E; 
		}
		else { 
			cfg.channel[ch].cmd[cmd].All = IN_E; 
		}
	}
	//if (fSave) {
	//	return eepromWriteStr((uint8_t*)(&eeSet), EE_SET_STRUCT_PAGE, sizeof(eeSet));
	//}
	return true;
}

bool NMEA::setCfgDefault(nmeaCfg_t& cfg, bool fSave)
{
	for (int ch = 0; ch < SysConst::UARTChannel; ch++) {
		setCfgDefault(cfg, ch, false);
	}
	//if (fSave) {
	//	eepromWriteStr((uint8_t*)(&eeSet), EE_SET_STRUCT_PAGE, sizeof(eeSet));
	//}
}
