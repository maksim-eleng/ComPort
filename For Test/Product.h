#pragma once
#include <iostream>
#include <string>
#include <map>

/**
* ��������� �������� ��������� ��������, ������� ������ ��������� ���
* ���������� ��������.
*/

class Eeprom {
public:
  virtual ~Eeprom() {}
  virtual std::string Operation() const = 0;
};

/**
* ���������� �������� ������������� ��������� ���������� ���������� ��������.
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
* ����� ��������� ��������� ��������� �����, ������� ������ ���������� ������
* ������ �������. ��������� ��������� ������ ������������� ���������� �����
* ������.
*/

class Creator {
  /**
  * �������� ��������, ��� ��������� ����� ����� ���������� ����������
  * ���������� ������ �� ���������.
  */
public:
  virtual ~Creator(){};
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
    Eeprom* eeprom = FactoryMethod();
    Eeprom* eeprom2 = FactoryMethod();
    // �����, �������� � ���� ���������.
    std::string result = "Creator: The same creator's code has just worked with " + eeprom->Operation();
    delete eeprom;
    return result;
  }
};
