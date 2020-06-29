#pragma once
#include <iostream>
#include "ComPort.h"



class EventSystem: public IObsTimeBase, public IObsComPort
{
public:
	EventSystem() {}
	
	virtual void handleEvent(TimeBase& ref, TimeBase::evtMask_t) override;

	virtual void handleEvent(ComPort& ref, comEvtMsk_t evtMask) override
	{
		// 
		if (evtMask & ref.EVT_RX_USER_CHAR) {
			while (ref.userCharGetReceivedCounter()) {
				char str[150] = {};
				ref.getRxStr(str, sizeof(str), '\n');
				std::cout << str;
				ref.userCharHandled();
			}
		}
		// 
		if (evtMask & ref.EVT_RX80_FULL) {
		}
		// 
		if (evtMask & ref.EVT_CTS_CHANGED) {
		}
		// 
		if (evtMask & ref.EVT_DSR_CHANGED) {
		}
		// 
		if (evtMask & ref.EVT_ERR_LINE) {
		}
		// 
		if (evtMask & ref.EVT_ERR_TX) {
			std::cout << "\n\n\n\nError Tx" << '\n';
		}
		// 
		if (evtMask & ref.EVT_ERR_RX_BUF_OVF) {
			std::cout << "\n\n\n\nRX buf ovf err" << '\n';
		}
		//
		if (evtMask & ref.EVT_ERR_INVALID_PARAM) {
			std::cout << "Invalid parameter.\n";
		}
		// 
		if (evtMask & ref.EVT_ERR_CRITICAL) {
			std::cout << '\n' << "Com critical error" << '\n';
			comEvtMsk_t events;
			events = ref.reopen();
			if(ref.EVT_NO != events)
				ref.close();
		}
	}
};

