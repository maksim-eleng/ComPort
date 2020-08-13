#pragma once
#include "Eeprom.h"

/**
* @brief Конкретный продукт NmeaEepromCfg представляет реализацию интерфейса Eeprom
*/
class NmeaCfgEeprom:
  public Eeprom
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
        uint8_t in : 4;	  // !=0 - io используется для UART. ==0 - как IO
        uint8_t out : 4;	// !=0 - io используется для UART. ==0 - как IO
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
  * Обратите внимание, что сигнатура метода по-прежнему использует тип
  * абстрактного продукта, хотя фактически из метода возвращается конкретный
  * продукт. Таким образом, Создатель может оставаться независимым от
  * конкретных классов продуктов.
  */
public:
  CreatorNmeaCfgEeprom() = default;

  Eeprom* FactoryMethod() const {
    return new NmeaCfgEeprom();
  }
};

