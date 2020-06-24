#pragma once

#ifdef _WIN32

#include "SysConst.h"
#include "Buffer.h"

class ComPortWin32
{
public:
	/************************************************************
	 * @brief Com port event masks. Must be set by means of setEvent()
	 in com module only. Events mask may be check in external path.
	*************************************************************/
	typedef enum EVT_COMM_MASK_ENUM
	{
		EVT_NO,
		// User char was detected in com-port stream
		// User char set throught setParam(USE_USER_CHAR_EVENT, '$')
		EVT_RX_USER_CHAR	= (1 << 1),
		// End of string char was detected in com-port stream
		// This char set throught setParam(USE_EOF_CHAR_EVENT, '\n')
		EVT_RX_EOF_CHAR		= (1 << 2),
		// Win32 rx buffer if full more than 80%
		EVT_RX80_FULL			= (1 << 3),
		// CTS changed state
		EVT_CTS_CHANGED		= (1 << 4),
		// DSR changed state
		EVT_DSR_CHANGED		= (1 << 5),
		// Line status error occurred
		EVT_ERR_LINE					= (1 << 6),
		// Error when tx. Last data not transmitted
		EVT_ERR_TX						= (1 << 7),
		// Rx buffer overflow when write operation
		EVT_ERR_RX_BUF_OVF		= (1 << 8),
		// Critical errors as com not ready, not open, not handle,...
		EVT_ERR_CRITICAL			= (1 << 10),
	}comEvtMsk_t;

	/*********************************************************
	 * @brief For config Com-port's Baud rate.
	 For description see PARAM_INIT_ENUM
	**********************************************************/
	typedef enum BAUD_ENUM
	{
		B_NO,
		B_1200 = CBR_1200,
		B_2400 = CBR_2400,
		B_4800 = CBR_4800,
		B_9600 = CBR_9600,
		B_38400 = CBR_38400,
		B_115200 = CBR_115200,
	}comBaud_t;

	/*********************************************************
	 * @brief For config Com-port's Parity variante.
	 For description see PARAM_INIT_ENUM
	**********************************************************/
	typedef enum PARITY_ENUM
	{
		NO = NOPARITY,
		ODD = ODDPARITY,
		EVEN = EVENPARITY,
		MARK = MARKPARITY,
		SPACE = SPACEPARITY,
	}comParity_t;

	/*********************************************************
	 * @brief Numbers of stop bit. For config Com-port.
	 For description see PARAM_INIT_ENUM
	**********************************************************/
	typedef enum STOP_BIT_ENUM
	{
		ONE_BIT = ONESTOPBIT,
		ONE5_BIT = ONE5STOPBITS,
		TWO_BIT = TWOSTOPBITS
	}comStopBit_t;

	/*********************************************************
	 * @brief DTR line control. For config Com-port.
	 For description see PARAM_INIT_ENUM
	**********************************************************/
	typedef enum DTR_CONTROL_ENUM
	{
		DTR_DISABLE = DTR_CONTROL_DISABLE,	// DTR out =0ff if COM-port is opened. State can be changed by function EscapeCommFunction()
		DTR_ENABLE = DTR_CONTROL_ENABLE,	// DTR out =0n if COM-port is opened. State can be changed by function EscapeCommFunction()	
		DTR_HANDSHAKE = DTR_CONTROL_HANDSHAKE	// DTR out = 0n / Off automatically if COM - port is opened / closed
	}comDTRControl_t;

	/*********************************************************
	 * @brief RTS line control. For config Com-port.
	 For description see PARAM_INIT_ENUM
	**********************************************************/
	typedef enum RTS_CONTROL_ENUM
	{
		RTS_DISABLE = RTS_CONTROL_DISABLE,
		RTS_ENABLE = RTS_CONTROL_ENABLE,
		RTS_HANDSHAKE = RTS_CONTROL_HANDSHAKE,
		RTS_TOGGLE = RTS_CONTROL_TOGGLE
	}comRTSControl_t;

	/*********************************************************
	* @brief 
	Event's mask for toggle WIN system's event mask. For config Com-port only 
	and detect event in checkEventWin32
	 For description see PARAM_INIT_ENUM
	**********************************************************/
	typedef enum EVENT_SET_MASK_ENUM
	{
		// Generate event if any char was received
		EVTSET_RX_CHAR			= EV_RXCHAR,
		// Generate event if evtChar char was received
		// Event char is set throught second parameter cfgValue (e.g. '\n')
		EVTSET_RX_EOFCHAR		= EV_RXFLAG,
		// Event if win system Rx Buffer is full by 80%
		EVTSET_RX80FULL			= EV_RX80FULL,
		EVTSET_CTS_CHANGED	= EV_CTS,		// CTS input changed
		EVTSET_DSR_CHANGED	= EV_DSR,		// DSR input changed
		EVTSET_LINE_ERR			= EV_ERR,		// Errors frame, overrun ore RxParity was detected
	}comEvtSetMsk_t;

	/*********************************************************
	@brief
	* For Parameters init commands as name of parameter	***
	* For interface config Win32 DCB struct for COM - port
	* Instruction about DCB:
	*http://vsokovikov.narod.ru/New_MSDN_API/Comm_res/str_dcb.htm
	*https://ru.wikibooks.org/wiki/COM-%D0%BF%D0%BE%D1%80%D1%82_%D0%B2_Windows_%28%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5%29
	*********************************************************/
	typedef enum PARAM_INIT_ENUM
	{
	NO_PARAM,
	// Set Baud. Use BAUD_ENUM as second param
	BAUD,
	/****		FOR STRUCTURE FRAME		*****/
		// Size is set throught second parameter (4...8).
	BYTE_SIZE,
		// Use STOP_BIT_ENUM as second param:
		// ONE_BIT, ONE5_BIT, TWO_BIT
	STOP_BITS,
	/***	 parity check		************/
		// Paruty type. Use PARITY_ENUM as second param
		// NO, ODD, EVEN, MARK, SPACE
	PARITY_TYPE,
		// use/or not check parity control for input data 
		// Use bool val us second param for on/off
	PARITY_CHECK,
		// replace received byte to char, if parity err (set by PARITY_ERR_CHAR)
		// Error char must be set as second parameter
	PARITY_USE_ERR_CHAR,
		// switch off replace received byte to char, if parity err (set by PARITY_ERR_CHAR)
		// second param may not be used
	PARITY_NOT_USE_ERR_CHAR,
	/***	Connection for flow control		**********/
	//4(DTR - Data terminal ready)--->-- - 6 (DSR - Data set ready)
	//7(RTS - Request to Send)--->-- - 8 (CTS - Clear to Send - готовность приема)
	//8 (CTS - Clear to Send - -- - < -- - 7 (RTS - Request to Send)
		// Libe DTR---->----DSR
	USE_DTR_OUT,		// DTR control. Use DTR_CONTROL_ENUM as second param
	USE_DSR_FLOW,		// transmit only if DSR input = On. Use bool val us second param for on/off
	USE_DSR_SENS,		// receive only if DSR input = On. Use bool val us second param for on/off
		// Libe RTS---->----CTS
	USE_RTS_OUT,		// RTS out control. Use RTS_CONTROL_ENUM as second param
	USE_CTS_FLOW,		// transmit if CTS input(on PC) (receiver ready) = on. Use bool val as second param for on/off
	//	/*****  FOR OTHER ERRORS	*******/
		// ignor NULL symbol if received
		// Use bool val us second param for on/off
	USE_NULL_IGNORE,	
	
	//*********** Event part	**********/
		// Use user char event. Generate event EVT_RX_USER_CHAR
		// if char was detected in input stream.
		// Char must be set as second param
	USE_USER_CHAR_EVENT,
		// Use end of string char event. Generate event EVT_RX_EOF_CHAR
		// if char was detected in input stream.
		// Char must be set as second param
	USE_EOF_CHAR_EVENT,
		// Set event's mask for sys WIN ComPort.
		// Use EVENT_SET_MASK_ENUM for forming mask as (mask1 | mask2)
	EV_SET,
	//*****  Settins for _COMMTIMEOUTS struct  ************/
	// Recomendation parameters for asynchronous regine: 100, 0, 0, 0, 0 (default in create port time)
	// Time interval is set throught second parameter cfgValue.
	T_READ_INTERVAL,						/* ReadIntervalTimeout - Maximum time between read chars.	*/
	T_READ_TOTAL_MULTIPLIER,   	/* ReadTotalTimeoutMultiplier - Multiplier of characters.	*/
	T_READ_TOTAL_CONSTANT,     	/* ReadTotalTimeoutConstant - Constant in milliseconds.		*/
	T_WRITE_TOTAL_MULTIPLIER,  	/* WriteTotalTimeoutMultiplier - Multiplier of characters.*/
	T_WRITE_TOTAL_CONSTANT,    	/* WriteTotalTimeoutConstant - Constant in milliseconds.	*/
	}comParamInit_t;

	/***************************************************************
	* @brief Set parameters of com-port. May be used after create object of port and 
	* if port is opened (if not - the file descriptor not defined).
	* Configuration DCB, TIMEOUTS structs for communication throught com port
	* @param	param <comParamInit_t>	- type of parameter. The PARAM_INIT_ENUM must be used
	* @param	val   <uint32_t>	- value for parameter. See  PARAM_INIT_ENUM for description
	* @return	<evtMask_t> EVT_NO (0) - parameter was set if without errors, or event mask about error.
						If error - port handler is closed
	****************************************************************/
	comEvtMsk_t setParam(comParamInit_t param, uint32_t val = 0);

	/*****************************************************************
	 * @brief Check of event.
	 * @param evtMask_t events - external variable of events
	 * @param evtMask_t mask - mask for check. The EVT_COMM_MASK_ENUM must be used
	 * @return true if event is set 
	****************************************************************/
	bool checkEvent(comEvtMsk_t& events, comEvtMsk_t mask) const;

	/**************************************************************
	* @brief Open COM-port with default values 8N1 with EV_RX_CHAR (Win32 event) mask.
	* Addition parameters will be set letter via setParam() function
	* Creates a file descriptor.
	* Timings set as: for Read 1 char - max, the rest coeffichience - 0
	* (for without delays and wait operation).
	* @param comNum	<int> - number of com port
	* @param baud		<comBaud_t> - BaudRate. The BAUD_ENUM must be used
	* @return				<evtMask_t> EVT_NO (0) - if OK, 
									or event mask of EVT_COMM_MASK_ENUM and port handler is closed
	***************************************************************/
	comEvtMsk_t open( int comNum, comBaud_t baud );

	/**************************************************************
	* @brief Open COM-port with default values 8N1 with EV_RX_CHAR (Win32 event) mask
	* Addition parameters will be set letter via setParam() function
	* Creates a file descriptor.
	* Timings set as: for Read 1 char - max, the rest coeffichience - 0
	* (for without delays and wait operation).
	* @param comName	<const char* const> - number of com port in the form "COM1" or "\\.\COM1"
	* @param baud		<comBaud_t> - BaudRate. The BAUD_ENUM must be used
	* @return				<evtMask_t> EVT_NO (0) - if OK,
									or event mask of EVT_COMM_MASK_ENUM and port handler is closed
	***************************************************************/
	comEvtMsk_t open( const char* const comName, comBaud_t baud );

	/**************************************************************
	* @brief Close com-port. (handle only). 
	* Other all variables of class are reset in default value, except user's param
	* You can reopen port letter 
	**************************************************************/
	void close();

	/****************************************************************
	 * @brief Reopen of port with last parameters (port number, baud,...).
	 * @return	<evtMask_t> EVT_NO (0) - if OK,
								or event mask of EVT_COMM_MASK_ENUM and port handler is closed
	***************************************************************/
	comEvtMsk_t reopen();


	/***********************************************************
	****************	 Protected section	********************
	************************************************************/
protected:
	Buffer m_rxBuf;			// buffer for Rx
	Buffer m_txBuf;			// buffer for Rx

	/****************************************************************
	 * @brief Create ComPortWin32 object.
	 * Link com port object with external Rx & Tx Buffers
	 * Rx/Tx Buffers may be external or set dynamically.
	 * @param pRxBuf :<char*> - pointer to external Rx buffer. If =0 - dynamic method
	 * @param pTxBuf :<char*> - pointer to external Tx buffer. If =0 - dynamic method
	 * @param sizeRxBuf:<int> - size of Rx buffer
	 * @param sizeTxBuf:<int> - size of Tx buffer
	***************************************************************/
	ComPortWin32(char* const pRxBuf, char* const pTxBuf,
		int sizeRxBuf, int sizeTxBuf);
	
	/****************************************************************
	 * @brief Delete ComPortWin32 object. Delete handler, buffers if 
	 dynamically created (if not - reset buffer's index)
	***************************************************************/
	~ComPortWin32();

	/*************************************************************
	* @brief For periodically check win32 events for com (events mask must be set before where
	* com parameters was set).
	* checkWin32ComEvents() must be call automatically from time base handleEvent() 
	* if ComPort class using as observed throught IObsTimeBase.
	* The period must be less than time when rx buffer's is full.
	* If EV_RXCHAR event -> read win32 rx buffer while class rxBuf is not full
	* (if full - the rest of data in win32 rx buffer are lost)
	* @return	<evtMask_t> event mask of EVT_COMM_MASK_ENUM
	*************************************************************/
	comEvtMsk_t checkWin32ComEvents();

	/***************************************************************
	* @brief Start of transmit data from tx buffer, while buffer is
	not empty. If write to port error was detected - tx buffer stay on 
	previous state.
	* @return	<evtMask_t> event mask of EVT_COMM_MASK_ENUM
	**************************************************************/
	comEvtMsk_t startTx();

	/****************************************************************
	 * @brief Set event in external variable for detect events in the future.
	 * @param events <evtMask_t> - external variables for result of event
	 * @param mask <evtMask_t> - mask of event from EVT_COMM_MASK_ENUM
	 * Use:	evtMask_t evt; setEvent(evt, EVT_ERR_TX);
	***************************************************************/
	void setEvent(comEvtMsk_t& events, comEvtMsk_t mask);


	/***********************************************************
	****************	 Private section	********************
	************************************************************/
private:
	static constexpr int EVT_CHAR_FICTIVE_VALUE = -1;
	// Char for user char event. Generate event EVT_RX_USER_CHAR
	// if char was detected in input stream.
	// Char must be set as second param as setParam(USE_USER_CHAR_EVENT, '$')
	char m_userChar = EVT_CHAR_FICTIVE_VALUE;
	// Char for end of string char event. Generate event EVT_RX_EOF_CHAR
	// if char was detected in input stream.
	// Char must be set as second param as setParam(USE_EOF_CHAR_EVENT, '\n')
	char m_eofChar = EVT_CHAR_FICTIVE_VALUE;
	// Number of port after last connection
	int m_comNumber = 0;
	// Baud of port after last connection
	comBaud_t m_comBaud = B_NO;
	HANDLE m_hPort = INVALID_HANDLE_VALUE;	// for CreateFile()
	OVERLAPPED m_rxOverlap = { 0 };



};

#endif
