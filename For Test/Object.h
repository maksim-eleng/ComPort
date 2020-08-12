#pragma once
#include <string>
#include "Product.h"

/**
* Конкретные Создатели переопределяют фабричный метод для того, чтобы изменить
* тип результирующего продукта.
*/
class ConcreteCreator1 : public Creator 
{
  /**
  * Обратите внимание, что сигнатура метода по-прежнему использует тип
  * абстрактного продукта, хотя фактически из метода возвращается конкретный
  * продукт. Таким образом, Создатель может оставаться независимым от
  * конкретных классов продуктов.
  */
public:
  Eeprom* FactoryMethod() const override {
    return new ConcreteProduct1();
  }
};

class ConcreteCreator2 : public Creator 
{
  /**
  * Обратите внимание, что сигнатура метода по-прежнему использует тип
  * абстрактного продукта, хотя фактически из метода возвращается конкретный
  * продукт. Таким образом, Создатель может оставаться независимым от
  * конкретных классов продуктов.
  */
public:
  Eeprom* FactoryMethod() const override {
    return new ConcreteProduct2();
  }
};

class MakerNmeaEepromCfg : public Creator 
{
  /**
  * Обратите внимание, что сигнатура метода по-прежнему использует тип
  * абстрактного продукта, хотя фактически из метода возвращается конкретный
  * продукт. Таким образом, Создатель может оставаться независимым от
  * конкретных классов продуктов.
  */
public:
  MakerNmeaEepromCfg() {
    std::cout << "MakerNmeaEepromCfg\n";
  }

  Eeprom* FactoryMethod() const {
    return new NmeaEepromCfg();
  }
};
