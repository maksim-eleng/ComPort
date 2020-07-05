#include "ComPort.h"
#include "utility.h"

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
ComPort& ComPort::operator<<(const char* cstr)
{
	print(cstr);
	return *this;
}

/**************************************************************/
ComPort& ComPort::operator<<(const int num)
{
	char cstr[12];
	convIntToStr(num, cstr);
	print(cstr);
	return *this;
}

/**************************************************************/
bool ComPort::print(const char* cstr)
{
	comEvtMsk_t evt = EVT_NO;
	if (!isPortOpened()) {
		setEvent(evt, EVT_ERR_CRITICAL);
	}
	if (EVT_NO == evt) {
		int res;
		bool fPutTerm = false;
		res = m_txBuf.put(cstr, m_cfg.evtChar);
		if (bufResultNG == res) {
			setEvent(evt, EVT_ERR_TX);
		}
		else {
			evt = startTx();
		}
	}
	if (EVT_NO != evt) {
		notifyObservers(evt);
		return false;
	}
	return true;
}

/**************************************************************/
bool ComPort::print(std::string str)
{
	return print(str.c_str());
}



/**************************************************************/
int ComPort::getRxStr( char* str, int size)
{
  return m_rxBuf.get(str, size, m_cfg.evtChar);
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
int ComPort::searchInRxBuf(char byte)
{
	return m_rxBuf.search(byte);
}

/**************************************************************/
int ComPort::searchInRxBuf(const char* str, bool isReturnIndAfterStr, int pStartInBuf)
{
	return m_rxBuf.search(str, isReturnIndAfterStr, pStartInBuf);
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
