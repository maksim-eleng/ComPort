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
      uint8_t BautRate;			  // �������� ������ UART.
      uint8_t TimeInterval;	  // �������� �������� �������������� ������
      uint8_t TIDPriorityPermis;
      // permis use in/out for UART.
      // If prohibited - in/out may be used as io
      struct {
        uint8_t in  : 4;	// !=0 - io ������������ ��� UART. ==0 - ��� IO
        uint8_t out : 4;	// !=0 - io ������������ ��� UART. ==0 - ��� IO
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

