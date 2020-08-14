#include <iostream>
#include <string>
#include "Eeprom.h"
#include "Nmea.h"
#include "ConcreteProduct2.h"








/**
* Клиентский код работает с экземпляром конкретного создателя, хотя и через его
* базовый интерфейс. Пока клиент продолжает работать с создателем через базовый
* интерфейс, вы можете передать ему любой подкласс создателя.
*/
void ClientCode(const CreatorEeprom& creator) {
  // ...
  std::cout << "Client: I'm not aware of the creator's class, but it still works.\n"
    << creator.SomeOperation() << std::endl;
  // ...
}

/**
* Приложение выбирает тип создателя в зависимости от конфигурации или среды.
*/
int main() {

  Eeprom* pEeprom = new NmeaCfgEeprom();
  std::pair<uint8_t, std::vector<uint8_t>> tmp = pEeprom->Operation();
  delete pEeprom;
 

  std::cout << "App: Launched with the ConcreteCreator3.\n";
  CreatorEeprom* creator3 = new CreatorNmeaCfgEeprom();

  ClientCode(*creator3);
  delete creator3;

  std::cout << "App: Launched with the ConcreteCreator2.\n";
  CreatorEeprom* creator2 = new ConcreteCreator2();
  ClientCode(*creator2);
  delete creator2;
  
  return 0;
}