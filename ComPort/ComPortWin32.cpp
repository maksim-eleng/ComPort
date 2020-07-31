#include "ComPortWin32.h"

#ifdef _WIN32
#include <assert.h>
#include <iostream>

uint8_t ComPortWin32::m_numOfObject = 0;


/********************************************************
* @brief *****		For mov semantics		******************
*********************************************************/
ComPortWin32::ComPortWin32(ComPortWin32&& com) noexcept
{
  *this = std::move(com);
}

/*********************************************************/
ComPortWin32& ComPortWin32::operator=(ComPortWin32&& com) noexcept
{
  if (this == &com)
    return *this;
  // destructer will be called after move operation and --m_numOfObject in destructor
  ++m_numOfObject;
  // move or copy all variables
  m_rxBuf = std::move(com.m_rxBuf);
  m_txBuf = std::move(com.m_txBuf);
  m_cfg = com.m_cfg;
  m_rxOverlap = com.m_rxOverlap;					
  m_hPort = com.m_hPort;
  m_evtCharCnt = com.m_evtCharCnt;
  m_pSysClk = com.m_pSysClk;
  // delete old port from sysClk and add new
  com.m_pSysClk->removeObserver(com);
  m_pSysClk->addObserver(*this, TimeBase::EVT_10MS);
  return *this;
}




/***********************************************************/
ComPortWin32::ComPortWin32(TimeBase& sysClk,
  int sizeRxBuf, int sizeTxBuf)
  :m_rxBuf(sizeRxBuf), m_txBuf(sizeTxBuf)
{
  std::cout << "ComWin32 constructor. & = " << this << "\n";
  m_pSysClk = &sysClk;
  ++m_numOfObject;
}

/***********************************************************/
ComPortWin32::~ComPortWin32()
{
  std::cout << "ComWin32 destructor. & = " << this << " destroyed.\n";
  --m_numOfObject;
  if(!m_numOfObject)
    close();
}


/***********************************************************/
ComPortWin32::comEvtSetMsk_t operator|(ComPortWin32::comEvtSetMsk_t l, ComPortWin32::comEvtSetMsk_t r)
{
  return (ComPortWin32::comEvtSetMsk_t)(l | (unsigned int)r);
}

/***********************************************************/
void operator|=(ComPortWin32::comEvtSetMsk_t& l, const ComPortWin32::comEvtSetMsk_t& r)
{
  l = l | r;
}

/***********************************************************/
ComPortWin32::comEvtMsk_t ComPortWin32::setParam(ComPortWin32::comCfg_t& ref)
{
  // modified mirror of comCfg_t m_cfg if field in comCfg_t ref was defined
  comCfg_t set = m_cfg;
  // Field of dcb modified if necessary.
  DCB dcb;
  DWORD evtMask(0);   // for GetMask()
  bool fPortReady = true;
  comEvtMsk_t evt = EVT_NO;

  //read current DCB
  fPortReady = GetCommState(m_hPort, &dcb);
  if (!fPortReady && ERROR_SUCCESS != GetLastError()) {
    setEvent(evt, EVT_ERR_CRITICAL);
    return evt;
  }

  // permis event for rx any char (default)
  set.evtSet |= EVTSET_RX_CHAR;
  // set default parameters in dcb, timeout,...
  dcb.fBinary = true;   // binary use only
  dcb.fOutX = false;    // hardware control not used
  dcb.fInX = false;
  dcb.fAbortOnError = false;

  // Configure temporarry variabes if input parameter is not default value
  // Baud rate & Number of port are sets in cfg when port is opened
  if (ref.baud != B_NOT_CFG) {
    set.baud = ref.baud;
    dcb.BaudRate = ref.baud;
  }
  else {
    dcb.BaudRate = set.baud;
  }
  if (ref.number != COM_NOT_CFG) {
    set.number = ref.number;
  }
  if (ref.byteSize != COM_NOT_CFG) {
    set.byteSize = ref.byteSize;
    dcb.ByteSize = ref.byteSize;
  }
  if (ref.fOutxDsrFlow != COM_NOT_CFG) {
    set.fOutxDsrFlow = ref.fOutxDsrFlow;
    dcb.fOutxDsrFlow = ref.fOutxDsrFlow;
  }
  if (ref.fOutxCtsFlow != COM_NOT_CFG) {
    set.fOutxCtsFlow = ref.fOutxCtsFlow;
    dcb.fOutxCtsFlow = ref.fOutxCtsFlow;
  }
  if (ref.fDsrSensitivity != COM_NOT_CFG) {
    set.fDsrSensitivity = ref.fDsrSensitivity;
    dcb.fDsrSensitivity = ref.fDsrSensitivity;
  }
  if (ref.fNull != COM_NOT_CFG) {
    set.fNull = ref.fNull;
    dcb.fNull = ref.fNull;
  }
  if (ref.controlDTR != DTR_NOT_CFG) {
    set.controlDTR = ref.controlDTR;
    dcb.fDtrControl = ref.controlDTR;
  }
  if (ref.controlRTS != RTS_NOT_CFG) {
    set.controlRTS = ref.controlRTS;
    dcb.fRtsControl = ref.controlRTS;
  }
  if (ref.stopBits != SBIT_NOT_CFG) {
    set.stopBits = ref.stopBits;
    dcb.StopBits = ref.stopBits;
  }
  if (ref.parity != P_NOT_CFG) {
    set.parity = ref.parity;
    dcb.Parity = ref.parity;
    if (ref.parity != P_NO) {
      dcb.fParity = true;
    }
  }
  if (ref.parityChar != COM_NOT_CFG) {
    set.parityChar = ref.parityChar;
    dcb.fErrorChar = true;
  }
  if (ref.evtChar != COM_NOT_CFG) {
    set.evtChar = ref.evtChar;
    dcb.EvtChar = ref.evtChar;
    set.evtSet |= EVTSET_RX_USER_CHAR;
  }
  if (ref.evtSet != EVTSET_NOT_SET) {
    if (fPortReady) {
      fPortReady = GetCommMask(m_hPort, &evtMask);
      if (fPortReady) {
        set.evtSet |= (ref.evtSet | (comEvtSetMsk_t)evtMask);
      }
    }
  }

  // analize of current settings
  // 1. Minimal parameters of port must be set
  // 2. byteSize = 4...8
  // 3. if parity char was set - parity type must be set too
  // 4. pSysClk != null
  if ((set.baud   == B_NOT_CFG) || 
    set.number    == COM_NOT_CFG ||
    set.stopBits  == SBIT_NOT_CFG ||
    set.parity    == P_NOT_CFG ||
    set.evtSet    == EVTSET_NOT_SET ||
    set.evtChar   == COM_NOT_CFG ||
    ( set.byteSize < 4 || set.byteSize > 8) ||
    (set.parityChar != COM_NOT_CFG && (set.parity == P_NOT_CFG || set.parity == P_NO)))
  {
    setEvent(evt, EVT_ERR_INVALID_PARAM);
  }

  // DCB struct init if not port errors
  // if parameters are set correctly
  if (fPortReady && evt == EVT_NO) {
    fPortReady = SetCommState(m_hPort, &dcb);
    if (!fPortReady) {
      if (ERROR_INVALID_PARAMETER ==  GetLastError())
        setEvent(evt, EVT_ERR_INVALID_PARAM);
      else
        setEvent(evt, EVT_ERR_CRITICAL);
    }
  }

  // set WIN32 events
  if (fPortReady && evt == EVT_NO) {
    fPortReady = SetCommMask(m_hPort, set.evtSet);
    if (!fPortReady) {
      if (ERROR_INVALID_PARAMETER == GetLastError())
        setEvent(evt, EVT_ERR_INVALID_PARAM);
      else
        setEvent(evt, EVT_ERR_CRITICAL);
    }
  }

  // store temporarry mirror
  if (fPortReady && evt == EVT_NO) {
    m_cfg = set;
  }

  return evt;
}

/***********************************************************/
/* Description in http://vsokovikov.narod.ru/New_MSDN_API/Menage_files/fn_createfile.htm
https://ru.wikibooks.org/wiki/COM-%D0%BF%D0%BE%D1%80%D1%82_%D0%B2_Windows_%28%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5%29*/
/***********************************************************/
HANDLE comOpenHandle(unsigned comNum)
{
  //if (m_hPort != INVALID_HANDLE_VALUE)
  //  return EVT_ERR_OPENED_EARLIER;
  HANDLE hPort;
  //forming string with number of COM-port & open file
  wchar_t portNum[10] = L"\\\\.\\COM";
  int i = 0;
  while( portNum[i] != '\0' && i < sizeof( portNum ) / sizeof( wchar_t ) ) { ++i; }
  if( comNum < 10 )
    portNum[i] = comNum + '0';
  else {
    portNum[i] = comNum / 10 + '0';
    portNum[++i] = comNum % 10 + '0';
  }

  // open file
  hPort = CreateFile( portNum,	/*lpFileName*/
    GENERIC_READ | GENERIC_WRITE,	/*dwDesiredAccess - open for RW*/
    0,								            /*dwShareMode. Must be always 0 for COM*/
    NULL,							            /*lpSecurityAttributes. Must be always 0 for COM*/
    OPEN_EXISTING,					      /*dwCreationDistribution = OPEN_EXISTING for com*/
    FILE_FLAG_OVERLAPPED,			    /*dwFlagsAndAttributes=0 for synchro regime or
                                  /FILE_FLAG_OVERLAPPED - for asynchronous*/
    NULL							            /*hTemplateFile = NULL for com*/
  );
  return hPort;
  //return (INVALID_HANDLE_VALUE == m_hPort) ? EVT_ERR_CRITICAL : EVT_NO;
}

/***********************************************************/
unsigned comGetQuantityForOpen()
{
  unsigned num = 0;
  for (int i = 0; i < ComPortWin32::MAX_COM_NUM; ++i) {
    HANDLE h = comOpenHandle(i);
    if ( h != INVALID_HANDLE_VALUE) {
      ++num;
      CloseHandle(h);
    }
  }
  return num;
}

/***********************************************************/
/* Description in http://vsokovikov.narod.ru/New_MSDN_API/Menage_files/fn_createfile.htm
https://ru.wikibooks.org/wiki/COM-%D0%BF%D0%BE%D1%80%D1%82_%D0%B2_Windows_%28%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5%29*/
/***********************************************************/
ComPortWin32::comEvtMsk_t ComPortWin32::open( unsigned comNum, comBaud_t baud )
{
  // open of port if not opened
  if (m_hPort == INVALID_HANDLE_VALUE) {
    m_hPort = comOpenHandle(comNum);
    if( INVALID_HANDLE_VALUE == m_hPort ) {
      return EVT_ERR_CRITICAL;
    }
  }

  bool fPortReady( true );
  comEvtMsk_t events = EVT_NO;

  // Set recomendation buffer's size (rx/tx)
  fPortReady = SetupComm( m_hPort, (m_rxBuf.getBufSize() * 2), (m_txBuf.getBufSize() * 2) );
  
  // set timeouts for not delay
  if( fPortReady ) {
    COMMTIMEOUTS t;
    t.ReadIntervalTimeout = -1;
    t.ReadTotalTimeoutConstant = 0;
    t.ReadTotalTimeoutMultiplier = 0;
    t.WriteTotalTimeoutConstant = 0;
    t.WriteTotalTimeoutMultiplier = 0;
    fPortReady = SetCommTimeouts( m_hPort, &t );
  }

  if( fPortReady ) {
    m_rxOverlap.hEvent = CreateEvent( NULL,    // �������� ������������ �� ��������� 
      true,     // manual ������� 
      false,    // ��������� �� ���������� 
      NULL      // ��� �����
    );
    if( !m_rxOverlap.hEvent ) {
      fPortReady = false;
    }
  }

  // forming result of operation 
  // subscribe to sysClk for periodically interview of WIN32 rx Buffer
  if (fPortReady) {
    subscribe(*m_pSysClk, TimeBase::EVT_10MS);
    m_cfg.number = comNum;
    m_cfg.baud = baud;
  }
  else {
    close();
    setEvent(events, EVT_ERR_CRITICAL);
  }

  return events;
}

/***********************************************************/
ComPortWin32::comEvtMsk_t ComPortWin32::open( const char* const comName, comBaud_t baud )
{
  int i = 0;
  // Convert str to uint
  while( (comName[i] < '1' || comName[i] > '9') && comName[i] != '\0' ) { ++i; }
  unsigned int num = comName[i] - '0';
  if( comName[++i] != '\0' )
    num = num * 10 + comName[i] - '0';
  return open( num, baud );
}

/***********************************************************/
ComPortWin32::comEvtMsk_t ComPortWin32::openFirstFree(unsigned& startComNum, comBaud_t baud)
{
  comEvtMsk_t evt = EVT_ERR_CRITICAL;
  for (; evt && startComNum < MAX_COM_NUM; ++startComNum)
    evt = open(startComNum, baud);
  return evt;
}

/***********************************************************/
void ComPortWin32::close()
{
  if (m_hPort != INVALID_HANDLE_VALUE) {
    CloseHandle(m_hPort);
    m_hPort = INVALID_HANDLE_VALUE;
    m_pSysClk->removeObserver(*this);
  }
}

/***********************************************************/
ComPortWin32::comEvtMsk_t ComPortWin32::reopen()
{
  comEvtMsk_t evt; 
  evt = open(m_cfg.number, m_cfg.baud);
  if (EVT_NO == evt) {
    evt = setParam(m_cfg);
  }
  return evt;
}

/***********************************************************/
bool ComPortWin32::isPortOpened() const
{
  return m_hPort != INVALID_HANDLE_VALUE;
}

int ComPortWin32::userCharGetReceivedCounter() const
{
  return m_evtCharCnt;
}

/***********************************************************/
void ComPortWin32::userCharHandled()
{
  --m_evtCharCnt;
}

/***********************************************************/
int ComPortWin32::getPortNumber() const
{
    return m_cfg.number;
}

/***********************************************************/
int ComPortWin32::getBaud() const
{
  return m_cfg.baud;
}

/******************************************************/
void ComPortWin32::setEvent(comEvtMsk_t& events, comEvtMsk_t mask)
{
  uint32_t tmp = events;
  events = (comEvtMsk_t)(tmp | mask);
}

/******************************************************/
ComPortWin32::comEvtMsk_t ComPortWin32::startTx()
{
  if (!isPortOpened()) {
    return EVT_ERR_CRITICAL;
  }
  if (m_txBuf.checkIsEpty()) {
    return EVT_ERR_TX;
  }

  char buf[50];
  DWORD dwRet;
  DWORD err;
  comEvtMsk_t evt = EVT_NO;
  DWORD len = 0;
  int cnt = 0;

  int index = m_txBuf.getIndexForRead();

  while (index != m_txBuf.getIndexForWrite()) // don't use < for cyclical buffer
  {
    // buffering data
    char data;
    len = 0;
    // buffering data in buf
    while (len < sizeof(buf))
    {
      data = m_txBuf.get(index);
      if (data != Buffer::resultNG) {
        buf[len] = data;
        ++len;
      }
      else {
        break; // buffer is empty - go out from copy to buf
      }
    }//end copy to buf[]

    // send to com
    if (len) {
      OVERLAPPED ovf = { 0 };
      dwRet = WriteFile(m_hPort, buf, len, NULL, &ovf);
      err = GetLastError();
      // get len = writen bytes
      len = 0;
      if (!dwRet && (ERROR_IO_PENDING == err)) {
        dwRet = GetOverlappedResult(m_hPort, &ovf, &len, true);
        err = GetLastError();
      }
      // len - writen bytes, err=ERROR_IO_PENDING || ERROR_IO_INCOMPLETE - OK
      if (!len || (err != ERROR_SUCCESS && err != ERROR_IO_PENDING && err != ERROR_IO_INCOMPLETE)) {
        // error of last transmit. Break of operation
        setEvent(evt, EVT_ERR_TX);
        index = -1; // incorrect value for setIndexForRead() 
        break;
      }
    }
  }
  // All ok - correction pointer in tx buffer
  m_txBuf.setIndexForRead(index);
  return evt;
}

/**************************************************************/
bool ComPortWin32::print(const char* cstr)
{
  comEvtMsk_t evt = EVT_NO;
  if (!isPortOpened()) {
    setEvent(evt, EVT_ERR_CRITICAL);
  }
  if (EVT_NO == evt) {
    int res;
    bool fPutTerm = false;
    res = m_txBuf.put(cstr, m_cfg.evtChar);
    if (Buffer::resultNG == res) {
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
bool ComPortWin32::subscribe(TimeBase& sysClk, TimeBase::tBaseEvtMsk_t evtMsk)
{
  m_pSysClk = &sysClk;
  return sysClk.addObserver(*this, evtMsk);
}

/***********************************************************/
ComPortWin32::comEvtMsk_t ComPortWin32::checkCoreEvents()
{
  static DWORD dwEvtMask = 0;
  static enum {
    waitEvt,
    pending,
  }state = waitEvt;

  if (!isPortOpened()) {
    state = waitEvt;
    DWORD dwEvtMask = 0;
    return EVT_ERR_CRITICAL;
  }
  comEvtMsk_t events = EVT_NO;
  DWORD dwRet;
  bool fReady = 0;    // =1 - ready for read data
  char buf[50];       // for ReadFile

  // this section need in order for the WaitCommEvent and GetOverlappedResult 
  // to be performed at different times
  switch (state) 
  {
    case waitEvt:
      dwRet = WaitCommEvent(m_hPort, &dwEvtMask, &m_rxOverlap);
      if (dwRet)
        fReady = 1;
      else {
        if ( ERROR_IO_PENDING == GetLastError())
          state = pending;
      }  
    break;
    case pending:
    {
      DWORD nnn;  //  for GetOverlappedResult() only
      dwRet = GetOverlappedResult(m_hPort, &m_rxOverlap, &nnn, false);
      if (dwRet) {
        fReady = true;
        state = waitEvt;
      }
      else {
        if (ERROR_IO_INCOMPLETE != GetLastError())
          setEvent(events, EVT_ERR_CRITICAL);
      }
    }
    break;
    default: break;
  }

  // Read mask 
  if (fReady) {
    // Forming mask with configurabled mask and event
    DWORD msk;
    if (GetCommMask(m_hPort, &msk))
      dwEvtMask &= msk;
    else
      dwEvtMask = 0;

    /******  EVENT HANDLING  ******/

    // Any byte was received.
    // All data from COM buffer moves to classes rxBuffer throught buf[]
    // If Event char used and received -> notifyObservers();
    if (dwEvtMask & EVTSET_RX_CHAR) {
      DWORD len;
      // read data from system rx buf while rxBuf state not overflow
      do {
        dwRet = ReadFile(m_hPort, buf, sizeof(buf), &len, &m_rxOverlap);
        if (!dwRet) {
          if (ERROR_SUCCESS != GetLastError())
            setEvent(events, EVT_ERR_CRITICAL);
          break;
        }
        // if buffer was readed.
        for (DWORD i = 0; i < len; ++i) {
          int byte;
          byte = m_rxBuf.put(buf[i]);
          // if buffer is overflow - correction buffer and set events
          if (byte == Buffer::resultNG) {
            setEvent(events, EVT_ERR_RX_BUF_OVF);
            break;
          }
          // if Event char was detected in input stream
          if (buf[i] == m_cfg.evtChar) {
            setEvent(events, EVT_RX_USER_CHAR);
            ++m_evtCharCnt;
          }
        }
      } while (len);//while
    }
  
    // EOF char received
    if (dwEvtMask & EVTSET_RX_USER_CHAR) {
      setEvent(events, EVT_RX_USER_CHAR);
    }
    // Receive buffer is 80 percent full
    if (dwEvtMask & EVTSET_RX80FULL) {
      setEvent(events, EVT_RX80_FULL);
    }
    // CTS changed state
    if (dwEvtMask & EVTSET_CTS_CHANGED) {
      setEvent(events, EVT_CTS_CHANGED);
    }
    // DSR changed state
    if (dwEvtMask & EVTSET_DSR_CHANGED) {
      setEvent(events, EVT_DSR_CHANGED);
    }
    // Line status error occurred
    if (dwEvtMask & EVTSET_LINE_ERR) {
      setEvent(events, EVT_ERR_LINE);
    }
  }

  return events;
}

/**************************************************************/
void ComPortWin32::handleEvent(TimeBase&, TimeBase::tBaseEvtMsk_t)
{
  ComPortWin32::comEvtMsk_t events;
  events = checkCoreEvents();
  if (events) {
    notifyObservers(events);
  }
}


#endif
