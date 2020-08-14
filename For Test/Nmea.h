#pragma once
#include "Eeprom.h"
#include <vector>

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

  nmeaCfgEeprom_t m_nmeaCfgEeprom = {0};
  const uint8_t m_EEPROM_ID = 1;

  NmeaCfgEeprom() {
    int sz = sizeof(*this);
    //sz = sizeof(m_nmeaCfgEeprom);
  }

  std::pair<uint8_t, std::vector<uint8_t>> Operation() override 
  {
    std::pair<uint8_t, std::vector<uint8_t>> res;
    res.first = m_EEPROM_ID;
    res.second.reserve(sizeof(m_nmeaCfgEeprom));
    uint8_t* pField = reinterpret_cast<uint8_t*>(&m_nmeaCfgEeprom);
    for (size_t ii = 0; ii < sizeof(m_nmeaCfgEeprom); ++ii, ++pField) {
      res.second.emplace_back(*pField);
    }
    return res;
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

