
#include "STimer.h"


static std::vector<STimer*>	m_pTimers;



//STimer clkTimeBase( SysConst::clkTimeBase, SysConst::clkTimeBase, STimer::START );

/***************************************************************************************/
STimer::STimer( uint32_t _delay, uint32_t _period, S_TIMER_STATE_ENUM _state )
	:m_delay( _delay / SysConst::clkTimeBase ), m_period(_period / SysConst::clkTimeBase), m_state(_state)
{
	m_pTimers.push_back( this );
//	if(m_pTimers.back() == &clkTimeBase)
}

/*********************************************************/
void STimer::erace()
{
	for( int ind = 0; ind < m_pTimers.size(); ++ind ) {
		if( m_pTimers[ind] == this ) {
			m_pTimers.erase( m_pTimers.begin() + ind );
		}
	}
}

/*********************************************************/
void STimer::start( )
{
	m_delay = m_period;
	m_state = START;
}

/*********************************************************/
void STimer::stop( )
{
	m_state = STOP;
}

/*********************************************************/
int64_t STimer::getTimeLeft() const
{
	return ((int64_t)m_delay * SysConst::clkTimeBase);
}

/*********************************************************/
int64_t STimer::getTimePassed( ) const
{
	if( m_period ) {
		if( (m_period - 1) > m_delay ) {
			return (((int64_t)m_period - 1 - m_delay) * SysConst::clkTimeBase);
		}
	}
	return (-1);
}

STimer::STimerState_t STimer::getState() const
{
	return m_state;
}

/*********************************************************/
void STimer::loop()
{


	for( auto tim : m_pTimers ) {
		if( tim->m_state == START ) {
			if( tim->m_delay ) {
				--tim->m_delay;
			}
			else {
				tim->notifyObservers(IObsSTimer::PERIOD_EVENT );
				if( tim->m_period )
					tim->m_delay = tim->m_period - 1;
				else
					tim->m_state = DONE;
			}
		}
	}
}
