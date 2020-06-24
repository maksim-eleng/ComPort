
//#include <iostream>
//#include <windows.h>
//#include <string>
//#include <stdio.h>

#include "ComPort.h"
#include "EventSystem.h"

using namespace std;
char rxBuf[SysConst::rxBufSize];
char txBuf[SysConst::rxBufSize];
ComPort com(rxBuf, txBuf, sizeof(rxBuf), sizeof(txBuf));
uint32_t k;

bool convStrToInt(const char* str, int& res)
{
	unsigned int tmpRes = 0;
	int ind = 0;
	res = 0;
	if (str[ind] == '-')
		++ind;
	while (str[ind] != '\0') {
		tmpRes *= 10;
		if (str[ind] < '0' || str[ind] > '9')
			return 0;
		tmpRes += str[ind] - '0';
		if (tmpRes > INT_MAX)
			return 0;
		++ind;
	}
	res = tmpRes;
	if (str[0] == '-')	
		res = -res;
	return true;
}

int main(int argc, char* argv[])
{
	ComPort::comEvtMsk_t events;

	TimeBase sysClk(EVT_1S | EVT_10MS);
	EventSystem evt;

	sysClk.addObserver(evt, EVT_1S);
	sysClk.addObserver(com, EVT_10MS);
	com.addObserver(evt);


	{
		int comNum = 0;
		int baud = 0;
		events = com.EVT_ERR_CRITICAL;
		// set parameters of com port from command string 
		if (argc > 1) {
			bool fReady = convStrToInt(argv[1], comNum);
			if (argc > 2 && fReady)
				fReady = convStrToInt(argv[2], baud);
			if (fReady) {
				if (!baud) {
					baud = com.B_4800;
				}
				events = com.open(comNum, (ComPort::comBaud_t)baud);
			}
		}
		// or search port and connect with default baud
		else {
			comNum = 1;
			baud = com.B_4800;
			for (; events && comNum < 20; ++comNum) {
				events = com.open(comNum, (ComPort::comBaud_t)baud);
				if (!events)
					break;
			}
		}
		if (events)
			std::cout << "Com not connected";
		else
			std::cout << "Com" << comNum << " found and connected on " << baud;
	}
	events = com.EVT_ERR_CRITICAL;
	events = com.setParam(com.USE_USER_CHAR_EVENT, '$');
	events = com.setParam(com.USE_EOF_CHAR_EVENT, '\n');
	if (events) {
		evt.handleEvent(com, events);
	}

	while (1) {
	
		
		sysClk.loop();
		

	} // end while(1)




	
	
	

// 	system("PAUSE");
	return 1;

}
