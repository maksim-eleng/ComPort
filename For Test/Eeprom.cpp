#include "Eeprom.h"

/**********************************************************/
Eeprom::eepromFat_t&& Eeprom::getFatTable()
{
  eepromFat_t fat = {0};
  //// прочитать первый блок, определить кол-во блоков
  //std::vector<char>raw;
  //raw.reserve(128);
  //if (!readBlock(0, raw))
  //  return std::move(fat);
  //char* pData = raw.data();
  //// num of blocks
  //fat.numOfBlocks = *(reinterpret_cast<uint16_t*>(pData));
  //fat.fatTable.reserve(fat.numOfBlocks);
  //// check full size of eeprom
  //size_t sz = fat.numOfBlocks * (1 + sizeof(eepromFatBlockInfo_t));
  //if (sz > raw.size()) {
  //  raw.reserve(sz);
  //  readBlock(raw.size(), raw);
  //}
  //// read eeprom table
  //eepromFatBlockInfo_t* block = new eepromFatBlockInfo_t;
  //pData += sizeof(fat.numOfBlocks);
  //for (uint16_t i = 0; i < fat.numOfBlocks; ++i) {
  //  memcpy(block, pData, sizeof(eepromFatBlockInfo_t));
  //  pData += sizeof(eepromFatBlockInfo_t);
  //  fat.fatTable.emplace_back(*block);
  //}
  //delete block;
  return std::move(fat);
}

/**********************************************************/
size_t Eeprom::setFatTable(eepromFat_t& fat)
{
  std::vector<char> raw;
  raw.reserve(sizeof(fat));
  size_t res = 0;
  //for(size_t i = 0; i < sizeof(fat.numOfBlocks); ++i, ++res)
  //  raw.emplace_back(fat.numOfBlocks);
  //for (size_t k = 0; k < fat.fatTable.size(); ++k) {
  //  char* pData = reinterpret_cast<char*>(&fat.fatTable[k]);
  //  for(size_t i = 0; i < sizeof(fat.fatTable[0]); ++i, ++res, ++pData)
  //    raw.emplace_back(*pData);
  //}
  //if (!writeBlock(raw, 0)) {
  //  res = false;
  //}
  return res;
}

/**********************************************************/


