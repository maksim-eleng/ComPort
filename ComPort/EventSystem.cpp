#include "EventSystem.h"
#include <iostream>


/**************************************************************/
void EventSystem::handleEvent(TimeBase& ref, TimeBase::tBaseEvtMsk_t evtMsk)
{
	if (evtMsk & ref.EVT_1S) {
		std::cout << '\n' << ref.getTimeStr() << '\t' << ref.getDateStr() << '\n';
	}
}

/**************************************************************/
void EventSystem::handleEvent(ComPort& com, const comEvtMsk_t evtMask)
{
	// 
	if (evtMask & com.EVT_RX_USER_CHAR) {
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
