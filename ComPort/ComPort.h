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
		int sizeRxBuf = MIN_BUFFER_SIZE, int sizeTxBuf = MIN_BUFFER_SIZE);

	/***********************************************************
	 * @brief Delete ComPort object.
	 * All observers are deleted. Additionally see descriptor in SysComPort_t path
	************************************************************/
	~ComPort();

	/**************************************************************
	* @brief Get C_style str from rx buffer
	* @param str 				<char*>	- Pointer to dst external buffer
	* @param size				<int>		- size of dst buffer
	* @param ensSymbol	<ensSymbol> - end symbol of str like '\n' or '\0'
	* @return	true - ok
	*			false - end symbol not found befor end of rx buffer
	**************************************************************/
	bool getRxStr( char* str, int size, char ensSymbol = '\0' );

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

	/************************************************
	 * @brief Print to port c_style string
	 * @param cstr	<const char*> - c_style string
	 * @param endSymbol <char> - end symbol of string. End symbol'\0' not transmitting
	 * @return	true - OK
	 *					false - not ok. Inthis case EVT_ERR_CRITICAL, EVT_ERR_TX events may be 
										transmit to observers
	***********************************************/
	bool print(const char* cstr, char endSymbol = '\0');


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
