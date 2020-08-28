#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "EEPROMWin32.h"



/**
* ��������� �������� ��������� ��������, ������� ������ ��������� ���
* ���������� ��������.
*/
class Eeprom: EEPROMWin32
{
public:
  // ��� ���������� ������� ��������
  typedef struct EE_BLOCK_STRUCT{
    uint8_t objID;
    size_t size;
    char* raw;
  }eepromFieldInfo_t;

  typedef struct EE_FAT_BLOCK_STRUCT{
    uint32_t objID;
    uint32_t size;
    size_t offset;
  }eepromFatBlockInfo_t;

  typedef struct EE_FAT_STRUCT{
    uint16_t numOfBlocks = 0;
    std::vector<eepromFatBlockInfo_t> fatTable;
  }eepromFat_t;

  typedef struct EE_DATA_STRUCT{
    uint16_t numOfBlocks = 0;
    std::vector<eepromFatBlockInfo_t> fatTable;
    std::vector<uint8_t> data;
  }eepromData_t;
  
  virtual ~Eeprom() {}

  Eeprom(const char* fileName = 0)
    :EEPROMWin32(fileName)
  {}
  


  /**
   * @brief ��������� ������ FAT �������, ������������ � RAW ��������� eepromFat_t
   * @return 
  */
  eepromFat_t&& getFatTable();

  /**
  * @brief �������� ������ FAT �������, ������������ RAW � ��������� eepromFat_t
  * @return �������� �� ���� � eeprom
  */
  size_t setFatTable(eepromFat_t& fat);


  virtual bool saveEepromField() = 0;




protected:

private:
  //std::string m_raw = "";
};



