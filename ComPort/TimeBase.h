#pragma once
#include <vector>
#include <string>
#include <assert.h>
#include "SysConst.h"
/*****************************************************
 * @brief Change include and using according base system.
 Win32TimeBase class working in win32 system
******************************************************/
#ifdef _WIN32
#include "TimeBaseWin32.h"
using TimeBase_t = TimeBaseWin32;
#endif

class IObsTimeBase;

/**************************************************************
 * @brief Time base class.
 For generation general clocks for system from SysConst::clkTimeBase to 1s
 May be used for synchronization blocks of system.
 Generation of events - according set of mask when an object was created or when
 observer was added.
***************************************************************/
class TimeBase : public TimeBase_t
{
	using tBaseEvtMskInt_t = uint8_t;

public:

	// mask of events. 
	typedef enum EVT_TIME__ENUM: tBaseEvtMskInt_t
	{
		EVT_NO,
		EVT_1US		= (1 << 0),
		EVT_10US	= (1 << 1),
		EVT_100US	= (1 << 2),
		EVT_1MS		= (1 << 3),
		EVT_10MS	= (1 << 4),
		EVT_100MS	= (1 << 5),
		EVT_1S		= (1 << 6),
		EVT_10S		= (1 << 7),
	}tBaseEvtMsk_t;

	// For Observer
	typedef struct {
		IObsTimeBase* pToObs;		// pointer to observer
		tBaseEvtMsk_t evtMsk;	// mask for generation event for this observer
	}tBaseObs_t;


	/*******************************************************************
	* @brief Create object by default.
	********************************************************************/
	TimeBase()	{}

	/*******************************************************************
	 * @brief Create object.
	 * @param evtMask <evtMask_t> - sets for generation of time interval
	 *				May be set as (EVT_1MS | EVT_10US).  The minimum period of 
	 *				event must be >= then SysConst::clkTimeBase
	********************************************************************/
	TimeBase(tBaseEvtMsk_t evtMask);

	/*******************************************************************
	 * @brief	Add observer for event to another objects.
	 * @param obs <IObsTimeBase&> - object of observer that inherits IObsTimeBase
	 * @param evtMsk <evtMask_t> - mask of events for observer. TimeBase
	 *					will be event to observer only mask is set
	 * @return	true - ok
	 *					false - desired mask < SysConst::clkTimeBase and can't be set
	********************************************************************/
	bool addObserver(IObsTimeBase& obs, tBaseEvtMsk_t evtMsk);
	

	/*******************************************************************
	 * @brief Remove observer form TimeBase object
	 * @param obs <IObsTimeBase&> - object that inherits IObsTimeBase
	********************************************************************/
	void removeObserver(IObsTimeBase& obs);

	/*******************************************************************
	* @brief Get observers vector for TimeBase object  
	* @return 
	********************************************************************/
	std::vector<tBaseObs_t>& getObservers();

	/*******************************************************************
	 * @brief Main loop using from parent class SysTimeBase_t.
	 Must be called periodicaaly in main loop circle
	 like as while(1)
	********************************************************************/
	//void loop();


	/*******************************************************************
	 * @brief		Get system time in string format "hh:mm:ss"
	 * @return <string>=""hh:mm:ss""
	********************************************************************/
	std::string& getTime();

	/*******************************************************************
	 * @brief		Get system date in string format "dd:mm:yy"
	 * @return <string>=""hh:mm:ss""
	********************************************************************/
	std::string& getDate();




private:
	// for store event's mask, set when creating an object or 
	// observer is added.
	// If bit in m_evtMsk = 1 - event as EVT_1MS was permitted
	tBaseEvtMsk_t m_evtMsk = EVT_NO;


	// object Observers 
	std::vector<tBaseObs_t>m_observers;
	
	// for system time
	struct SYS_TIME_STRUCT {
		uint8_t sec;
		uint8_t min;
		uint8_t hour;
		uint8_t day;
		uint8_t month;
		uint8_t year;
	}m_time = { 0 };

	// for forming system tyme in string format
	std::string m_timeStr = "";
	std::string m_dateStr = "";


	/******************************************************************
	* @brief Add event mask for generate timer's events.
	* @param evtMsk <tBaseEvtMsk_t> - mask of event. May be set as EVT_10US | EVT_10US
	* @return	true - ok
	*					false - desired mask < SysConst::clkTimeBase and can't be set
	*******************************************************************/
	bool addEvent(tBaseEvtMsk_t evtMsk);

	/******************************************************************
	* @brief Remove event mask for generate timer's events.
	* @param evtMsk <tBaseEvtMsk_t> - mask of event for delete. May be set as EVT_10US | EVT_10US
	*******************************************************************/
	void removeEvent(tBaseEvtMsk_t evtMsk);

	/**********************************************
	* @brief Notify observers from m_observers list, if observer
	* was subscribed to event
	* @param events <obsEvtCode_t> - mask for events that occured 
	(reason of call. May be used by observer for hanle the event
	***********************************************/
	void notifyObservers(const tBaseEvtMsk_t events);

	/******************************************************************
	 * @brief	*** Event handler for inherited TimeBase_t (like us TimeBaseWin32) class
	 * Increment the system time (call clkIncrement()) and call observer's handler, 
	 * if an event occured
	 * Call automatically every SysConst::clkTimeBase
	******************************************************************/
	virtual void handleEvent() override;

	/******************************************************************
	 * @brief Increment system time and forming events of time interval 
	 * wich are allowed during configuration (where object was created
	 * or observer was added)
	 * @return <evtMask_t> - mask for events that occured
	******************************************************************/
	inline tBaseEvtMsk_t clkIncrement();
	
	/******************************************************************
	 * @brief Convertion field of system time data to std::string
	 * @param num <uint> - number for convertion
	 * @return char* = input char* 
	*******************************************************************/
	std::string convTimeFieldToStr(unsigned int num);

	/******************************************************************
	 * @brief Checks mask if can be setted
	 * @param evtMsk 
	 * @return	true - ok
	 *					false - desired mask < SysConst::clkTimeBase and can't be set
	*******************************************************************/
	bool checkEvtMskForSet(tBaseEvtMsk_t evtMsk);

	friend tBaseEvtMsk_t& operator|(tBaseEvtMsk_t& l, const tBaseEvtMsk_t& r);


};


/*********************************************************************
 * @brief Interface class for events organization for observers.
 * TimeBase objects (this observable class) must add observer throught
 * addObserver() function. Observers must inherit IObsTimeBase class
 * as public and must redefine handleEvent(TimeBase&, TimeBase::evtMask_t) function as
 * virtual void handleEvent(TimeBase&, TimeBase::evtMask_t) override.
 * If TimeBase's object call internal function notifyObservers() -> call all
 * handleEvent(TimeBase&, TimeBase::evtMask_t) function for all observers.

 * The observer may read evtMask_t as addition parameter of call event function
**********************************************************************/

class IObsTimeBase
{
public:

	virtual void handleEvent(TimeBase&, TimeBase::tBaseEvtMsk_t) = 0;
};

