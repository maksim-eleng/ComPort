
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


int main(int argc, char* argv[])
{
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
