#pragma once
#include "Eeprom.h"
#include <vector>

/**
* @brief Product field 
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
        uint8_t in  : 4;	// !=0 - io используется для UART. ==0 - как IO
        uint8_t out : 4;	// !=0 - io используется для UART. ==0 - как IO
      }ioUsedForUART;
    }chCfg[10];

    uint8_t numOfTerminalChannel;			// channel number of UART for terminal 
  }nmeaCfgEeprom_t;

  struct nmeaCfgEepromInfo_t {
    const size_t size = sizeof(nmeaCfgEeprom_t);
  };

  virtual nmeaCfgEepromInfo_t getObjInfo() const {
    return nmeaCfgEepromInfo_t();
  }

  virtual bool loadEepromField()
  {

    return 1;
  }

  // сохранение текущих настроек 
  virtual bool saveEepromField() override
  {
    eepromFieldInfo_t eeSet;
    eeSet.objID = m_EEPROM_ID;
    eeSet.size = sizeof(nmeaCfgEeprom_t);
    eeSet.raw = new char[sizeof(nmeaCfgEeprom_t)];
    nmeaCfgEeprom_t nmeaCfgEeprom;
    memcpy(eeSet.raw, reinterpret_cast<char*>(&nmeaCfgEeprom), eeSet.size);
    
    
    delete[] eeSet.raw;
    return 1;
  }
private:
  const uint8_t m_EEPROM_ID = 1;
};

  class EepromMaker
  {
  public:
    virtual ~EepromMaker() {}

    /**
     * @brief 
    */
    void save() {
      Eeprom* obj = factrory();
      obj->saveEepromField();

    }

    /**
     * @brief 
    */
    void load() {
      Eeprom* obj = factrory();
      //obj->loadEepromField();

    }
    
  private:
    virtual Eeprom* factrory() = 0;

  };

  /**
   * @brief 
  */
  class NmeaCfgEepromMaker
    :public EepromMaker
  {
  public:

  private:
    virtual Eeprom* factrory() override {
      return new NmeaCfgEeprom();
    }

  };




class Nmea
{
public:
  friend NmeaCfgEeprom;

  Nmea()
  {
    NmeaCfgEepromMaker cfg;
    cfg.save();
    
    
  }



private:
  

};



/**
* @brief 
* @return 
*/
//std::pair<uint8_t, std::vector<uint8_t>> eepromLoad()  
//{
//  std::pair<uint8_t, std::vector<uint8_t>> res;
//  res.first = m_EEPROM_ID;
//  size_t sz = this->getSize();
//  res.second.reserve(sizeof(m_cfgEeprom));
//  uint8_t* pField = reinterpret_cast<uint8_t*>(&m_cfgEeprom);
//  for (size_t ii = 0; ii < sizeof(m_cfgEeprom); ++ii, ++pField) {
//    res.second.emplace_back(*pField); 
//  }
//  return res;
//}

/**/
//bool eepromSave(eeprom_t) 
//{
//
//  return 1;
//}

//eeprom_t makeObject() 
//{
//  eeprom_t res;
//  nmeaCfgEeprom_t cfg;
//  res.first = m_EEPROM_ID;
//  res.second.reserve(sizeof(nmeaCfgEeprom_t));
//  uint8_t* pField = reinterpret_cast<uint8_t*>(&cfg);
//  for (size_t ii = 0; ii < sizeof(nmeaCfgEeprom_t); ++ii, ++pField) {
//    res.second.emplace_back(*pField); 
//  }
//  return res;
//}
