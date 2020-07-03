#include "Buffer.h"

#include <cassert>

/**********************************************************/
Buffer::Buffer(char* buf, int size)
	: m_data(buf), m_size(size)
{
	assert(m_size && "Buffer's size = 0");
	// the index can't occupy a signed bit
	assert(( m_size > sizeof(size) * 4 )	&& "Size is big");
	if (!m_data) {
		m_data = new (std::nothrow) char[m_size];
		if (!m_data)	return;
		isDynamic_m_data = true;
	}
	clear();
}

/**********************************************************/
Buffer::~Buffer()
{
	if (isDynamic_m_data) {
		delete[]m_data;
		m_data = nullptr;
	}
}

/**********************************************************/
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
	}
	else { 
		// overflow
		ENABLE_INTERRUPT;
		return bufResultNG;
	}
	ENABLE_INTERRUPT;
	return m_front;
}

/**********************************************************/
int Buffer::put(const char* str, bool fPutTerminator)
{
	assert(str);
	int i = 0;
	int ind = m_front;
	char data = BUF_NOT_CFG;

	while (str[i] == '\0') { ++i; }

	for (int res = bufResultNG; data != '\0'; ++i) {
		data = str[i];
		if (data == '\0' && !fPutTerminator) { 
			break; 
		}
		res = put(data);
		if (res == bufResultNG) {
			DISABLE_INTERRUPT;
			m_front = ind;
			calcLength();
			return bufResultNG;
		}
	}
	return m_front;
}

/**********************************************************/
int Buffer::put(std::string& str, bool fPutTerminator)
{
	return put(str.c_str(), fPutTerminator);
}

/**********************************************************/
char Buffer::get()
{
	char byte;
	if (m_len)
	{
		DISABLE_INTERRUPT;
		byte = m_data[m_end];
		++m_end;
		--m_len;
		//организация кольца. указатели Front, End
		if (m_end >= m_size) { m_end = 0; }
		ENABLE_INTERRUPT;
		return byte;
	}
	return bufResultNG;
}

/**********************************************************/
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
	return bufResultNG;
}

/**********************************************************/
int Buffer::get(char* str, int sizeStr)
{
	assert(str && sizeStr);
	int i = 0;
	int ind = m_end;
	char data = '\0';
	--sizeStr;
	while (data == '\0') {
		data = get();
		if (data == bufResultNG) {
			m_end = ind;
			calcLength();
			return bufResultNG;
		}
	}
	str[i] = data;
	++i;

	while (data != '\0') {
		data = get();
		if (i == sizeStr || data == bufResultNG) {
			str[i] = '\0';
			m_end = ind;
			calcLength();
			return bufResultNG;
		}
		str[i] = data;
		++i;
	}
	return m_end;
}

/**********************************************************/
int Buffer::get(std::string& str)
{
	int i = 0;
	int ind = m_end;
	char data = '\0';
	
	while (data == '\0') {
		data = get();
		if (data == bufResultNG) {
			m_end = ind;
			calcLength();
			return bufResultNG;
		}
	}
	str.push_back(data);

	while (data != '\0') {
		data = get();
		if (data == bufResultNG) {
			m_end = ind;
			calcLength();
			return bufResultNG;
		}
		str.push_back(data);
	}
	return m_end;
}

/*****************************************************/
int Buffer::getBufSize() const	{
	return m_size; 
}

/*****************************************************/
int Buffer::getIndexForWrite() const {
	return m_front; 
}

/*****************************************************/
int Buffer::getIndexForRead() const {
	return m_end; 
}

/*****************************************************/
bool Buffer::checkIsNotEpty() const {
	return (bool)m_len; 
}

/*****************************************************/
bool Buffer::checkIsEpty() const {
	return !(bool)m_len;
}

/*****************************************************/
void Buffer::setIndexForWrite(int newIndexForWrite)
{
	if (newIndexForWrite >= 0 && newIndexForWrite < m_size) {
		m_front = newIndexForWrite;
		calcLength();
	}
}

/*****************************************************/
void Buffer::setIndexForRead(int newIndexForRead)
{
	if (newIndexForRead >= 0 && newIndexForRead < m_size) {
		m_end = newIndexForRead;
		calcLength();
	}
}

/*****************************************************/
void Buffer::resetIndex()
{
	DISABLE_INTERRUPT;
	m_front = 0;
	m_end = 0;
	m_len = 0;
	m_maxLen = 0;
	ENABLE_INTERRUPT;
}

/*****************************************************/
int Buffer::operator=(const char byte)
{
	clear();
	return put(byte);
}

/*****************************************************/
int Buffer::operator=(const char* str)
{
	clear();
	return put(str);
}

/*****************************************************/
int Buffer::operator=(std::string& str)
{
	clear();
	return put(str);
}

/*****************************************************/
int Buffer::operator+=(const char byte) { 
	return put(byte); 
}

/*****************************************************/
int Buffer::operator+=(const char* str) {
	return put(str); 
}

/*****************************************************/
int Buffer::operator+=(std::string& str) {
	return put(str); 
}

/*****************************************************/
char Buffer::operator[](int index)
{
	assert(index >= 0 && index < m_size - 1);
	return m_data[index];
}

/*****************************************************/
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

/*****************************************************/
int Buffer::search(const char* str, int pStartInBuf, bool isReturnIndAfterStr)
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

/*****************************************************/
bool Buffer::copyStrTo(Buffer& dstBuf)
{
	int end = m_end;
	int front = dstBuf.m_front;
	char data = '\0';

	while (data == '\0') {
		data = get(end);
		if (data == bufResultNG) {
			return false;
		}
	}

	while (data != '\0') {
		data = get(end);
		if (data == bufResultNG) {
			dstBuf.setIndexForWrite(front);
			dstBuf.calcLength();
			return false;
		}
		dstBuf.put(data);
	}
	return true;
}

/*****************************************************/
bool Buffer::transferStrTo(Buffer& dstBuf)
{
	int end = m_end;
	int front = dstBuf.m_front;
	char data = '\0';

	while (data == '\0') {
		data = get();
		if (data == bufResultNG) {
			m_end = end;
			calcLength();
			return false;
		}
	}

	while (data != '\0') {
		data = get();
		if (data == bufResultNG) {
			setIndexForWrite(front);
			calcLength();
			dstBuf.setIndexForWrite(front);
			dstBuf.calcLength();
			return false;
		}
		dstBuf.put(data);
	}
	return true;
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
	ENABLE_INTERRUPT;
}




