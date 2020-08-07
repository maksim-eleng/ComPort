#pragma once

/**************************************************/
//template<typename IObsBase, typename evtMsk_t>

class IObsComPort;
class Observer
{
public:

//	typedef struct{
//		IObsComPort* pToObs;	// pointer to observer
//		comEvtMsk_t evtMsk;	// mask for generation event for this observer
//	}obsData_t;
//
//	Observer() = default;
//	~Observer() = default;
//
//	Observer(const Observer&) = delete;
//	Observer(Observer&&) = delete;
//	Observer& operator=(const Observer&) = delete;
//	Observer& operator=(Observer&&) = delete;
//
//
//	void addObserver(IObsComPort& obs, comEvtMsk_t evtMsk)
//	{
//		// search obs. Add mask if found or add obs with current musk, if not
//		for (auto& _obs : m_obs) {
//			if (_obs.pToObs == &obs) {
//				_obs.evtMsk = (comEvtMsk_t)(_obs.evtMsk | (comEvtMsk_t)(evtMsk));
//				return;
//			}
//		}
//		obsData_t _obj = { &obs, evtMsk };
//		m_obs.push_back(_obj);
//	}
//
//	/**************************************************/
//	void removeObserver(IObsComPort& obs)
//	{
//		std::vector<obsData_t>::iterator _obs;
//
//		for (_obs = m_obs.begin(); _obs != m_obs.end(); ++_obs) {
//			if (_obs->pToObs == &obs) {
//				m_obs.erase(_obs);
//				break;
//			}
//		}
//	}
//
//	/**************************************************************/
//	void notifyObservers(comEvtMsk_t evtMsk)
//	{
//		for (auto& _obs : m_obs) {
//			//				_obs.pToObs->handleEvent(com, evtMsk);
//		}
//	}
//
//
//private:
//	std::vector<obsData_t> m_obs;		// data about observer
};




void addObserver(IObsBase& obs, evtMsk_t evtMsk)
{
	// search obs. Add mask if found or add obs with current musk, if not
	for (auto& _obs : m_observers) {
		if (_obs.pToObs == &obs) {
			_obs.evtMsk = (evtMsk_t)(_obs.evtMsk | (evtMsk_t)(evtMsk));
			return;
		}
	}
	ObsBase _obj = { &obs, evtMsk };
	m_observers.push_back(_obj);
}
