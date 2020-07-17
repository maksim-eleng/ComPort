#pragma once

#include <windows.h>
#include <stdint.h>

namespace SysConst {
	constexpr uint32_t clkTimeBase = 1000;	// mks, main clock time base
	constexpr char	UARTChannel = 1;				// number of channel of UART for NMEA
	constexpr int	rxBufSize = 300;
	constexpr int	txBufSize = 500;


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
