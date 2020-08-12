#include <iostream>
#include <string>
#include "Object.h"








/**
* Клиентский код работает с экземпляром конкретного создателя, хотя и через его
* базовый интерфейс. Пока клиент продолжает работать с создателем через базовый
* интерфейс, вы можете передать ему любой подкласс создателя.
*/
void ClientCode(const Creator& creator) {
  // ...
  std::cout << "Client: I'm not aware of the creator's class, but it still works.\n"
    << creator.SomeOperation() << std::endl;
  // ...
}

/**
* Приложение выбирает тип создателя в зависимости от конфигурации или среды.
*/

class A {
public:
  int m_a;
};

class B {
public:
  A m_b;
};





int main() {
  A* p1 = new A;
  B* p2 = new B;
//  p1->m_a = 0;
  p2->m_b.m_a = 0;

  std::cout << "App: Launched with the ConcreteCreator3.\n";
  Creator* creator3 = new MakerNmeaEepromCfg();
  ClientCode(*creator3);
  delete creator3;

  std::cout << "App: Launched with the ConcreteCreator1.\n";
  Creator* creator1 = new ConcreteCreator1();
//  sz = sizeof(*creator1);
  ClientCode(*creator1);
  std::cout << std::endl;
  delete creator1;

  std::cout << "App: Launched with the ConcreteCreator2.\n";
  Creator* creator2 = new ConcreteCreator2();
  ClientCode(*creator2);
  delete creator2;
  
  return 0;
}