#pragma once
#include "ComPort.h"
#include "TimeBase.h"



class EventSystem: public IObsTimeBase, public IObsComPort
{
public:
	EventSystem() {}
	
	virtual void handleEvent(TimeBase& ref, TimeBase::tBaseEvtMsk_t) override;

	virtual void handleEvent(ComPort& com, comEvtMsk_t evtMask) override;

};


