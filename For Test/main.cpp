#include <iostream>
#include <string>
#include "Eeprom.h"
#include "Nmea.h"
#include "ConcreteProduct2.h"








///**
//* Клиентский код работает с экземпляром конкретного создателя, хотя и через его
//* базовый интерфейс. Пока клиент продолжает работать с создателем через базовый
//* интерфейс, вы можете передать ему любой подкласс создателя.
//*/
//void ClientCode(const CreatorEeprom& creator) {
//  // ...
//  std::cout << "Client: I'm not aware of the creator's class, but it still works.\n"
//    << creator.SomeOperation() << std::endl;
//  // ...
//}

/**
* Приложение выбирает тип создателя в зависимости от конфигурации или среды.
*/


int main() {

  class A {
  public:
    char a[100];
    virtual size_t getSize() = 0;
  };
  class B : public A {
  public:
    char b[100];
    virtual size_t getSize() { return sizeof(*this); }
  };

  B b;
  int sz = sizeof(b);
  //A* a = new B();
  size_t s = b.getSize();

  struct {
    std::string str = "Temp hellow";
    const char* str2 = { "Str2 Hellow" };
    std::vector <uint8_t> v;
    std::string str3 = "Temp2 hellow";
  }tmp;

  sz = sizeof(tmp);
  uint8_t* p = reinterpret_cast<uint8_t*>(&tmp);

  uint8_t mas[150];
  int i;
  for (i = 0; i < 10; ++i) {
    tmp.v.push_back(i);
  }


  for (i = 0; i < tmp.str.size(); ++i) {
    mas[i] = tmp.str.data()[i];
  }
  for (int k=0; tmp.str2[k] != 0; ++k, ++i) {
    mas[i] = tmp.str2[k];
  }
  for (int k = 0; k < tmp.v.size(); ++k, ++i) {
    mas[i] = tmp.v.data()[k];
  }


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