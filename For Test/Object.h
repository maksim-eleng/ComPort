#pragma once
#include <string>
#include "Product.h"

/**
* ���������� ��������� �������������� ��������� ����� ��� ����, ����� ��������
* ��� ��������������� ��������.
*/
class ConcreteCreator1 : public Creator 
{
  /**
  * �������� ��������, ��� ��������� ������ ��-�������� ���������� ���
  * ������������ ��������, ���� ���������� �� ������ ������������ ����������
  * �������. ����� �������, ��������� ����� ���������� ����������� ��
  * ���������� ������� ���������.
  */
public:
  Eeprom* FactoryMethod() const override {
    return new ConcreteProduct1();
  }
};

class ConcreteCreator2 : public Creator 
{
  /**
  * �������� ��������, ��� ��������� ������ ��-�������� ���������� ���
  * ������������ ��������, ���� ���������� �� ������ ������������ ����������
  * �������. ����� �������, ��������� ����� ���������� ����������� ��
  * ���������� ������� ���������.
  */
public:
  Eeprom* FactoryMethod() const override {
    return new ConcreteProduct2();
  }
};

class MakerNmeaEepromCfg : public Creator 
{
  /**
  * �������� ��������, ��� ��������� ������ ��-�������� ���������� ���
  * ������������ ��������, ���� ���������� �� ������ ������������ ����������
  * �������. ����� �������, ��������� ����� ���������� ����������� ��
  * ���������� ������� ���������.
  */
public:
  MakerNmeaEepromCfg() {
    std::cout << "MakerNmeaEepromCfg\n";
  }

  Eeprom* FactoryMethod() const {
    return new NmeaEepromCfg();
  }
};
