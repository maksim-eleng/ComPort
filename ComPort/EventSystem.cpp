#include "EventSystem.h"

extern ComPort com;

void EventSystem::handleEvent(TimeBase& ref, TimeBase::evtMask_t evtMsk)
{
	if (evtMsk & EVT_1S) {
		std::cout << '\n' << ref.getTime() << '\t' << ref.getDate() << '\n';
		com.print("$GPRMC,101530.32,A,2726.68,S,15307.56,E,012,032,050620,11,E*63\r\n");
		com.print("$GPGGA,101525.83,2726.68,S,15307.56,E,1,4,002,+15,M,046,M,,*71\r\n");
		com.print("$GPGNS,104757.19,2726.68,S,15307.56,E,,6,002,15,046,,*73\r\n");
	}
}

