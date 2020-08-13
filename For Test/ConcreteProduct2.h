#pragma once
#include "Eeprom.h"


/**
*  онкретные ѕродукты предоставл€ют различные реализации интерфейса Eeprom.
*/
class ConcreteProduct2 : public Eeprom {
public:
  std::string Operation() const override {
    return "{Result of the ConcreteProduct1}";
  }
};


/**
*  онкретные —оздатели переопредел€ют фабричный метод дл€ того, чтобы изменить
* тип результирующего продукта.
*/
class ConcreteCreator2 : public CreatorEeprom
{
  /**
  * ќбратите внимание, что сигнатура метода по-прежнему использует тип
  * абстрактного продукта, хот€ фактически из метода возвращаетс€ конкретный
  * продукт. “аким образом, —оздатель может оставатьс€ независимым от
  * конкретных классов продуктов.
  */
public:
  Eeprom* FactoryMethod() const override {
    return new ConcreteProduct2();
  }
};

