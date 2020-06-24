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
  return ((res < 0) ? false : true);
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
	if (bufResultIsNG != m_txBuf.put(cstr, endSymbol)) {
		evt = startTx();
	}
	else {
		setEvent(evt, EVT_ERR_TX);
	}
	if (evt) {
		notifyObservers(evt);
	}
	return (evt == EVT_NO) ? true : false;
}
