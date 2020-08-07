
#include "TimeBase.h"


/**************************************************/
TimeBase::TimeBase(tBaseEvtMsk_t evtMsk)
{
	addEvent(evtMsk);
}
/**************************************************/
bool TimeBase::addObserver(IObsTimeBase& obs, tBaseEvtMsk_t evtMsk)
{
	if (addEvent(evtMsk)) {
		// search obs. Add mask if found or add obs with current musk, if not
		for (auto& _obs : m_observers) {
			if (_obs.pToObs == &obs) {
				_obs.evtMsk = static_cast<tBaseEvtMsk_t>(_obs.evtMsk | static_cast<tBaseEvtMsk_t>(evtMsk));
				return true;
			}
		}
		tBaseObs_t _obj = { &obs, evtMsk };
		m_observers.push_back(_obj);
		return true;
	}
	return false;
}

/**************************************************/
void TimeBase::removeObserver(const IObsTimeBase& obs)
{
	std::vector<tBaseObs_t>::iterator it;
	std::vector<tBaseObs_t>::iterator itForDel;
	tBaseEvtMsk_t resMsk = EVT_NO;
	for (it = m_observers.begin(); it != m_observers.end(); ++it) {
		if (&obs != it->pToObs) {
			resMsk = static_cast<tBaseEvtMsk_t> (resMsk | it->evtMsk);
		}
		else {
			itForDel = it;
		}
	}
	if (itForDel._Ptr) {
		m_observers.erase(itForDel);
		removeEvent(static_cast<tBaseEvtMsk_t>(~resMsk));
	}
}

/**************************************************/
const std::vector<TimeBase::tBaseObs_t>& TimeBase::getObservers()
{
	return m_observers;
}

/**************************************************/
void TimeBase::notifyObservers(tBaseEvtMsk_t evtMsk)
{
	for (auto& obs : m_observers) {
		tBaseEvtMsk_t mask = static_cast<tBaseEvtMsk_t>(obs.evtMsk & evtMsk);
		if (mask) {
			obs.pToObs->handleEvent(*this, mask);
		}
	} 
}

/**************************************************/
const std::string& TimeBase::getTimeStr()
{
	m_timeStr = convTimeFieldToStr(m_time.hour);
	m_timeStr += ':';
	m_timeStr += convTimeFieldToStr(m_time.min);
	m_timeStr += ':';
	m_timeStr += convTimeFieldToStr(m_time.sec);
	return m_timeStr;
}

/**************************************************/
const std::string& TimeBase::getDateStr()
{
	m_dateStr = convTimeFieldToStr(m_time.day);
	m_dateStr += '.';
	m_dateStr += convTimeFieldToStr(m_time.month);
	m_dateStr += '.';
	m_dateStr += convTimeFieldToStr(m_time.year);
	return m_dateStr;
}

/**************************************************/
bool TimeBase::addEvent(tBaseEvtMsk_t evtMsk)
{
	if (checkEvtMskForSet(evtMsk)) {
		m_evtMsk = static_cast<tBaseEvtMsk_t>(m_evtMsk | evtMsk);
		return true;
	}
	return false;
}

/**************************************************/
void TimeBase::removeEvent(tBaseEvtMsk_t evtMsk)
{
	m_evtMsk = static_cast<tBaseEvtMsk_t>(m_evtMsk & (~evtMsk));
}

/**************************************************/
void TimeBase::handleEvent()
{
	tBaseEvtMsk_t events = clkIncrement();
	// notify Observers
	if (events) {
		notifyObservers(events);
	}
}

/**************************************************/
inline TimeBase::tBaseEvtMsk_t TimeBase::clkIncrement()
{
	// field layout (number of position) must be much number of bits in EVT_??? mask
	constexpr static uint32_t div[] = {
		(uint32_t)1E6 / SysConst::clkTimeBase / 1000000,	// for 1us.	  =0 if clkTimeBase < 1us
		(uint32_t)1E6 / SysConst::clkTimeBase / 100000,		// for 10us.  =0 if clkTimeBase < 10us
		(uint32_t)1E6 / SysConst::clkTimeBase / 10000,		// for 100us. =0 if clkTimeBase < 100us
		(uint32_t)1E6 / SysConst::clkTimeBase / 1000,			// for 1ms.	  =0 if clkTimeBase < 1ms
		(uint32_t)1E6 / SysConst::clkTimeBase / 100,			// for 10ms.  =0 if clkTimeBase < 10ms
		(uint32_t)1E6 / SysConst::clkTimeBase / 10,				// for 100ms. =0 if clkTimeBase < 100ms
		(uint32_t)1E6 / SysConst::clkTimeBase,						// for 1s.	  =0 if clkTimeBase < 1s
	};

	static uint32_t cnt = 0;
	unsigned evtRes = EVT_NO;
	unsigned tmpMsk = m_evtMsk;
	int pos = 0;

	while (tmpMsk) {
		while (!(tmpMsk & (1 << pos))) { ++pos; }
		if (div[pos] && (!(cnt % div[pos])))
			evtRes |= 1 << pos;
		tmpMsk &= ~(1 << pos);
		++pos;
	}

	// System clock
	if (evtRes & EVT_1S) {
		if (++m_time.sec >= 60) {
			m_time.sec = 0;
			++m_time.min;
		}
		if (m_time.min >= 60) {
			m_time.min = 0;
			++m_time.hour;
		}
		if (m_time.hour >= 24) {
			m_time.hour = 0;
			++m_time.day;
		}
		if (m_time.day > 30) {
			m_time.day = 0;
			++m_time.month;
		}
		if (m_time.month > 12) {
			m_time.month = 0;
			++m_time.year;
		}
	}

	if (++cnt > 1E6L)
		cnt = 0;

	return static_cast<tBaseEvtMsk_t>(evtRes);
}

/**************************************************/
std::string TimeBase::convTimeFieldToStr(unsigned int num)
{
	std::string str;
	if (num >= 10) {
		str = num / 10 + '0';
	}
	else {
		str = '0';
	}
	str += num % 10 + '0';
	return str;
}

/**************************************************/
bool TimeBase::checkEvtMskForSet(tBaseEvtMsk_t evtMsk)
{
	unsigned int multiplier = 1;
	unsigned tmpMsk = evtMsk;

	while (!(tmpMsk & (1 << 0))) {
		tmpMsk >>= 1;
		multiplier *= 10;
	}
	if (multiplier >= SysConst::clkTimeBase) {
		return true;
	}
	return false;
}

