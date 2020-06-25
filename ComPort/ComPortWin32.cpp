#include "ComPortWin32.h"

#ifdef _WIN32
#include <assert.h>


/***********************************************************/
ComPortWin32::comEvtMsk_t ComPortWin32::setParam(comParamInit_t param, uint32_t val)
{
  DCB dcb;
  bool fPortReady;
  comEvtMsk_t events = EVT_NO;
  
  fPortReady = GetCommState(m_hPort, &dcb);
  if (!fPortReady && ERROR_SUCCESS != GetLastError()) {
    setEvent(events, EVT_ERR_CRITICAL);
    param = NO_PARAM;
  }
  
  // forming parameter's variable
  switch (param)
  {
    case BAUD: {
      assert(val);
      dcb.BaudRate = (DWORD)val;
    }
    break;

    case BYTE_SIZE: {
      assert(val >= 5 && val <= 8);
      dcb.ByteSize = (BYTE)val;
    }
    break;

    case STOP_BITS: {
      comStopBit_t tmp = static_cast<comStopBit_t>(val);
      assert(tmp == SBIT_ONE || tmp == SBIT_ONE5 || tmp == SBIT_TWO);
      dcb.StopBits = (BYTE)val;
    }
    break;

    case PARITY_TYPE: {
      comParity_t tmp = static_cast<comParity_t>(val);
      assert(tmp == P_NO || tmp == P_ODD || tmp == P_EVEN || tmp == P_MARK || tmp == P_SPACE);
      dcb.Parity = val;
    }
    break;

    case PARITY_CHECK: {
      dcb.fParity = (bool)val;
    }
    break;

    case PARITY_USE_ERR_CHAR: {
      dcb.fErrorChar = true;
      dcb.ErrorChar = val;
    }
    break;

    case PARITY_NOT_USE_ERR_CHAR: {
      dcb.fErrorChar = false;
    }
    break;

    case USE_DTR_OUT: {
      comDTRControl_t tmp = static_cast<comDTRControl_t>(val);
      assert(tmp == DTR_DISABLE || tmp == DTR_ENABLE || tmp == DTR_HANDSHAKE);
      dcb.fDtrControl = val;
    }
    break;

    case USE_DSR_FLOW: {
      dcb.fOutxDsrFlow = (bool)val;
    }
    break;

    case USE_DSR_SENS: {
      dcb.fDsrSensitivity = (bool)val;
    }
    break;

    case USE_RTS_OUT: {
      comRTSControl_t tmp = static_cast<comRTSControl_t>(val);
      assert(tmp == RTS_DISABLE || tmp == RTS_ENABLE || tmp == RTS_HANDSHAKE || tmp == RTS_TOGGLE);
      dcb.fRtsControl = val;
    }
    break;

    case USE_CTS_FLOW: {
      dcb.fOutxCtsFlow = val;
    }
    break;

    case USE_NULL_IGNORE: {
      dcb.fNull = val;
    }
    break;
    
    case USE_USER_CHAR_EVENT:
      m_userChar = (char)val;
    break;

    case USE_EOF_CHAR_EVENT: 
    {
      DWORD evtMask(0);
      fPortReady = GetCommMask(m_hPort, &evtMask);
      if ( fPortReady ) {
        fPortReady = SetCommMask(m_hPort, evtMask | EVTSET_RX_EOFCHAR);
      }
      if ( fPortReady ) {
        dcb.EvtChar = (char)val;
        m_eofChar = (char)val;
      }
      else if (ERROR_SUCCESS != GetLastError()){
        setEvent(events, EVT_ERR_CRITICAL);
      }
    }
    break;

    case EV_SET: {
      DWORD evtMask(0);
      fPortReady = GetCommMask(m_hPort, &evtMask);
      assert(!(evtMask & USE_EOF_CHAR_EVENT)); // in this case must use USE_EOF_CHAR_EVENT command
      if ( fPortReady ) {
        fPortReady = SetCommMask(m_hPort, evtMask | (val));
      }
      if ((!fPortReady) && ERROR_SUCCESS != GetLastError()) {
        setEvent(events, EVT_ERR_CRITICAL);
      }
    }
    break;

    default:;
  }//switch

  // DCB struct init
  if ( fPortReady && (!checkEvent(events, EVT_ERR_CRITICAL)) ) { 
    fPortReady = SetCommState(m_hPort, &dcb);
    if ((!fPortReady) && ERROR_SUCCESS != GetLastError()) {
      setEvent(events, EVT_ERR_CRITICAL);
      param = NO_PARAM;
    }
  }

  /*** For timeouts ****/
  if (param == T_READ_INTERVAL || param == T_READ_TOTAL_MULTIPLIER ||
      param == T_READ_TOTAL_CONSTANT || param == T_WRITE_TOTAL_MULTIPLIER ||
      param == T_WRITE_TOTAL_CONSTANT ) {
    COMMTIMEOUTS CommTimeOuts;
    fPortReady = GetCommTimeouts(m_hPort, &CommTimeOuts);
    if ( fPortReady ) {
      switch ( param ) { 
        case T_READ_INTERVAL:
          CommTimeOuts.ReadIntervalTimeout = val;
        break;
        case T_READ_TOTAL_MULTIPLIER:
          CommTimeOuts.ReadTotalTimeoutMultiplier = val;
        break;
        case T_READ_TOTAL_CONSTANT:
          CommTimeOuts.ReadTotalTimeoutConstant = val;
        break;
        case T_WRITE_TOTAL_MULTIPLIER:
          CommTimeOuts.WriteTotalTimeoutMultiplier = val;
        break;
        case T_WRITE_TOTAL_CONSTANT:
          CommTimeOuts.WriteTotalTimeoutConstant = val;
        break;
      } //switch
      fPortReady = SetCommTimeouts(m_hPort, &CommTimeOuts);
    } //if
    if ((!fPortReady) && ERROR_SUCCESS != GetLastError()) {
      setEvent(events, EVT_ERR_CRITICAL);
    }
  }//if

  // close handle if any eeror and return event mask
  if( checkEvent(events, EVT_ERR_CRITICAL) ) {
    close();
  }

  return events;
}

/***********************************************************/
ComPortWin32::comEvtMsk_t ComPortWin32::setParam(comCfg_t& param)
{
  DCB dcb;
  bool fPortReady;
  comEvtMsk_t evt = EVT_NO;

  comCfg_t tmpSet;

  m_comCfg = param;
  
  // read from sys DCB to m_comCfg
  if (EVT_NO != getParam(m_comCfg)) {
    return evt;
  } 
  // set default parameters in dcb
  //if (!m_comCfg.fdefParamIsSet) {
  //  m_comCfg.fdefParamIsSet = true;
  //  dcb.fBinary = true;
  //  dcb.fOutX = false;
  //  dcb.fInX = false;
  //  dcb.fAbortOnError = false;
  //}
  //forming comSet struct
  //dcb.BaudRate = param.baud;
  //dcb.ByteSize = param.byteSize;
  //dcb.StopBits = param.stopBits;
  //// if != NO - fParity will be to On state
  //if (param.parity == NO) {
  //  dcb.fParity = false;
  //}
  //else {
  //  dcb.fParity = true;
  //  dcb.Parity = param.parity;
  //}
  // if fErrorChar != -1 - fErrorChar will be to On state
  if (param.parityChar == -1) {
    dcb.fErrorChar = false;
  }
  else {
    dcb.fErrorChar = true;
    dcb.fErrorChar = param.parityChar;
  }

  //dcb.EvtChar = param.evtChar;
  //dcb.fDtrControl = param.controlDTR;
  //dcb.fRtsControl = param.controlRTS;
  //dcb.fOutxDsrFlow = param.fOutxDsrFlow;
  //dcb.fOutxCtsFlow = param.fOutxCtsFlow;
  //dcb.fDsrSensitivity = param.fDsrSensitivity;
  //dcb.fNull = param.fNull;
  // DCB struct init
  fPortReady = SetCommState(m_hPort, &dcb);
  if ((!fPortReady) && ERROR_SUCCESS != GetLastError()) {
    setEvent(evt, EVT_ERR_CRITICAL);
  }


  //// forming parameter's variable
  //switch (param)
  //{
  //case BAUD: {
  //  assert(val);
  //  dcb.BaudRate = (DWORD)val;
  //}
  //         break;

  //case BYTE_SIZE: {
  //  assert(val >= 5 && val <= 8);
  //  dcb.ByteSize = (BYTE)val;
  //}
  //              break;

  //case STOP_BITS: {
  //  comStopBit_t tmp = static_cast<comStopBit_t>(val);
  //  assert(tmp == ONE_BIT || tmp == ONE5_BIT || tmp == TWO_BIT);
  //  dcb.StopBits = (BYTE)val;
  //}
  //              break;

  //case PARITY_TYPE: {
  //  comParity_t tmp = static_cast<comParity_t>(val);
  //  assert(tmp == NO || tmp == ODD || tmp == EVEN || tmp == MARK || tmp == SPACE);
  //  dcb.Parity = val;
  //}
  //                break;

  //case PARITY_CHECK: {
  //  dcb.fParity = (bool)val;
  //}
  //                 break;

  //case PARITY_USE_ERR_CHAR: {
  //  dcb.fErrorChar = true;
  //  dcb.ErrorChar = val;
  //}
  //                        break;

  //case PARITY_NOT_USE_ERR_CHAR: {
  //  dcb.fErrorChar = false;
  //}
  //                            break;

  //case USE_DTR_OUT: {
  //  comDTRControl_t tmp = static_cast<comDTRControl_t>(val);
  //  assert(tmp == DTR_DISABLE || tmp == DTR_ENABLE || tmp == DTR_HANDSHAKE);
  //  dcb.fDtrControl = val;
  //}
  //                break;

  //case USE_DSR_FLOW: {
  //  dcb.fOutxDsrFlow = (bool)val;
  //}
  //                 break;

  //case USE_DSR_SENS: {
  //  dcb.fDsrSensitivity = (bool)val;
  //}
  //                 break;

  //case USE_RTS_OUT: {
  //  comRTSControl_t tmp = static_cast<comRTSControl_t>(val);
  //  assert(tmp == RTS_DISABLE || tmp == RTS_ENABLE || tmp == RTS_HANDSHAKE || tmp == RTS_TOGGLE);
  //  dcb.fRtsControl = val;
  //}
  //                break;

  //case USE_CTS_FLOW: {
  //  dcb.fOutxCtsFlow = val;
  //}
  //                 break;

  //case USE_NULL_IGNORE: {
  //  dcb.fNull = val;
  //}
  //                    break;

  //case USE_USER_CHAR_EVENT:
  //  m_userChar = (char)val;
  //  break;

  //case USE_EOF_CHAR_EVENT:
  //{
  //  DWORD evtMask(0);
  //  fPortReady = GetCommMask(m_hPort, &evtMask);
  //  if (fPortReady) {
  //    fPortReady = SetCommMask(m_hPort, evtMask | EVTSET_RX_EOFCHAR);
  //  }
  //  if (fPortReady) {
  //    dcb.EvtChar = (char)val;
  //    m_eofChar = (char)val;
  //  }
  //  else if (ERROR_SUCCESS != GetLastError()) {
  //    setEvent(events, EVT_ERR_CRITICAL);
  //  }
  //}
  //break;

  //case EV_SET: {
  //  DWORD evtMask(0);
  //  fPortReady = GetCommMask(m_hPort, &evtMask);
  //  assert(!(evtMask & USE_EOF_CHAR_EVENT)); // in this case must use USE_EOF_CHAR_EVENT command
  //  if (fPortReady) {
  //    fPortReady = SetCommMask(m_hPort, evtMask | (val));
  //  }
  //  if ((!fPortReady) && ERROR_SUCCESS != GetLastError()) {
  //    setEvent(events, EVT_ERR_CRITICAL);
  //  }
  //}
  //           break;

  //default:;
  //}//switch

  //// DCB struct init
  //if (fPortReady && (!checkEvent(events, EVT_ERR_CRITICAL))) {
  //  fPortReady = SetCommState(m_hPort, &dcb);
  //  if ((!fPortReady) && ERROR_SUCCESS != GetLastError()) {
  //    setEvent(events, EVT_ERR_CRITICAL);
  //    param = NO_PARAM;
  //  }
  //}

  ///*** For timeouts ****/
  //if (param == T_READ_INTERVAL || param == T_READ_TOTAL_MULTIPLIER ||
  //  param == T_READ_TOTAL_CONSTANT || param == T_WRITE_TOTAL_MULTIPLIER ||
  //  param == T_WRITE_TOTAL_CONSTANT) {
  //  COMMTIMEOUTS CommTimeOuts;
  //  fPortReady = GetCommTimeouts(m_hPort, &CommTimeOuts);
  //  if (fPortReady) {
  //    switch (param) {
  //    case T_READ_INTERVAL:
  //      CommTimeOuts.ReadIntervalTimeout = val;
  //      break;
  //    case T_READ_TOTAL_MULTIPLIER:
  //      CommTimeOuts.ReadTotalTimeoutMultiplier = val;
  //      break;
  //    case T_READ_TOTAL_CONSTANT:
  //      CommTimeOuts.ReadTotalTimeoutConstant = val;
  //      break;
  //    case T_WRITE_TOTAL_MULTIPLIER:
  //      CommTimeOuts.WriteTotalTimeoutMultiplier = val;
  //      break;
  //    case T_WRITE_TOTAL_CONSTANT:
  //      CommTimeOuts.WriteTotalTimeoutConstant = val;
  //      break;
  //    } //switch
  //    fPortReady = SetCommTimeouts(m_hPort, &CommTimeOuts);
  //  } //if
  //  if ((!fPortReady) && ERROR_SUCCESS != GetLastError()) {
  //    setEvent(events, EVT_ERR_CRITICAL);
  //  }
  //}//if

  //// close handle if any eeror and return event mask
  //if (checkEvent(events, EVT_ERR_CRITICAL)) {
  //  close();
  //}

  //return events;
}

/***********************************************************/
ComPortWin32::comEvtMsk_t ComPortWin32::getParam(comCfg_t& param)
{
  DCB dcb = {0};
  bool fPortReady;
  comEvtMsk_t events = EVT_NO;

  //// read system settings
  //fPortReady = GetCommState(m_hPort, &dcb);
  //if (fPortReady || ERROR_SUCCESS == GetLastError()) {
  //  //forming comSet struct
  //  param.baud = (comBaud_t)dcb.BaudRate;
  //  param.byteSize = dcb.ByteSize;
  //  param.stopBits = (comStopBit_t)dcb.StopBits;
  //  // if fParity not set - parity not use
  //  if (!dcb.fParity)
  //    param.parity = NO;
  //  else
  //    param.parity = (comParity_t)dcb.Parity;
  //  // if fParity not set - parity not use
  //  if (!dcb.fErrorChar)
  //    param.parityChar = -1;
  //  else
  //    param.parityChar = dcb.ErrorChar;
  //  param.evtChar = dcb.EvtChar;
  //  param.controlDTR = (comDTRControl_t)dcb.fDtrControl;
  //  param.controlRTS = (comRTSControl_t)dcb.fRtsControl;
  //  param.fOutxDsrFlow = dcb.fOutxDsrFlow;
  //  param.fOutxCtsFlow = dcb.fOutxCtsFlow;
  //  param.fDsrSensitivity = dcb.fDsrSensitivity;
  //  param.fNull = dcb.fNull;
  //}
  //else {
  //  setEvent(events, EVT_ERR_CRITICAL);
  //}

  return events;
}

/***********************************************************/
bool ComPortWin32::checkEvent(comEvtMsk_t& events, comEvtMsk_t mask) const
{
  return events & mask;
}

/***********************************************************/
/* Description in http://vsokovikov.narod.ru/New_MSDN_API/Menage_files/fn_createfile.htm
https://ru.wikibooks.org/wiki/COM-%D0%BF%D0%BE%D1%80%D1%82_%D0%B2_Windows_%28%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5%29*/
/***********************************************************/
ComPortWin32::comEvtMsk_t ComPortWin32::open( int comNum, comBaud_t baud )
{
  close();

  bool fPortReady( true );
  comEvtMsk_t events = EVT_NO;

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
  m_hPort = CreateFile( portNum,	/*lpFileName*/
    GENERIC_READ | GENERIC_WRITE,	/*dwDesiredAccess - open for RW*/
    0,								            /*dwShareMode. Must be always 0 for COM*/
    NULL,							            /*lpSecurityAttributes. Must be always 0 for COM*/
    OPEN_EXISTING,					      /*dwCreationDistribution = OPEN_EXISTING for com*/
    FILE_FLAG_OVERLAPPED,			    /*dwFlagsAndAttributes=0 for synchro regime or
                                  /FILE_FLAG_OVERLAPPED - for asynchronous*/
    NULL							            /*hTemplateFile = NULL for com*/
  );
  if( INVALID_HANDLE_VALUE == m_hPort ) {
    setEvent(events, EVT_ERR_CRITICAL);
    close();
    return events;
  }

  // config baud, and 8N1 as default
  DCB dcb;
  fPortReady = GetCommState( m_hPort, &dcb );
  if( fPortReady ) {
    dcb.BaudRate = baud;
    dcb.ByteSize = 8;
    dcb.Parity = P_NO;
    dcb.StopBits = SBIT_ONE;
    fPortReady = SetCommState( m_hPort, &dcb );
  }
  
  // Set recomendation buffer's size (rx/tx)
  if( fPortReady ) {
    fPortReady = SetupComm( m_hPort, (m_rxBuf.getBufSize() * 2), (m_txBuf.getBufSize() * 2) );
  }
  
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

  // Create event. Event type - in SetCommMask()
  // EV_RX_CHAR - us default for receive
  if( fPortReady ) {
    events = setParam( EV_SET, EVTSET_RX_CHAR);
    if(events)
      fPortReady = false;
  }

  if( fPortReady ) {
    m_rxOverlap.hEvent = CreateEvent( NULL,    // атрибуты безопасности по умолчанию 
      true,     // manual события 
      false,    // состояние не сигнальное 
      NULL      // без имени
    );
    if( !m_rxOverlap.hEvent ) {
      fPortReady = false;
    }
  }

  // forming result of operation 
  if (fPortReady) {
    m_comNumber = comNum;
    m_comBaud = baud;
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
void ComPortWin32::close()
{
  if (m_hPort != INVALID_HANDLE_VALUE) {
    CloseHandle(m_hPort);
    m_hPort = INVALID_HANDLE_VALUE;
  }
  m_rxBuf.resetIndex();
  m_txBuf.resetIndex();
  m_rxOverlap = { 0 };
}

/***********************************************************/
ComPortWin32::comEvtMsk_t ComPortWin32::reopen()
{
  comEvtMsk_t evt = EVT_NO;
  evt = open(m_comNumber, m_comBaud);
  //if (evt == EVT_NO)
  //  setParam(USE_USER_CHAR_EVENT, '$');
  //if (evt == EVT_NO)
  //  setParam(USE_EOF_CHAR_EVENT, '\n');
  return evt;
}

/***********************************************************/
ComPortWin32::ComPortWin32(char* const pRxBuf, char* const pTxBuf,
  int sizeRxBuf, int sizeTxBuf)
  :m_rxBuf(pRxBuf, sizeRxBuf), m_txBuf(pTxBuf, sizeTxBuf)
{}

/***********************************************************/
ComPortWin32::~ComPortWin32()
{
  close();
}

/***********************************************************/
ComPortWin32::comEvtMsk_t ComPortWin32::checkWin32ComEvents()
{
  static DWORD dwEvtMask = 0;
  comEvtMsk_t events = EVT_NO;
  DWORD dwRet;
  bool fReady = 0;    // =1 - ready for read data
  char buf[50];       // for ReadFile
  static enum {
    waitEvt,
    pending,
  }state = waitEvt;

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
        else
          setEvent(events, EVT_ERR_CRITICAL);
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
          if (byte == bufResultIsNG) {
            setEvent(events, EVT_ERR_RX_BUF_OVF);
            break;
          }
          // if Event char was detected in input stream
          if (buf[i] == m_userChar && m_userChar != EVT_CHAR_FICTIVE_VALUE)
            setEvent(events, EVT_RX_USER_CHAR);
          //// if EOF char was detected in input stream
          //if (buf[i] == m_eofChar && m_eofChar != EVT_CHAR_FICTIVE_VALUE) {
          //  setEvent(events, EVT_RX_EOF_CHAR);
          //}
        }
      } while (len);//while
    }
  
    // EOF char received
    if (dwEvtMask & EVTSET_RX_EOFCHAR) {
      setEvent(events, EVT_RX_EOF_CHAR);
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

/******************************************************/
ComPortWin32::comEvtMsk_t ComPortWin32::startTx()
{
  comEvtMsk_t evt = EVT_NO;
  if (m_hPort == INVALID_HANDLE_VALUE) {
    setEvent(evt, EVT_ERR_CRITICAL);
    return evt;
  }
  if (m_txBuf.checkIsEpty()) {
    setEvent(evt, EVT_ERR_TX);
    return evt;
  }

  char buf[50];
  DWORD dwRet;
  DWORD err;
  DWORD len = 0;
  int lenTotal = 0;
  int cnt = 0;

  int index = m_txBuf.getIndexForRead();
  while (index != m_txBuf.getIndexForWrite())
  {
    // buffering data
    char data;
    len = 0;
    while (len < sizeof(buf))
    {
      data = m_txBuf.get(index);
      if (data != bufResultIsNG) {
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
      if (len && (err == ERROR_IO_PENDING || err == ERROR_IO_INCOMPLETE)) {
        lenTotal += len;
      }
      // error of last transmit. Break of operation
      else {
        setEvent(evt, EVT_ERR_TX);
        lenTotal = 0;
        break;
      }
    }
  }
  // All ok - correction pointer in tx buffer, else - in previous state 
  m_txBuf.jump(lenTotal, false);
  
  return evt;
}

/******************************************************/
void ComPortWin32::setEvent(comEvtMsk_t& events, comEvtMsk_t mask)
{
  uint32_t tmp = events;
  events = (comEvtMsk_t)(tmp | mask);
}


#endif
