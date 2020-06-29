#include "ComPort.h"


/************************************************************/
ComPort::ComPort(char* const pRxBuf, char* const pTxBuf,
  int sizeRxBuf, int sizeTxBuf)
  :SysComPort_t(pRxBuf, pTxBuf, sizeRxBuf, sizeTxBuf)
{}

/************************************************************/
ComPort::~ComPort()
{
  m_observers.clear();
}

/**************************************************************/
bool ComPort::getRxStr( char* str, int size, char ensSymbol )
{
  int res;
  res = m_rxBuf.get( str, size, ensSymbol );
  return ((res == bufResultIsNG) ? false : true);
}

/***********************************************/
void ComPort::addObserver(IObsComPort & obs)
{
  m_observers.push_back(&obs);
}

/***********************************************/
void ComPort::removeObserver(IObsComPort & obs)
{
	for (int ind = 0; ind < m_observers.size(); ++ind) {
		if (&obs == m_observers[ind]) {
			m_observers.erase(m_observers.begin() + ind);
			break;
		}
	}
}

/**************************************************************/
void ComPort::notifyObservers(comEvtMsk_t evtMask)
{
  for (auto& observer : m_observers) {
    observer->handleEvent(*this, evtMask);
  }
}

/**************************************************************/
bool ComPort::print(const char* cstr, char endSymbol)
{
	comEvtMsk_t evt = EVT_NO;
	if (!isPortOpened()) {
		setEvent(evt, EVT_ERR_CRITICAL);
	}
	if (EVT_NO == evt) {
		if (bufResultIsNG != m_txBuf.put(cstr, endSymbol)) {
			evt = startTx();
		}
		else {
			setEvent(evt, EVT_ERR_TX);
		}
	}
	if (EVT_NO != evt) {
		notifyObservers(evt);
		return false;
	}
	return true;
}

/**************************************************************/
void ComPort::handleEvent(TimeBase&, TimeBase::evtMask_t)
{
	SysComPort_t::comEvtMsk_t events;
	events = SysComPort_t::checkCoreEvents();
	if (events)
	{
		notifyObservers(events);
	}
}
