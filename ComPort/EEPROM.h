#pragma once
#ifndef __EEPROM_H
#define __EEPROM_H

/*****************************************************
* @brief Change include and using according base system.
ComPortWin32 class working in win32 system
******************************************************/
#ifdef _WIN32
#include "EEPROMWin32.h"
using SysEEPROMClass_t = EEPROMWin32;
#endif



typedef struct EEPROM_SET_STRUCT
{
//	nmeaCfgEEPROM_t nmeaCfg;	// sets for NMEA
	char tmp[30]{ "TestTestTest\n" };
}sysEEPROMSets_t;



class EEPROM : public SysEEPROMClass_t
{
public:
	typedef enum :uint8_t
	{
		EE_OK,
		EE_FILE_NOT_FOUND,
		EE_NOT_INIT,
	}eepromStatus_t;

	EEPROM(const char* fileName)
		:SysEEPROMClass_t(fileName),
		m_status(EE_OK)
	{
		if (!read(&m_struct, sizeof(m_struct))) {
			m_status = EE_FILE_NOT_FOUND;
		}
		else if (!isInit())
				m_status = EE_NOT_INIT;
	}

	EEPROM() = default;
	~EEPROM() = default;

	bool isInit()
	{
		uint8_t* p = reinterpret_cast<uint8_t*> (&m_struct);
		uint8_t d = *p;
		short c = 0;
		for (int i = 1; i < sizeof(m_struct); ++i) {
			if (d == p[i])	
				++c;
			else 
				d = p[i];
		}
		return (static_cast<long>(c) * 100 / sizeof(m_struct) <= 50) ? true : false;
	}


	EEPROM& getObject() {
		return *this;
	}

private:
	sysEEPROMSets_t m_struct;
	eepromStatus_t m_status;
};




#endif // !__EEPROM_H
