#pragma once

#include "SysConst.h"
#include "TimeBase.h"
#include <vector>

/*****************************************************
 * @brief Change include and using according base system.
 ComPortWin32 class working in win32 system
******************************************************/
#ifdef _WIN32
#include "ComPortWin32.h"
using SysComPort_t = ComPortWin32;
#endif


// declaration of interface for observer
class IObsComPort;

/*********************************************************************
* @brief	Class of Com port. The system path embody in SysComPort_t path
*
*********************************************************************/
class ComPort: public SysComPort_t
{
public:

	/***********************************************************
	 * @brief Data about observers
	************************************************************/
	typedef struct{
		IObsComPort * pToObs;	// pointer to observer
		comEvtMsk_t evtMsk;		// mask for generation event for *pToObs observer
	}obsData_t;



#if RX_DATA_DETECT_METHOD == PERIODICALLY_INTERROGATION
	/***********************************************************
	* @brief	Create ComPort object with synchronization with system's clock.
	* The system clock needed if port can not use interrupt for receive and for
	* check events of port the hundler of event must be call periodically.
	* Initialization of Rx/Tx Buffers performed in SysComPort_t path.
	* @param sysClk :<TimeBase> - system clock object.
	* @param sizeRxBuf:<unsigned> - size of Rx buffer. If not set -  SysConst::txBufSize
	* @param sizeTxBuf:<unsigned> - size of Tx buffer. If not set -  SysConst::txBufSize
	************************************************************/
	ComPort(TimeBase& sysClk,
		unsigned sizeRxBuf = SysConst::rxBufSize, unsigned sizeTxBuf = SysConst::txBufSize);

#else
	/***********************************************************
	 * @brief	Create ComPort object.
	 * Initialization of Rx/Tx Buffers performed in SysComPort_t path.
	 * @param sizeRxBuf:<int> - size of Rx buffer. If not set - SysConst::txBufSize
	 * @param sizeTxBuf:<int> - size of Tx buffer. If not set - SysConst::txBufSize
	************************************************************/
	ComPort(unsigned sizeRxBuf = SysConst::rxBufSize, unsigned sizeTxBuf = SysConst::txBufSize);

#endif

	/********************************************************
	* @brief Deleted constructors
	*********************************************************/
	ComPort() = delete;

	ComPort(const ComPort&) = delete;

	ComPort& operator=(const ComPort&) = delete;

	/********************************************************
	* @brief *****		For mov semantics		******************
	*********************************************************/
	ComPort(ComPort&& com) = default;

	ComPort& operator=(ComPort&&) = default;

	/***********************************************************
	 * @brief Delete ComPort object.
	 * All observers of port are deleted. 
	 * Additionally see descriptor in SysComPort_t path
	************************************************************/
	~ComPort();

	/************************************************
	* @brief Print to port std::string while EOF char
	* not will be printed (EOF char set us Event char in port config).
	* If data in buffer starts with '\0' - not copy.
	* @param cstr	<const char*> - std::string
	* @return	true - OK
	*					false - not ok. In this case EVT_ERR_CRITICAL, EVT_ERR_TX events may be
							transmitted to observers
	***********************************************/
	bool print(std::string str);

	/************************************************
	* @brief Print to port c_style string while EOF char 
	* not will be printed (EOF char set us Event char in port config).
	* If data in buffer starts with '\0' - not copy.
	* EVT_ERR_CRITICAL, EVT_ERR_TX events may be 	transmitted to observers
	* if data can't be transmitted
	* @param cstr	<const char*> - c_style string
	* @return	<ComPort&> for use as com<<"ddd"<<"sss";
	***********************************************/
	ComPort& operator<<(const char* cstr);

	/************************************************
	* @brief Print to port integer number.
	* @param num	<const int> - integer
	* @return	<ComPort&> for use as com<<"ddd"<<"sss";
	***********************************************/
	ComPort& operator<<(const int num);

	/**************************************************************
	 * @brief	Get string from Rx buffer to external buffer while 
	 * EOF user char not will be copied to out str (EOF char set us 
	 * Event char in port config). Out str allways end with '\0'.
	 * If data in buffer starts with '\0' - not copy.
	 * If function complete with error, the Rx buffer returns
	 * to previous state.
	 * @param str <char*> - external buffer for destination.
	 * @param sizeStr <int> - size of external buffer
	 * @return next buffer's index for read operation
	 *						or 	bufResultIsNG - the end of string not
	 *						found before buffer is empty
	**************************************************************/
	int getRxStr( char* str, int size);

	/**************************************************************
	 * @brief	Redirect received string (from Rx buffer) from port to 
	 * anather port (Tx) while EOF user char not will be redirected 
	 * (EOF char set us Event char in port config).
	 * If data in source buffer starts with '\0' - not copy.
	 * If function complete with error, the buffers returns
	 * to previous state.
	 * @param dstCom <ComPort> - port of destination
	 * @param fTransfer <bool>	= true - transfer data with delete data in source
	 *													=false - copy data
	 * @return	true - Transfer is success
	 *					false - the EOF char not found in buffer of
	 *						source while copy operation. All buffers
	 *						will be return to previous state
	***************************************************************/
	bool redirectStrTo(ComPort& dstCom, bool fTransfer = true);

	/****************************************************
	 * @brief	Search symbol in Rx buffer in not readed range.
	 * Buffer's indexes don't change.
	 * @param byte <char> - char for search
	 * @return <int> - index of buffer or bufResultNG if not found
	*****************************************************/
	int searchInRxBuf(char byte);

	/****************************************************
	 * @brief	Search c_style string in RX buffer in not readed range.
	 * Buffer's indexes don't change.
	 * @param str <const char* str> - string for search
	 * @param pStartInBuf <int> - start index for search. If 0 or not defined - the seach begins
	 *		with index for read operation
	 * @param isReturnIndAfterStr <bool>:
	 *		if = false or not defined - the function return index in buffer where found start of input string
	 *		if = true - the function return next index in buffer where found end of input string
	 * @return <int> - index of buffer or bufResultNG if not found
	*****************************************************/
	int searchInRxBuf(const char* str, int pStartInBuf = 0, bool isReturnIndAfterStr = false);

	/**********************************************
	* @brief	Add observer for event  to another objects.
	* @param obs <IObserverComPort&> - object that inherits IObsComPort
	* @param evtMsk <comEvtMsk_t> - mask of events for observer. ComPort
	*					will be event to observer only mask is set
	* @return	true - ok
	***********************************************/
	bool addObserver(IObsComPort& obs, const comEvtMsk_t evtMsk);

	/**********************************************
	* @brief Remove observer
	* @param ref <IObserverComPort&> - object that inherits IObserverComPort
	***********************************************/
	void removeObserver(const IObsComPort& obs);



	/***********************************************************
	****************	 Private section	********************
	************************************************************/
private:

	// pointers for observer objects and masks 
	std::vector< obsData_t > m_observers;

	/**********************************************
	 * @brief Notify observers from m_observers list
	 * @param evtMask <comEvtMsk_t> - reason of call. May be used
								by observer for handler the event
	***********************************************/
	virtual void notifyObservers(comEvtMsk_t evtMask) override final;

};



/*********************************************************************
**********************************************************************
*********************************************************************/


/*********************************************************************
 * @brief Interface class for events organization for observers
 * The Port objects (this observable class) must add observer throught
 * addObserver() function. Observers must inherit IObsComPort class
 * as public and must redefine handleEvent(ComPort&) function as
 * virtual void handleEvent( ComPort& ref) override.
 * If ComPort's object call internal function notifyObservers() -> call all
 * handleEvent(ComPort&) function for all observers.
 * The observer may read obsEvtCode as addition parameter of call event function
 * via this->obsEvtCode and handle the cause of the event.
**********************************************************************/
class IObsComPort
{
public:

	virtual void handleEvent(ComPort&, const ComPort::comEvtMsk_t) = 0;
};



// typedef for another parth of program
using comBaud_t				= ComPort::comBaud_t;
using comEvtMsk_t			= ComPort::comEvtMsk_t;
using comCfg_t				= ComPort::comCfg_t;
