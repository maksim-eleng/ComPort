#pragma once
#include "Eeprom.h"
#include <vector>

/**
 * @brief 
*/
class NmeaCfgEeprom
{
public:
  typedef struct {
    struct CFG_CHANNEL_EEPROM_STRUCT
    {
      uint8_t cmdPermits[100];
      uint8_t BautRate;			  // скорость работы UART.
      uint8_t TimeInterval;	  // интервал передачи сформированных команд
      uint8_t TIDPriorityPermis;
      // permis use in/out for UART.
      // If prohibited - in/out may be used as io
      struct {
        uint8_t in  : 4;	// !=0 - io используется для UART. ==0 - как IO
        uint8_t out : 4;	// !=0 - io используется для UART. ==0 - как IO
      }ioUsedForUART;
    }chCfg[10];

    uint8_t numOfTerminalChannel;			// channel number of UART for terminal 
  }nmeaCfgEeprom_t;

private:
  const uint8_t m_EEPROM_ID = 1;
};






class Nmea
{
public:
  friend NmeaCfgEeprom;

  Nmea()
  {
    
    
  }



private:
  

};

