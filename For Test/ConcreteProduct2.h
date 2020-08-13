#pragma once
#include "Eeprom.h"


/**
* ���������� �������� ������������� ��������� ���������� ���������� Eeprom.
*/
class ConcreteProduct2 : public Eeprom {
public:
  std::string Operation() const override {
    return "{Result of the ConcreteProduct1}";
  }
};


/**
* ���������� ��������� �������������� ��������� ����� ��� ����, ����� ��������
* ��� ��������������� ��������.
*/
class ConcreteCreator2 : public CreatorEeprom
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

