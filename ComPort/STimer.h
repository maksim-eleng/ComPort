

/*
 *  Created on: 14.09.2019
 *      Author: Maksim
-------------------------------------------------------------
����������� ������ �����:
- ����������� ����� ��� ����� �������� �����
- �������� � ������ ������������ ��� �������������� �������
- ����� ��� ��������� � START, STOP, �������� ������
- ��������������� �� ��������� �����
- ��������� �������
- �������� �������
- ������� ������� � ������� � ������� ��������
-------------------------------------------------------------
����� ������ ������:
����������
// ������ �������� ����������� ������ �������, ms
#define SYS_CLK_PERIOD	1
��������� ���������� ���� S_TIMER_STRUCT timer.
����������� ������� ����������� �������� (��� ����� ������� ����� ��������� �������)
void Clk1(void){ InvBit(PORTB, 0); }
�������������� ������ (��������� ��������� timer1)
stimerCreate(&timer, 5, 10, SYS_CLK_PERIOD, START, Clk1);
� ��������� ����� (������� ��������� SYS_CLK_PERIOD) �������� �-�
stimerLoop(), ������� ����� �������� Clk1() � �������� 10 � � ��������� �����
������ ������� 5.
���� State!=SART - ����� ������ ����� ��� ����� � ����������  Clk1()
-------------------------------------------------------------
*/

//#include <stdint.h>
#include "TimeBase.h"



class STimer;
class IObsSTimer
{
public:
	typedef enum OBS_EVT_CODES_ENUM
	{
		NO_CODE,
		PERIOD_EVENT,

	}obsEvtCode_t;

	obsEvtCode_t obsEvtCode;

	virtual void handleEvent( STimer& ) = 0;
};




class STimer
{
public:

	/************************************************
	 * @brief State of timers
	*************************************************/
	typedef enum S_TIMER_STATE_ENUM
	{
		STOP,				// stop timer state
		START,			// staer timer.
		DONE, 			// used. This state can be set after single start
//		DELETED			// timer was deleted. The new timer can be created instead this
	}STimerState_t;


	/**************************************************************************************
	* @brief	***		Cerate and inicialization timer		***
		If timer was deleted earlier (status = DELETED), new timer will be created instead the old
		Timer's modes:
		1.	Single - delay != 0, period = 0
		2.	Periodical, first event at once: delay = 0, period != 0
		3.	Periodical, first event after delay: delay != 0, period != 0
	* @param _delay 	<uint32_t> - delay of start timer
	* @param _period	<uint32_t> - period of timer.
	* @param _state 	<uint32_t> - initial state (START or STOP).
	***************************************************************************************/
	STimer( uint32_t _delay, uint32_t _period, S_TIMER_STATE_ENUM _state = STOP );

	/********************************************************
	* @brief	Delete of timer object from scan sheet. 
						The timer's object will not be deleted.
	*********************************************************/
	void erace();


	/********************************************************
	* @brief  Start of timer. May be used for restart timer
	*********************************************************/
	void start( );

	/********************************************************
	* @brief  Stop of timer.
	*********************************************************/
	void stop( );

	/********************************************************
	* @brief  Get time (in mks) left to notify.
	* @return - time in mks
	*********************************************************/
	int64_t getTimeLeft() const;

	/********************************************************
	* @brief  Get time (in mks) from last reinitialization or start of timer.
	* @return - time in mks
	*********************************************************/
	int64_t getTimePassed( ) const;

	/********************************************************
	* @brief  Get state of timer.
	* @return - S_TIMER_STATE_ENUM state (STOP, START, DONE)
	*********************************************************/
	STimerState_t getState( ) const;

	/********************************************************
	* ***   Loop function. Must be call periodically with
	SysConst::clkTimeBase period. If interrupt 1ms as time base available 
	(in like as STM32 system) loop() will be synchronized with this interrupt.
	In case when interrupt not available (like as win32) - periodically scan will be used	****
	Example:

	*********************************************************/
	static void loop( void );


private:
	uint32_t			m_delay;	//����� ����� ����� ��������� �-� void (*pFunc)(void)
	uint32_t			m_period;	//������ ����������. ���� ==0 ->����� 1-�������� �������
	STimerState_t	m_state;	//��������� ������� - STOP, START,  ������������

	static volatile TimeBase clkTimeBase;

	std::vector<IObsSTimer*>m_observers;

	/**********************************************
		* @brief Notify observers from m_observers list
	***********************************************/
	void notifyObservers( IObsSTimer::obsEvtCode_t code )
	{
		for(auto& observer : m_observers) {
			observer->obsEvtCode = code;
			observer->handleEvent( *this );
		}
	}


};

