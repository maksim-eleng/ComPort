#pragma once
#include <iostream>
#include <string>
#include <map>

/**
* Интерфейс Продукта объявляет операции, которые должны выполнять все
* конкретные продукты.
*/

class Eeprom {
public:
  virtual ~Eeprom() {}
  virtual std::string Operation() const = 0;
};

/**
* Конкретные Продукты предоставляют различные реализации интерфейса Продукта.
*/
class ConcreteProduct1 : public Eeprom {
public:
  std::string Operation() const override {
    return "{Result of the ConcreteProduct1}";
  }
};
class ConcreteProduct2 : public Eeprom {
public:
  std::string Operation() const override {
    return "{Result of the ConcreteProduct2}";
  }
};


class NmeaEepromCfg:
  public Eeprom
{
public:
  char tmp[1000] = "";

  typedef struct NMEA_CFG_EEPROM_STRUCT
  {
    char name1[10] = "Hellow1";
    char name2[10] = "Hellow2";
    char name3[10] = "Hellow3";
  }nmeaCfgEEPROM_t;



  std::string Operation() const override {
    std::string str;
    nmeaCfgEEPROM_t nmeaCfg;
    str.reserve(sizeof(nmeaCfg));
    uint8_t* pb = reinterpret_cast<uint8_t*>(&nmeaCfg);
    for (int i = 0; i < sizeof(nmeaCfg); ++i) {
      str += pb[i];
    }
    return std::move(str);
  }
};



/**
* Класс Создатель объявляет фабричный метод, который должен возвращать объект
* класса Продукт. Подклассы Создателя обычно предоставляют реализацию этого
* метода.
*/

class Creator {
  /**
  * Обратите внимание, что Создатель может также обеспечить реализацию
  * фабричного метода по умолчанию.
  */
public:
  virtual ~Creator(){};
  virtual Eeprom* FactoryMethod() const = 0;
  /**
  * Также заметьте, что, несмотря на название, основная обязанность Создателя
  * не заключается в создании продуктов. Обычно он содержит некоторую базовую
  * бизнес-логику, которая основана на объектах Продуктов, возвращаемых
  * фабричным методом. Подклассы могут косвенно изменять эту бизнес-логику,
  * переопределяя фабричный метод и возвращая из него другой тип продукта.
  */

  std::string SomeOperation() const {
    // Вызываем фабричный метод, чтобы получить объект-продукт.
    Eeprom* eeprom = FactoryMethod();
    Eeprom* eeprom2 = FactoryMethod();
    // Далее, работаем с этим продуктом.
    std::string result = "Creator: The same creator's code has just worked with " + eeprom->Operation();
    delete eeprom;
    return result;
  }
};
