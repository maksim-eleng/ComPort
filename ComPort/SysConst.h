#pragma once

#include <windows.h>
#include <stdint.h>

namespace SysConst {
	constexpr uint32_t clkTimeBase = 1000;	// mks, main clock time base
	constexpr char	UARTChannel = 1;
	constexpr int	rxBufSize = 300;
	constexpr int	txBufSize = 500;
}