#include "ComPort.h"
#include "utility.h"
#include <iostream>

/************************************************************/


#if RX_DATA_DETECT_METHOD == PERIODICALLY_INTERROGATION

ComPort::ComPort(TimeBase& sysClk, int sizeRxBuf, int sizeTxBuf)
  :SysComPort_t(sysClk, sizeRxBuf, sizeTxBuf),
	m_numOfChannel(SysComPort_t::m_numOfObject)
{}

#else

ComPort::ComPort(char* const pRxBuf, char* const pTxBuf, int sizeRxBuf, int sizeTxBuf)
	:SysComPort_t(pRxBuf, pTxBuf, sizeRxBuf, sizeTxBuf)
{
	++m_numOfObject;
	m_numOfChannel = m_numOfObject - 1;
}

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
  return m_rxBuf.get(str, size, m_cfg.evtChar);
}

/**************************************************************/
uint8_t ComPort::getNumOfChannel() const
{
	return m_numOfChannel;
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
  for (auto& obs : m_observers) {
    obs->handleEvent(*this, evtMask);
  }
}

