#include "ComPort.h"
#include "utility.h"
#include <iostream>

/************************************************************/


#if RX_DATA_DETECT_METHOD == PERIODICALLY_INTERROGATION

ComPort::ComPort(TimeBase& sysClk, unsigned sizeRxBuf, unsigned sizeTxBuf)
  :SysComPort_t(sysClk, sizeRxBuf, sizeTxBuf)
{}

#else

ComPort::ComPort(unsigned sizeRxBuf, unsigned sizeTxBuf)
	:SysComPort_t(sizeRxBuf, sizeTxBuf)
{}

#endif

/************************************************************/
ComPort::~ComPort()
{
	std::cout << "Destructor for com.\n";
  m_observers.clear();
}

/**************************************************************/
bool ComPort::print(std::string str) 
{
	return SysComPort_t::print(str.c_str());
}

/**************************************************************/
ComPort& ComPort::operator<<(const char* cstr)
{
	SysComPort_t::print(cstr);
	return *this;
}

/**************************************************************/
ComPort& ComPort::operator<<(const int num)
{
	char cstr[12];
	convIntToStr(num, cstr);
	SysComPort_t::print(cstr);
	return *this;
}

/**************************************************************/
int ComPort::getRxStr( char* str, int size)
{
	int res = m_rxBuf.get(str, size, m_cfg.evtChar);
	if (Buffer::resultNG != res) {
		userCharHandled();
	}
	return res;
}

/**************************************************************/
bool ComPort::redirectStrTo(ComPort& dstCom, bool fTransfer)
{
	bool res;
	if (fTransfer)
		res = m_rxBuf.transferStrTo(dstCom.m_txBuf, m_cfg.evtChar);
	else
		res = m_rxBuf.copyStrTo(dstCom.m_txBuf, m_cfg.evtChar);
	if (res)
		startTx();
	return res;
}

/**************************************************************/
int ComPort::searchInRxBuf(char byte)
{
	return m_rxBuf.search(byte);
}

/**************************************************************/
int ComPort::searchInRxBuf(const char* str, int pStartInBuf, bool isReturnIndAfterStr)
{
	return m_rxBuf.search(str, pStartInBuf, isReturnIndAfterStr);
}



/***********************************************/
bool ComPort::addObserver(IObsComPort & obs, const comEvtMsk_t evtMsk)
{
	// search obs. Add mask if found or add obs with current musk, if not
	for (auto& _obs : m_observers) {
		if (_obs.pToObs == &obs) {
			_obs.evtMsk = static_cast<comEvtMsk_t>(_obs.evtMsk | static_cast<comEvtMsk_t>(evtMsk));
			return true;
		}
	}
	obsData_t _obj = { &obs, evtMsk };
	m_observers.push_back(_obj);
	return true;
}

/***********************************************/
void ComPort::removeObserver(const IObsComPort & obs)
{
	std::vector<obsData_t>::iterator _obs;

	for (_obs = m_observers.begin(); _obs != m_observers.end(); ++_obs) {
		if (_obs->pToObs == &obs) {
			m_observers.erase(_obs);
			break;
		}
	}
}

/**************************************************************/
void ComPort::notifyObservers(comEvtMsk_t evtMsk)
{
  for (auto& obs : m_observers) {
		comEvtMsk_t resMsk = static_cast<comEvtMsk_t>(obs.evtMsk & evtMsk);
		if (resMsk)
			obs.pToObs->handleEvent(*this, resMsk);
  }
}

