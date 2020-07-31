
//#include <iostream>
//#include <windows.h>
//#include <string>
//#include <stdio.h>

#include "ComPort.h"
#include "EventSystem.h"
#include "utility.h"
#include <iostream>
#include "Nmea.h"
using namespace std;

TimeBase sysClk; 


nmeaCfgEEPROM_t nmeaCfgEEPROM;

std::vector<ComPort>com;

NMEA nmea(nmeaCfgEEPROM, com, sysClk);


int main(int argc, char* argv[])
{

	EventSystem evt;
	sysClk.addObserver(evt, sysClk.EVT_1S);

	// NMEA class doesn't add event's object as observer.
	// Add event system as observer for processing input string after NMEA
	com[0].addObserver(evt);
	com[1].addObserver(evt);

	com[0] << "Hello CH0!\n";
	com[1] << "Hello CH1!\n";
	
	while (1) {
	
		
		sysClk.loop();
		

	} // end while(1)




	
	
	

	return 1;

}
