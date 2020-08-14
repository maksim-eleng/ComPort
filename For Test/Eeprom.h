#pragma once
#include <iostream>
#include <vector>
#include <map>

/**
* Интерфейс Продукта объявляет операции, которые должны выполнять все
* конкретные продукты.
*/
class Eeprom {
public:
  virtual ~Eeprom() {}
  virtual std::pair<uint8_t, std::vector<uint8_t>> Operation() = 0;
};



/**
* Класс Создатель объявляет фабричный метод, который должен возвращать объект
* класса Продукт. Подклассы Создателя обычно предоставляют реализацию этого
* метода.
*/

class CreatorEeprom {
  /**
  * Обратите внимание, что Создатель может также обеспечить реализацию
  * фабричного метода по умолчанию.
  */
public:
  virtual ~CreatorEeprom(){};
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
    Eeprom* eeprom = this->FactoryMethod();
    int sz = sizeof(eeprom);
    sz = sizeof(*eeprom);
    // Далее, работаем с этим продуктом.
    std::string result = "Creator: The same creator's code has just worked with ";// +eeprom->Operation();
    delete eeprom;
    return result;
  }
};
