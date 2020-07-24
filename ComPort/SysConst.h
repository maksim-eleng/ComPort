#pragma once

#include <windows.h>
#include <stdint.h>

namespace SysConst {
	constexpr uint32_t	clkTimeBase = 1000;	// mks, main clock time base
	constexpr uint8_t		maxUARTChannel = 5;				// max number of UART channel for NMEA
	constexpr uint16_t	rxBufSize = 500;
	constexpr uint16_t	txBufSize = 500;
	constexpr uint8_t		nmeaMaxStrLen = 150;


}

/**************   Простые типы данных 	*******************
*********************************************************/
#ifndef uint8_t
#include "stdint.h"
#endif

#define No	0
#define NG	0
#define Off	0
#define UNLOCK	0
#ifndef NULL
#define NULL	0
#endif

#define Yes	1
#define OK	1
#define On	1
#define LOCK	1
