#pragma once
#include <iostream>
#include "ComPort.h"

#define tst

class EventSystem: public IObsTimeBase, public IObsComPort
{
public:
	EventSystem() {}
	
	virtual void handleEvent(TimeBase& ref, TimeBase::evtMask_t) override;

	virtual void handleEvent(ComPort& ref, ComPort::comEvtMsk_t evtMask) override
	{
		// 
		if (evtMask & ref.EVT_RX_USER_CHAR) {
		}
		// 
		if (evtMask & ref.EVT_RX_EOF_CHAR) {
			char str[150] = {};
			ref.getRxStr(str, sizeof(str), '\n');
			std::cout << str;
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
			std::cout << '\n' << "Error Tx" << '\n';
		}
		// 
		if (evtMask & ref.EVT_ERR_RX_BUF_OVF) {
			std::cout << '\n' << "RX buf ovf err" << '\n';
//			ref.close();
			
		}
		// 
		if (evtMask & ref.EVT_ERR_CRITICAL) {
			std::cout << '\n' << "Com critical error" << '\n';
			ComPort::comEvtMsk_t events;
			for (int i = 0; i < 10; ++i) {
				events = ref.reopen();
				if (ref.EVT_NO == events)
					break;
			}
			if(ref.EVT_NO != events)
				ref.close();
		}
	}
};

