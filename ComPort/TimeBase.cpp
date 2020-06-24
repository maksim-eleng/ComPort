
#include "TimeBase.h"

/**************************************************/
TimeBase::TimeBase(evtMask_t evtMask)
{
	evtMask_t tmpMsk = evtMask;
	int pos = 0;
	unsigned int multiplier;

	while (!(tmpMsk & (1 << pos))) {
		++pos;
	}
	for (multiplier = 1; pos; --pos) {
		multiplier *= 10;
	}
	if (multiplier >= SysConst::clkTimeBase) {
		m_evtMask = evtMask;
	}
	else { m_evtMask = 0; }
	assert(m_evtMask);
}

/**************************************************/
void TimeBase::addObserver(IObsTimeBase& obs, evtMask_t evtMask)
{
	timeBaseObs_t object = { &obs, evtMask };
	m_observers.push_back(object);
}

/**************************************************/
void TimeBase::removeObserver(IObsTimeBase& obs)
{
	for (int ind = 0; ind < m_observers.size(); ++ind) {
		if (&obs == m_observers[ind].pToObs) {
			m_observers.erase(m_observers.begin() + ind);
			break;
		}
	}
}

/**************************************************/
void TimeBase::notifyObservers(evtMask_t evtMsk)
{
	for (auto obs : m_observers) {
		evtMask_t mask = obs.evtMask & evtMsk;
		if (mask) {
			obs.pToObs->handleEvent(*this, mask);
		}
	}
}

/**************************************************/
std::string& TimeBase::getTime()
{
	char buf[3];
	m_timeStr = std::string(convTimeFieldToStr(buf, m_time.hour));
	m_timeStr += ':';
	m_timeStr += std::string(convTimeFieldToStr(buf, m_time.min));
	m_timeStr += ':';
	m_timeStr += std::string(convTimeFieldToStr(buf, m_time.sec));
	return m_timeStr;
}

/**************************************************/
std::string& TimeBase::getDate()
{
	char buf[3];
	m_dateStr = std::string(convTimeFieldToStr(buf, m_time.day));
	m_dateStr += '.';
	m_dateStr += std::string(convTimeFieldToStr(buf, m_time.month));
	m_dateStr += '.';
	m_dateStr += std::string(convTimeFieldToStr(buf, m_time.year));
	return m_dateStr;
}

/**************************************************/
void TimeBase::handleEvent()
{
	evtMask_t events = clkIncrement();
	// notify Observers
	if (events) {
		notifyObservers(events);
	}
}

/**************************************************/
inline TimeBase::evtMask_t TimeBase::clkIncrement()
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
	evtMask_t evtRes = 0;
	evtMask_t tmpMsk = m_evtMask;
	int pos = 0;

	while (tmpMsk) {
		while (!(tmpMsk & (1 << pos))) { ++pos; }
		if (div[pos] && (!(cnt % div[pos])))
			evtRes |= 1 << pos;
		tmpMsk &= ~(1 << pos);
		++pos;
	}

	// System clock
	if (!(cnt % ((uint32_t)1E6 / SysConst::clkTimeBase))) {
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

	return evtRes;
}

char* TimeBase::convTimeFieldToStr(char* str, unsigned int num)
{
	int i = 0;
	if (num >= 10) {
		str[i] = num / 10 + '0';
	}
	else {
		str[i] = '0';
	}
	str[++i] = num % 10 + '0';
	str[++i] = '\0';
	return str;
}