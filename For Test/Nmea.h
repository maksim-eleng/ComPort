#pragma once
#include "Eeprom.h"

/**
* @brief ���������� ������� NmeaEepromCfg ������������ ���������� ���������� Eeprom
*/
class NmeaCfgEeprom:
  public Eeprom
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
        uint8_t in : 4;	  // !=0 - io ������������ ��� UART. ==0 - ��� IO
        uint8_t out : 4;	// !=0 - io ������������ ��� UART. ==0 - ��� IO
      }ioUsedForUART;
    }chCfg[10];

    uint8_t numOfTerminalChannel;			// channel number of UART for terminal 
  }nmeaCfgEeprom_t;

  NmeaCfgEeprom() {
    std::memset(this, 0, sizeof(*this));
  }

  std::string Operation() const override {
    std::string str;
    
    nmeaCfgEeprom_t nmeaCfg;
    str.reserve(sizeof(nmeaCfg));
    uint8_t* pb = reinterpret_cast<uint8_t*>(&nmeaCfg);
    for (int i = 0; i < sizeof(nmeaCfg); ++i) {
      str += pb[i];
    }
    return std::move(str);
  }
};


class CreatorNmeaCfgEeprom : public CreatorEeprom 
{
  /**
  * �������� ��������, ��� ��������� ������ ��-�������� ���������� ���
  * ������������ ��������, ���� ���������� �� ������ ������������ ����������
  * �������. ����� �������, ��������� ����� ���������� ����������� ��
  * ���������� ������� ���������.
  */
public:
  CreatorNmeaCfgEeprom() = default;

  Eeprom* FactoryMethod() const {
    return new NmeaCfgEeprom();
  }
};

