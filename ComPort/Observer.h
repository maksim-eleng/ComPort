#pragma once

/**************************************************/
template<class ClassName, typename IObsBase, typename msk_t, typename Obs_t>
void addObserver(IObsBase& obs, msk_t evtMsk)
{
	for (auto& _obs : this->m_observers) {
		if (_obs.pToObs == &obs) {
			_obs.evtMsk = _obs.evtMsk | evtMsk;
			return true;
		}
	}
	Obs_t object = { &obs, evtMsk };
	this->m_observers.push_back(object);
}