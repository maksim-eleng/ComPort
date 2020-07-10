#include "EventSystem.h"
#include <iostream>

extern ComPort com;

/**************************************************************/
void EventSystem::handleEvent(TimeBase& ref, TimeBase::evtMask_t evtMsk)
{
	if (evtMsk & EVT_1S) {
		std::cout << '\n' << ref.getTime() << '\t' << ref.getDate() << '\n';
		com.print("$GPRMC,101530.32,A,2726.68,S,15307.56,E,012,032,050620,11,E*63\r\n");
		com.print("$GPGGA,101525.83,2726.68,S,15307.56,E,1,4,002,+15,M,046,M,,*71\r\n");
		com.print("$GPGNS,104757.19,2726.68,S,15307.56,E,,6,002,15,046,,*73\r\n");
	}
}

/**************************************************************/
void EventSystem::handleEvent(ComPort& com, comEvtMsk_t evtMask)
{
	// 
	if (evtMask & com.EVT_RX_USER_CHAR) {
		while (com.userCharGetReceivedCounter()) {
			char str[150] = {};
			com.getRxStr(str, sizeof(str));
			std::cout << str;
			com.userCharHandled();
		}
	}
	// 
	if (evtMask & com.EVT_RX80_FULL) {
	}
	// 
	if (evtMask & com.EVT_CTS_CHANGED) {
	}
	// 
	if (evtMask & com.EVT_DSR_CHANGED) {
	}
	// 
	if (evtMask & com.EVT_ERR_LINE) {
	}
	// 
	if (evtMask & com.EVT_ERR_TX) {
		std::cout << "\n\n\n\nError Tx" << '\n';
	}
	// 
	if (evtMask & com.EVT_ERR_RX_BUF_OVF) {
		std::cout << "\n\n\n\nRX buf ovf err" << '\n';
	}
	//
	if (evtMask & com.EVT_ERR_INVALID_PARAM) {
		std::cout << "Invalid parameter.\n";
	}
	// 
	if (evtMask & com.EVT_ERR_CRITICAL) {
		std::cout << '\n' << "Com critical error" << '\n';
		comEvtMsk_t events;
		events = com.reopen();
		if (com.EVT_NO != events)
			com.close();
	}
}
