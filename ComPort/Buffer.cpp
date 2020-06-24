#include "Buffer.h"



char Buffer::m_objectCounter = 0;

Buffer::Buffer(char* _buf, int _size)
	: m_data(_buf), m_size(_size)
{
	//assert(((_buf && m_size != MIN_BUFFER_SIZE) || (!_buf))
	//	&& "Size for external buffer  must be different from MIN_BUFFER_SIZE");
	assert(m_size && "Buffer's size = 0");
	assert(( m_size > sizeof(_size) * 4 )		// индекс не может занимать знаковый бит
		&& "Size is big");
	if (!m_data) {
		m_data = new (std::nothrow) char[m_size];
		if (!m_data)	return;
		isDynamic_m_data = true;
	}
	clear();
	++m_objectCounter;
}

Buffer::~Buffer()
{
	if (isDynamic_m_data) {
		delete[]m_data;
		m_data = nullptr;
	}
}

int Buffer::put(char byte)
{
	DISABLE_INTERRUPT;
	if (m_len < m_size)   //нет переполнения
	{
		++m_len;
		// формируем максимальную длину буфера
		if (m_len > m_maxLen) { m_maxLen = m_len; }
		m_data[m_front] = byte;
		++m_front;
		//организация кольца. указатели Front, End
		if (m_front >= m_size) { m_front = 0; }
		if (m_state != OVF) { m_state = NORMAL; }
	}
	else { 
		// overflow
		m_state = OVF; 
		return bufResultNG;
	}
	ENABLE_INTERRUPT;
	return m_front;
}

int Buffer::put(const char* str, char StrEndSymbol)
{
	assert(str);
	int i = 0;
	while (str[i] == '\0') { ++i; }
	char data = 1;
	int ind = m_front;
	while (str[i] != '\0') {
		if (put((data = str[i])) == bufResultIsNG) {
			m_front = ind;
			calcLength();
			return bufResultNG;
		}
		if (data == StrEndSymbol)
			return m_front;
		++i;
	};
	return m_front;
}

int Buffer::put(std::string& str, char StrEndSymbol)
{
	return put(str.c_str());
}

char Buffer::get()
{
	char byte = bufResultNG;
	DISABLE_INTERRUPT;
	if (m_len)
	{
		byte = m_data[m_end];
		++m_end;
		//организация кольца. указатели Front, End
		if (m_end >= m_size) { m_end = 0; }
		m_state = ((--m_len)) ? NORMAL : EMPTY;
		ENABLE_INTERRUPT;
		return byte;
	}
	ENABLE_INTERRUPT;
	return bufResultIsNG;
}

char Buffer::get(int& index)
{
	char data = bufResultNG;
	if (index != m_front)
	{
		data = m_data[index];
		++index;
		//организация кольца.
		if (index >= m_size) { index = 0; }
		return data;
	}
	return bufResultIsNG;
}

int Buffer::get(char* str, int sizeStr, char strEndSymbol)
{
	assert(str);
	int i = 0;
	--sizeStr;
	while (1)
	{
		str[i] = get();
		if (i == sizeStr || str[i] == bufResultIsNG) {
			str[i] = '\0';
			return bufResultNG;
		}
		if (str[i] == strEndSymbol)		
			return m_end;
		++i;
	}
}

int Buffer::get(std::string& str, char strEndSymbol)
{
	int i = 0;
	//буфер не пустой и нет конца строки
	while (1)
	{
		char data;
		if ((data = get()) == bufResultIsNG) {
			return bufResultNG;
		}
		else
			str.push_back(data);
		if (data == strEndSymbol)
			return m_end;
		++i;
	}
}

void Buffer::resetIndex()
{
	DISABLE_INTERRUPT;
	m_front = 0;
	m_end = 0;
	m_len = 0;
	m_maxLen = 0;
	m_state = EMPTY;
	ENABLE_INTERRUPT;
}

/****   Operators overflow	******/

int Buffer::operator=(const char byte)
{
	clear();
	return put(byte);
}

int Buffer::operator=(const char* str)
{
	assert(str);
	clear();
	return put(str);
}

int Buffer::operator=(std::string& str)
{
	clear();
	return put(str);
}

int Buffer::operator+=(const char byte) { return put(byte); }

int Buffer::operator+=(const char* str) { return put(str); }

int Buffer::operator+=(std::string& str) { return put(str); }

char Buffer::operator[](int index)
{
	assert(index >= 0 && index < m_size - 1);
	return m_data[index];
}


/**********	 Other function		******************/
/*************************************************/

int Buffer::search(char byte)
{
	int point = m_end, tmp_p;
	while (point != m_front) {
		tmp_p = point;
		if (byte == get(point))
			return tmp_p;
	}
	return bufResultNG;
}

int Buffer::search(const char* str, bool isReturnIndAfterStr, int pStartInBuf)
{
	assert(str);
	if (!pStartInBuf || pStartInBuf < m_end)
		pStartInBuf = m_end;
	int pFirstInStr = pStartInBuf;
	int tmp_p, i = 0;
	bool res = false;
	while (str[i] != '\0' && pStartInBuf != m_front) {
		tmp_p = pStartInBuf;
		if (str[i] == get(pStartInBuf)) {
			if (!res)
				pFirstInStr = tmp_p;
			res = true;
			++i;
		}
		else {
			res = false;
			i = 0;
		}
	}
	if (res) {
		return (isReturnIndAfterStr) ? pStartInBuf : pFirstInStr;
	}
	else {
		return bufResultNG;
	}
}

int Buffer::jump(int numSteps, bool isUnconditionalJump)
{
	// безусловный прыжок
	if (isUnconditionalJump) {
		if (numSteps >= m_end) {
			m_end = numSteps;
			calcLength();
			return m_end;
		}
		else return bufResultNG;
	}
	// последовательный
	int tEnd = m_end;
	while (numSteps && m_end != m_front) {
		// вперед
		if (numSteps >= 0) {
			if ((++m_end) >= m_size)
				m_end = 0;
			--numSteps;
		}
		// назад
		else {
			if ((--m_end) < 0)
				m_end = m_size - 1;
			++numSteps;
		}
	}
	if (m_end == m_front && numSteps != 0) {
		m_end = tEnd;
		return bufResultNG;
	}
	calcLength();
	return m_end;
}


/*****			PRIVAT SECTION			 **********/
/**************************************************/

void Buffer::clear()
{
	DISABLE_INTERRUPT;
	for (int i = 0; i < m_size; ++i)
		m_data[i] = 0;
	resetIndex();
}

void Buffer::calcLength()
{
	DISABLE_INTERRUPT;
	m_len = (m_front >= m_end) ? (m_front - m_end) : (m_size - m_end + m_front );
	m_state = (m_len) ? NORMAL : EMPTY;
	ENABLE_INTERRUPT;
}




