#pragma once

#include "SysConst.h"

#ifdef _WIN32

#include <intrin.h>

#pragma intrinsic(__rdtsc)


class TimeBaseWin32
{
public:

	void loop()
	{
		static uint64_t tBegin = 0;
		if((__rdtsc() - tBegin) >= m_ticksPerSysClk) {
			tBegin = __rdtsc();
			handleEvent();
		}
		//static uint64_t tBegin = 0;
		//LARGE_INTEGER Integer;
		//QueryPerformanceCounter(&Integer);
		//if ((Integer.QuadPart - tBegin) >= m_ticksPerSysClk) {
		//	tBegin = Integer.QuadPart;
		//	handleEvent();
		//}
	}

protected:

	TimeBaseWin32()
	{
		// Get information about CPU Frequency
		// QueryPerformanceFrequency() return operation per ms
		LARGE_INTEGER Integer;
		QueryPerformanceFrequency(&Integer);
		// forming operation per SYSCLOCK period
		m_ticksPerSysClk = Integer.QuadPart / 1000 * SysConst::clkTimeBase;
	}

private:
	uint64_t m_ticksPerSysClk;
	virtual void handleEvent() = 0;

};

#endif