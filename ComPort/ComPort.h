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
class ComPort: public SysComPort_t, public IObsTimeBase
{
public:

	/***********************************************************
	 * @brief	Create ComPort object.
	 * Link com port object with external Rx & Tx Buffers in SysComPort_t path
	 * Rx/Tx Buffers may be external or set dynamically.
	 * Use:	ComPort com(rxBuf, txBuf, sizeof(rxBuf), sizeof(txBuf));
					ComPort com4; - for buffers dynamic memory allocation 
	 * @param pRxBuf :<char*> - pointer to external Rx buffer. If =0 - dynamic method
	 * @param pTxBuf :<char*> - pointer to external Tx buffer. If =0 - dynamic method
	 * @param sizeRxBuf:<int> - size of Rx buffer. If not set - MIN_BUFFER_SIZE
	 * @param sizeTxBuf:<int> - size of Tx buffer. If not set - MIN_BUFFER_SIZE
	************************************************************/
	ComPort(char* const pRxBuf, char* const pTxBuf,
		int sizeRxBuf = MIN_BUF_SIZE, int sizeTxBuf = MIN_BUF_SIZE);

	/***********************************************************
	 * @brief Delete ComPort object.
	 * All observers are deleted. Additionally see descriptor in SysComPort_t path
	************************************************************/
	~ComPort();
	
	/************************************************
	* @brief Print to port c_style string while EOF char 
	* not will be printed
	* @param cstr	<const char*> - c_style string
	* @return	<ComPort&> for use as com<<"ddd"<<"sss";
	***********************************************/
	ComPort& operator<<(const char* cstr);

	/************************************************
	* @brief Print to port integer number string while 
	* EOF char not will be printed
	* @param num	<const int> - integer
	* @return	<ComPort&> for use as com<<"ddd"<<"sss";
	***********************************************/
	ComPort& operator<<(const int num);

	/************************************************
	* @brief Print to port c_style string while EOF char
	* not will be printed
	* @param cstr	<const char*> - c_style string
	* @return	true - OK
	*					false - not ok. In this case EVT_ERR_CRITICAL, EVT_ERR_TX events may be
							transmit to observers
	***********************************************/
	bool print(const char* cstr);

	/************************************************
 * @brief Print to port std::string. The terminator will be send if
 * event char was set in '\0' value where port was configured.
 * @param cstr	<const char*> - std::string
 * @return	true - OK
 *					false - not ok. In this case EVT_ERR_CRITICAL, EVT_ERR_TX events may be
							transmit to observers
***********************************************/
	bool print(std::string str);

	/**************************************************************
	 * @brief	Get string from Rx buffer to external buffer while 
	 * EOF user char not will be copied to out str. Out str allways 
	 * end with '\0'
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

	bool redirectStrTo(ComPort& dstCom, bool fTransfer = true) {
		bool res;
		if (fTransfer)
			res = m_rxBuf.transferStrTo(dstCom.m_txBuf);
		else
			res = m_rxBuf.copyStrTo(dstCom.m_txBuf);
		if (res)
			startTx();
		return res;
	}

	/**********************************************
	* @brief	Add observer for event  to another objects.
	* @param ref <IObserverComPort&> - object that inherits IObsComPort
	***********************************************/
	void addObserver(IObsComPort& obs);

	/**********************************************
	* @brief Remove observer
	* @param ref <IObserverComPort&> - object that inherits IObserverComPort
	***********************************************/
	void removeObserver(IObsComPort& obs);



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
	 * @param isReturnIndAfterStr <bool>:
	 *		if = false or not defined - the function return index in buffer where found start of input string
	 *		if = true - the function return next index in buffer where found end of input string
	 * @param pStartInBuf <int> - start index for search. If 0 or not defined - the seach begins
	 *		with index for read operation
	 * @return <int> - index of buffer or bufResultNG if not found
	*****************************************************/
	int searchInRxBuf(const char* str, bool isReturnIndAfterStr = false, int pStartInBuf = 0);




	/***********************************************************
	****************	 Private section	********************
	************************************************************/
private:
	// pointers for observer objects 
	std::vector< IObsComPort* > m_observers;


	/**********************************************
	 * @brief Notify observers from m_observers list
	 * @param evtCode <evtCode_t> - reason of call. May be used
								by observer for hanle the event
	***********************************************/
	void notifyObservers(comEvtMsk_t evtMask);

	/**********************************************
	 * @brief	*** Event handler for TimeBase class observer's
	 * Periodically check core's events of port, notify of observer if 
	 * core return event
	 * @param Not used
	***********************************************/
	virtual void handleEvent(TimeBase&, TimeBase::evtMask_t) override;



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

	virtual void handleEvent(ComPort&, ComPort::comEvtMsk_t) = 0;
};



// typedef for another parth of program
using comBaud_t				= ComPort::comBaud_t;
using comEvtMsk_t			= ComPort::comEvtMsk_t;
using comCfg_t				= ComPort::comCfg_t;
