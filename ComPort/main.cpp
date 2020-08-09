
//#include <iostream>
//#include <windows.h>
//#include <string>
//#include <stdio.h>

#include "ComPort.h"
#include "EventSystem.h"
#include "utility.h"
#include <iostream>
#include "Nmea.h"
#include "EEPROM.h"



TimeBase sysClk; 
std::vector<ComPort>com;


// Иерархия классов игровых персонажей
class Warrior
{
public:
  virtual void info() = 0;
  virtual ~Warrior() {}
};

class Archer : public Warrior
{
public:
  void info() {
		std::cout << "Archer" << std::endl;
  };
	char g;
};

class Horseman : public Warrior
{
public:
  void info() {
		std::cout << "Horseman" << std::endl;
  };
	char g;
};


// Фабрики объектов
class Factory
{
public:
	virtual Warrior* createWarrior() = 0;
  virtual ~Factory() {}
};


class ArchersFactory : public Factory
{
public:
  Warrior* createWarrior() {
    return new Archer;
  }
};

class HorsemanFactory : public Factory
{
public:
	Warrior* createWarrior() {
		return new Horseman;
	}
};




int main(int argc, char* argv[])
{
	ArchersFactory* archers_factory = new ArchersFactory;
	HorsemanFactory* horsman_factory = new HorsemanFactory;


	std::vector<Warrior*> v;
	v.push_back(horsman_factory->createWarrior());
	v.push_back(archers_factory->createWarrior());

	

	for (int i = 0; i < v.size(); i++)
		v[i]->info();


	// Config system
	EEPROM eeprom(SysConst::nmeaCfgFileName);
	//NMEA nmea(eeprom, com, sysClk);
	//NMEA n;
	//bool res = eeprom.isErased();
	//int i = sizeof(eeprom);



	EventSystem evt;
	sysClk.addObserver(evt, sysClk.EVT_1S);
	// NMEA class doesn't add event's object as observer.
	// Add event system as observer for processing input string after NMEA
	com[0].addObserver(evt, ComPort::EVT_RX_USER_CHAR);
	com[1].addObserver(evt, ComPort::EVT_RX_USER_CHAR);

	com[0] << "Hello CH0!\n";
	com[1] << "Hello CH1!\n";

 


	while (1) {
	
		
		sysClk.loop();
		

	} // end while(1)



	    
	
	

	return 1;

}
