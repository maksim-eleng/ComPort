#pragma once
#include <iostream>
#include <vector>
#include <map>

/**
* ��������� �������� ��������� ��������, ������� ������ ��������� ���
* ���������� ��������.
*/
class Eeprom {
public:
  virtual ~Eeprom() {}
  virtual std::pair<uint8_t, std::vector<uint8_t>> Operation() = 0;
};



/**
* ����� ��������� ��������� ��������� �����, ������� ������ ���������� ������
* ������ �������. ��������� ��������� ������ ������������� ���������� �����
* ������.
*/

class CreatorEeprom {
  /**
  * �������� ��������, ��� ��������� ����� ����� ���������� ����������
  * ���������� ������ �� ���������.
  */
public:
  virtual ~CreatorEeprom(){};
  virtual Eeprom* FactoryMethod() const = 0;

  /**
  * ����� ��������, ���, �������� �� ��������, �������� ����������� ���������
  * �� ����������� � �������� ���������. ������ �� �������� ��������� �������
  * ������-������, ������� �������� �� �������� ���������, ������������
  * ��������� �������. ��������� ����� �������� �������� ��� ������-������,
  * ������������� ��������� ����� � ��������� �� ���� ������ ��� ��������.
  */
  std::string SomeOperation() const {
    // �������� ��������� �����, ����� �������� ������-�������.
    Eeprom* eeprom = this->FactoryMethod();
    int sz = sizeof(eeprom);
    sz = sizeof(*eeprom);
    // �����, �������� � ���� ���������.
    std::string result = "Creator: The same creator's code has just worked with ";// +eeprom->Operation();
    delete eeprom;
    return result;
  }
};
