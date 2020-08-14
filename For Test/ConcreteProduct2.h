#pragma once
#include "Eeprom.h"


/**
* ���������� �������� ������������� ��������� ���������� ���������� Eeprom.
*/
class ConcreteProduct2 : public Eeprom {
public:
  std::pair<uint8_t, std::vector<uint8_t>> Operation() override {
    std::vector<uint8_t> tmp;
    std::pair<uint8_t, std::vector<uint8_t>> pair;
    return (pair);
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

