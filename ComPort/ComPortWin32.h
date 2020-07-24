#pragma once

#ifdef _WIN32

#include "Buffer.h"
#include "TimeBase.h"


// if interrupts for Rx data from port is not available, for receive
// data periodically interrogation must be used.
#define RX_DATA_DETECT_METHOD		PERIODICALLY_INTERROGATION


class ComPortWin32: public IObsTimeBase
{
public:

	static constexpr auto COM_NOT_CFG = -1;

	/************************************************************
	 * @brief Com port event masks. Must be set by means of setEvent()
	 in com module only. Events mask may be check in external path.
	*************************************************************/
	typedef enum EVT_COMM_WIN32_MASK_ENUM : uint16_t
	{
		EVT_NO,
		// User char was detected in com-port stream
		// User char set throught setParam(USE_USER_CHAR_EVENT, '$')
		EVT_RX_USER_CHAR			= (1 << 0),
		// Win32 rx buffer if full more than 80%
		EVT_RX80_FULL					= (1 << 1),
		// CTS changed state
		EVT_CTS_CHANGED				= (1 << 2),
		// DSR changed state
		EVT_DSR_CHANGED				= (1 << 3),
		// Line status error occurred
		EVT_ERR_LINE					= (1 << 4),
		// Error when tx. Last data not transmitted
		EVT_ERR_TX						= (1 << 5),
		// Rx buffer overflow when write operation
		EVT_ERR_RX_BUF_OVF		= (1 << 6),
		// Invalid parameter where set port parameters was detected
		EVT_ERR_INVALID_PARAM	= (1 << 7),
		// Critical errors as com not ready, not open, not handle,...
		EVT_ERR_CRITICAL			= (1 << 8),
		EVT_END								= EVT_ERR_CRITICAL,
	}comEvtMsk_t;
	 
	/*********************************************************
	 * @brief Baud rate. For config Com-port's 
	**********************************************************/
	typedef enum BAUD_ENUM
	{
		B_NOT_CFG	= COM_NOT_CFG,
		B_1200		= CBR_1200,
		B_2400		= CBR_2400,
		B_4800		= CBR_4800,
		B_9600		= CBR_9600,
		B_38400		= CBR_38400,
		B_115200	= CBR_115200,
	}comBaud_t;

	/*********************************************************
	 * @brief Parity variante. For config Com-port's 
	**********************************************************/
	typedef enum PARITY_ENUM
	{
		P_NO		= NOPARITY,
		P_ODD		= ODDPARITY,
		P_EVEN	= EVENPARITY,
		P_MARK	= MARKPARITY,
		P_SPACE	= SPACEPARITY,
		P_NOT_CFG = COM_NOT_CFG,
	}comParity_t;

	/*********************************************************
	 * @brief Numbers of stop bit. For config Com-port.
	**********************************************************/
	typedef enum STOP_BIT_ENUM
	{
		SBIT_ONE		= ONESTOPBIT,
		SBIT_ONE5		= ONE5STOPBITS,
		SBIT_TWO		= TWOSTOPBITS,
		SBIT_NOT_CFG = COM_NOT_CFG,
	}comStopBit_t;

	/*********************************************************
	 * @brief DTR line control. For config Com-port.
	**********************************************************/
	typedef enum DTR_CONTROL_ENUM
	{
		DTR_DISABLE		= DTR_CONTROL_DISABLE,		// DTR out =0ff if COM-port is opened. State can be changed by function EscapeCommFunction()
		DTR_ENABLE		= DTR_CONTROL_ENABLE,			// DTR out =0n if COM-port is opened. State can be changed by function EscapeCommFunction()	
		DTR_HANDSHAKE = DTR_CONTROL_HANDSHAKE,	// DTR out = 0n / Off automatically if COM - port is opened / closed
		DTR_NOT_CFG		= COM_NOT_CFG,
	}comDTRControl_t;

	/*********************************************************
	 * @brief RTS line control. For config Com-port.
	**********************************************************/
	typedef enum RTS_CONTROL_ENUM
	{
		RTS_DISABLE		= RTS_CONTROL_DISABLE,
		RTS_ENABLE		= RTS_CONTROL_ENABLE,
		RTS_HANDSHAKE = RTS_CONTROL_HANDSHAKE,
		RTS_TOGGLE		= RTS_CONTROL_TOGGLE,
		RTS_NOT_CFG		= COM_NOT_CFG,
	}comRTSControl_t;

	///*********************************************************
	//* @brief	
	//Event's mask for toggle WIN system's event mask. For config Com-port only
	//and detect event in checkEventWin32
	//**********************************************************/
	typedef enum EVENT_SET_MASK_ENUM 
	{
		EVTSET_NOT_SET			= 0,
		// Generate event if any char was received
		EVTSET_RX_CHAR			= EV_RXCHAR,
		// Generate event if evtChar char was received
		// Event char is set throught second parameter cfgValue (e.g. '\n')
		EVTSET_RX_USER_CHAR	= EV_RXFLAG,
		// Event if win system Rx Buffer is full by 80%
		EVTSET_RX80FULL			= EV_RX80FULL,
		EVTSET_CTS_CHANGED	= EV_CTS,		// CTS input changed
		EVTSET_DSR_CHANGED	= EV_DSR,		// DSR input changed
		EVTSET_LINE_ERR			= EV_ERR,		// Errors frame, overrun ore RxParity was detected
	}comEvtSetMsk_t;

	/**********************************************************
	 * @brief Set parameters struct of port.
	The object must be created before setParam(comCfg_t&) use
	***********************************************************/
	class comCfg_t
	{
	public:
		/***	Connection for flow control		**********/
		// 4(DTR - Data terminal ready)--->-- - 6 (DSR - Data set ready)
		// 7(RTS - Request to Send)--->-- - 8 (CTS - Clear to Send - готовность приема)
		// 8 (CTS - Clear to Send - -- - < -- - 7 (RTS - Request to Send)
		// Libe DTR---->----DSR

		comBaud_t baud;									// Baud. Use BAUD_ENUM
		int32_t number					: 7;		// number of port
		int32_t byteSize				: 5;		// =4...8 bit
		int32_t fOutxDsrFlow		: 2;		// =1 - transmit only if DSR input = On. 
		int32_t fOutxCtsFlow		: 2;		// =1 - transmit if CTS input(on PC) (receiver ready) = on.
		int32_t fDsrSensitivity : 2;		// =1 - receive only if DSR input = On.	
		int32_t fNull						: 2;		// =1 - in Rx stream '\0' will be ignored
		comDTRControl_t controlDTR: 2;	// control DTR out. Use DTR_CONTROL_ENUM
		comRTSControl_t controlRTS: 3;	// control RTS out. Use RTS_CONTROL_ENUM
		comStopBit_t stopBits			: 2;	// Num of stop bits. Use STOP_BIT_ENUM
		// if != NO - fParity will be to On state
		comParity_t parity				: 3;	// Parity check. Use PARITY_ENUM
		char  parityChar;					// != -1 - replace on char if parity error & parity != NO
		char  evtChar;						// != -1 - genegate event EVT_RX_USER_CHAR if byte was received
		comEvtSetMsk_t evtSet;		// events mask for generate event

		/********************************************************
		 * @brief Constructor for default set val in new object
		* Instruction about DCB:
		* http://vsokovikov.narod.ru/New_MSDN_API/Comm_res/str_dcb.htm
		* https://ru.wikibooks.org/wiki/COM-%D0%BF%D0%BE%D1%80%D1%82_%D0%B2_Windows_%28%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5%29
		*********************************************************/
		comCfg_t()
		{
			baud = B_NOT_CFG;						// Baud. Use BAUD_ENUM
			number = COM_NOT_CFG;
			byteSize = COM_NOT_CFG;
			fOutxDsrFlow = COM_NOT_CFG;			// =1 - transmit only if DSR input = On. 
			fOutxCtsFlow = COM_NOT_CFG;			// =1 - transmit if CTS input(on PC) (receiver ready) = on.
			fDsrSensitivity = COM_NOT_CFG;	// =1 - receive only if DSR input = On.
			fNull = COM_NOT_CFG;						// =1 - in Rx stream '\0' will be ignored
			controlDTR = DTR_NOT_CFG;				// control DTR out
			controlRTS = RTS_NOT_CFG;				// control RTS out
			stopBits = SBIT_NOT_CFG;
			parity = P_NOT_CFG;
			parityChar = COM_NOT_CFG;		// != -1 - replace on char if parity error & parity != NO
			evtChar = COM_NOT_CFG;			// != -1 - genegate event EVT_RX_USER_CHAR if byte was received
			evtSet = EVTSET_NOT_SET;		// not events as default
		}
	};

	// operator | for set events mask us EVTSET_RX_CHAR | EVTSET_RX_USER_CHAR
	// Using when setting up when setParam() used
	friend comEvtSetMsk_t operator|(comEvtSetMsk_t l, comEvtSetMsk_t r);

	// operator |= for set events mask us setEvtMsk |= EVTSET_RX_CHAR;
	// Using when setting up when setParam() used
	friend void operator|=(comEvtSetMsk_t& l, const comEvtSetMsk_t& r);

	/***************************************************************
	 * @brief Set parameters of com-port. May be used after create object of port and
	 * if port is opened (if not - the file descriptor not defined).
	 * Configuration DCB, timeouts, events for WIN32 port.
	 * @param param <comCfg_t> - external struct of config. Must be set previously
	 * @return <comEvtMsk_t>:	EVT_NO - set completted
														EVT_ERR_INVALID_PARAM - not all parameters are set ore the settings are not correct
	****************************************************************/
	comEvtMsk_t setParam(comCfg_t& param);

	/**************************************************************
	* @brief Open COM-port.
	* Parameters will be set letter via setParam() function
	* Creates a file descriptor. 
	* Timings set as: for Read 1 char - max, the rest coeffichience - 0
	* (for without delays and wait operation).
	* Description in http ://vsokovikov.narod.ru/New_MSDN_API/Menage_files/fn_createfile.htm
	* https://ru.wikibooks.org/wiki/COM-%D0%BF%D0%BE%D1%80%D1%82_%D0%B2_Windows_%28%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5%29
	* @param comNum	<int> - number of com port
	* @param baud		<comBaud_t> - BaudRate. The BAUD_ENUM must be used
	* @return				<evtMask_t> EVT_NO (0) - if OK, 
									or EVT_ERR_CRITICAL and port handler is closed
	***************************************************************/
	comEvtMsk_t open( int comNum, comBaud_t baud);

	/**************************************************************
	* @brief Open COM-port.
	* Parameters will be set letter via setParam() function
	* Creates a file descriptor.
	* Timings set as: for Read 1 char - max, the rest coeffichience - 0
	* (for without delays and wait operation).
	* Subscribe to sysClk with 10ms period
	* Description in http ://vsokovikov.narod.ru/New_MSDN_API/Menage_files/fn_createfile.htm
	* https://ru.wikibooks.org/wiki/COM-%D0%BF%D0%BE%D1%80%D1%82_%D0%B2_Windows_%28%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5%29
	* @param comName	<const char* const> - number of com port in the form "COM1" or "\\.\COM1"
	* @param baud			<comBaud_t> - BaudRate. The BAUD_ENUM must be used
	* @return					<evtMask_t> EVT_NO (0) - if OK,
										or EVT_ERR_CRITICAL and port handler is closed
	***************************************************************/
	comEvtMsk_t open( const char* const comName, comBaud_t baud );

	/**************************************************************
	* @brief Close com-port. (handle only). 
	* Other all variables of class are reset in default value, except user's param
	* The observer for sysClk will be deleted
	* You can reopen port letter 
	**************************************************************/
	void close();

	/****************************************************************
	 * @brief Reopen of port with last parameters (port number, baud,...).
	 * @return	<evtMask_t> EVT_NO (0) - if OK,
							or EVT_ERR_CRITICAL and port handler is closed
	***************************************************************/
	comEvtMsk_t reopen();

	/**************************************************************
	 * @brief Check port is opened or not
	 * @return true if port is opened 
	***************************************************************/
	bool isPortOpened() const;

	/*****************************************************************
	 * @brief Return number of received Event user char, if used 
	 * (user char must be set in setPAram() when config port)
	 * @return number of received Event user char 
	****************************************************************/
	int userCharGetReceivedCounter() const;

	/*****************************************************************
	 * @brief Must be call after every "user char received" handler 
	****************************************************************/
	void userCharHandled();

	/*****************************************************************
	 * @brief Return Number of port or COM_NOT_CFG_VALUE if not 
	 * configured. If port was opened, but not now - return last 
	 * configured.
	 * @return Number of port
	****************************************************************/
	int getPortNumber() const;

	/*****************************************************************
	 * @brief Return Baud rate of port or COM_NOT_CFG_VALUE if not
	 * configured. If port was opened, but not now - return last
	 * configured.
	 * @return Baud rate of port
	****************************************************************/
	int getBaud() const;



	/***********************************************************
	****************	 Protected section	********************
	************************************************************/
protected:
	Buffer m_rxBuf;			// buffer for Rx
	Buffer m_txBuf;			// buffer for Rx
	comCfg_t m_cfg;			// last cfg of port. For reopen()
	/****************************************************************
	 * @brief Create ComPortWin32 object.
	 * Link com port object with external Rx & Tx Buffers
	 * Rx/Tx Buffers may be external or set dynamically.
	 * @param pRxBuf :<char*> - pointer to external Rx buffer. If =0 - dynamic method
	 * @param pTxBuf :<char*> - pointer to external Tx buffer. If =0 - dynamic method
	 * @param sizeRxBuf:<int> - size of Rx buffer
	 * @param sizeTxBuf:<int> - size of Tx buffer
	***************************************************************/
	ComPortWin32(TimeBase& sysClk, 
		char* const pRxBuf, char* const pTxBuf,
		int sizeRxBuf, int sizeTxBuf);

	/****************************************************************
	 * @brief Delete ComPortWin32 object. Delete handler, buffers if
	 dynamically created (if not - reset buffer's index)
	***************************************************************/
	~ComPortWin32();

	/****************************************************************
	 * @brief Set event in external variable for detect events in the future.
	 * @param events <evtMask_t> - external variables for result of event
	 * @param mask <evtMask_t> - mask of event from EVT_COMM_MASK_ENUM
	 * Use:	evtMask_t evt; setEvent(evt, EVT_ERR_TX);
	***************************************************************/
	void setEvent(comEvtMsk_t& events, comEvtMsk_t mask);

	/***************************************************************
	* @brief Start of transmit data from tx buffer, while buffer is
	not empty. If write to port error was detected - tx buffer stay on
	previous state.
	* @return	<evtMask_t> event mask of EVT_COMM_MASK_ENUM
	**************************************************************/
	comEvtMsk_t startTx();

	/************************************************
	* @brief Print to port c_style string while EOF char
	* not will be printed (EOF char set us Event char in port config).
	* If data in buffer starts with '\0' - not copy.
	* @param cstr	<const char*> - c_style string
	* @return	true - OK
	*					false - not ok. In this case EVT_ERR_CRITICAL, EVT_ERR_TX events may be
							transmit to observers
	***********************************************/
	bool print(const char* cstr);



	/***********************************************************
	****************	 Private section	********************
	************************************************************/
private:
	HANDLE m_hPort = INVALID_HANDLE_VALUE;	// for CreateFile()
	OVERLAPPED m_rxOverlap = { 0 };					// struct of WIN32 events for ReadFile() 
	int m_evtCharCnt = 0;											// counter received m_cfg.evtChar, if used
	// pointer to clock of system
	TimeBase* m_pSysClk;


	/******************************************************
	 * @brief Subscribe object of port to clock synchronization system
	 * Must be used if object was created without clock of system parameter
	 * (see constructor)
	 * @param sysClk <TimeBase> - object of clock synchronization system
	 * @param evtMsk <tBaseEvtMsk_t> - mask of time events
	*****************************************************/
	bool subscribe(TimeBase& sysClk, TimeBase::tBaseEvtMsk_t evtMsk);

	/*************************************************************
	* @brief For periodically check win32 events for com (events mask must be set before where
	* com parameters was set).
	* checkCoreEvents() must be call automatically from time base handleEvent()
	* if ComPort class using as observed throught IObsTimeBase.
	* The period must be less than time when rx buffer's is full.
	* If EV_RXCHAR event -> read win32 rx buffer while class rxBuf is not full
	* (if full - the rest of data in win32 rx buffer are lost)
	* @return	<evtMask_t> event mask of EVT_COMM_MASK_ENUM (all)
	*************************************************************/
	comEvtMsk_t checkCoreEvents();

	/**********************************************
	 * @brief	*** Event handler for TimeBase class observer's
	 * Periodically check core's events of port, notify of observer if
	 * core return event from port
	 * @param Not used
	***********************************************/
	virtual void handleEvent(TimeBase&, TimeBase::tBaseEvtMsk_t) override;

	/**********************************************
	 * @brief Notify observers from m_observers list
	 * @param evtMask <comEvtMsk_t> - reason of call. May be used
								by observer for hanle the event
	***********************************************/
	virtual void notifyObservers(comEvtMsk_t evtMask) = 0;


};

#endif
