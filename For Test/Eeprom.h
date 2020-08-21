#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "EEPROMWin32.h"



/**
* Интерфейс Продукта объявляет операции, которые должны выполнять все
* конкретные продукты.
*/
class Eeprom: EEPROMWin32
{
public:
  typedef struct EE_FIELD_STRUCT{
    uint8_t objID = 0;
    size_t size = 0;
  }eepromFieldInfo_t;

  typedef struct EE_RAW_STRUCT{
    std::vector<eepromFieldInfo_t> eepromTable;
    const char* eofTable = "-EOF_MAP-";
    std::string data;
  }eepromRaw_t;
  
  virtual ~Eeprom() {}

  Eeprom(const char* fileName = 0)
    :EEPROMWin32(fileName)
  {
  }
  

  bool loadEepromField(uint8_t id)
  {
    eepromRaw_t raw;

    if (!readFile(m_raw)) {
      return false;
    }
    // получить пол-во записей
    uint8_t maxNum = m_raw.find(raw.eofTable) / raw.eepromTable.size();
    size_t startRaw = m_raw.find(raw.eofTable) + sizeof(raw.eofTable);
    const char* pData = m_raw.data();


    // поиск смещения 

  }

  virtual bool saveEepromField() = 0;




protected:

private:
  std::string m_raw = "";
};



