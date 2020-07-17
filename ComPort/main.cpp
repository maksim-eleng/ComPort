
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

char rxBuf[SysConst::rxBufSize];
char txBuf[SysConst::rxBufSize];
ComPort com(sysClk, rxBuf, txBuf, sizeof(rxBuf), sizeof(txBuf));
Nmea nmea;

int main(int argc, char* argv[])
{
	comEvtMsk_t events;
	
	
	EventSystem evt;

	sysClk.addObserver(evt, TimeBase::EVT_1S);
	com.addObserver(evt);
	
	
	{
		int comNum = 0;
		int baud = 0;
		events = com.EVT_ERR_CRITICAL;
		// set parameters of com port from command string 
		if (argc > 1) {
			bool fReady = convStrToInt(argv[1], comNum);
			if (argc > 2 && fReady) {
				fReady = convStrToInt(argv[2], baud);
			}
			if (fReady) {
				if (!baud) {
					baud = com.B_4800;
				}
				events = com.open(comNum, (comBaud_t)baud);
			}
		}
		// or search port and connect with default baud
		else {
			comNum = 1;
			baud = com.B_4800;
			for (; events && comNum < 20; ++comNum) {
				events = com.open(comNum, (comBaud_t)baud);
				if (!events)
					break;
			}
		}
		if (events)
			std::cout << "Com not connected";
		else
			std::cout << "Com" << comNum << " found and connected on " << baud << "\n";

		comCfg_t cfg;
		cfg.byteSize = 8;
		cfg.parity = com.P_NO;
		cfg.stopBits = com.SBIT_ONE;
		cfg.evtChar = '\n';
		events = com.setParam(cfg);
		if (events) {
			evt.handleEvent(com, events);
		}
		else {
			com << "Start.\n" << " Port #" << com.getPortNumber() << " was opened. Baud rate: " \
				<< com.getBaud() << "\n";
		}
	}

	while (1) {
	
		
		sysClk.loop();
		

	} // end while(1)




	
	
	

// 	system("PAUSE");
	return 1;

}
