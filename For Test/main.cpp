#include <iostream>
#include <string>
#include "Eeprom.h"
#include "Nmea.h"
#include "ConcreteProduct2.h"








///**
//* ���������� ��� �������� � ����������� ����������� ���������, ���� � ����� ���
//* ������� ���������. ���� ������ ���������� �������� � ���������� ����� �������
//* ���������, �� ������ �������� ��� ����� �������� ���������.
//*/
//void ClientCode(const CreatorEeprom& creator) {
//  // ...
//  std::cout << "Client: I'm not aware of the creator's class, but it still works.\n"
//    << creator.SomeOperation() << std::endl;
//  // ...
//}

/**
* ���������� �������� ��� ��������� � ����������� �� ������������ ��� �����.
*/


int main() {



  Nmea nmea;


  //Eeprom* pEeprom = new NmeaCfgEeprom();
  //std::pair<uint8_t, std::vector<uint8_t>> tmp = pEeprom->eepromLoad();
  //delete pEeprom;
  //
  //Eeprom* pE = new NmeaCfgEeprom();
  //std::pair<uint8_t, std::vector<uint8_t>>* pair = new std::pair<uint8_t, std::vector<uint8_t>>;
  //pE->eepromSave(*pair);
  //delete pair;
  //delete pE;

  //std::cout << "App: Launched with the ConcreteCreator3.\n";
  //CreatorEeprom* creator3 = new CreatorNmeaCfgEeprom();

  //ClientCode(*creator3);
  //delete creator3;

  //std::cout << "App: Launched with the ConcreteCreator2.\n";
  //CreatorEeprom* creator2 = new ConcreteCreator2();
  //ClientCode(*creator2);
  //delete creator2;
  
  return 0;
}