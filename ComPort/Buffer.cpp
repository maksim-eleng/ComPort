#include "Buffer.h"
#include <cassert>

#include <iostream>


/**********************************************************/
Buffer::Buffer(unsigned size)
	: m_size(size),
	m_data(new (std::nothrow) char[size])
{
	std::cout << "Buf constructor. &buf=" << this << "\n";

	if (!m_data)	
		exit(1);
	resetIndex();
}

/**********************************************************/
Buffer::Buffer(const Buffer& buf)
//	:m_data(new (std::nothrow) char[buf.m_size])
{
	std::cout << "Copy constructor of buffer. *" << this << " = *" << &buf << "\n";
	*this = buf;
	//if (!m_data)	
	//	exit(1);
	//copyIndex(buf);
	//memcpy(this->m_data, buf.m_data, sizeof(this->m_data));
}

/**********************************************************/
const Buffer& Buffer::operator=(const Buffer& buf)
{
	std::cout << "Copy operator= of buffer. *" << this << " = *" << &buf << "\n";

	if (this == &buf)
		return *this;
	delete[] m_data;
	m_data = new (std::nothrow) char[buf.m_size];
	copyIndex(buf);
	memcpy(this->m_data, buf.m_data, sizeof(this->m_data));
	return *this;
}

/**********************************************************/
Buffer::Buffer(Buffer&& buf) noexcept
{
	std::cout << "Move constructor of buffer. *" << this << " = *" << &buf << "\n";
	*this = static_cast<Buffer&&>(buf);
}

/**********************************************************/
const Buffer& Buffer::operator=(Buffer&& buf) noexcept
{
	std::cout << "Move operator= of buffer. *" << this << " = *" << &buf << "\n";

	if (this == &buf)
		return *this;
	delete[] m_data;
	copyIndex(buf);
	m_data = buf.m_data;
	buf.m_data = nullptr;
	return *this;
}

/**********************************************************/
Buffer::~Buffer()
{
	std::cout << "Buf destructor. &buf=" << this << " destroyed.\n";

	delete[]m_data;
	m_data = nullptr;
}

/**********************************************************/
int Buffer::put(char byte)
{
	#ifdef BUF_TYPE_LINEAR
	DISABLE_INTERRUPT;
	if (m_front < m_size)   //нет переполнения
	{
		m_data[m_front] = byte;
		++m_front;
	}
	else {
		// overflow
		ENABLE_INTERRUPT;
		return resultNG;
	}
	ENABLE_INTERRUPT;
	return m_front;

	#elif defined BUF_TYPE_CYCLICAL
	DISABLE_INTERRUPT;
	if (m_len < m_size)   //нет переполнения
	{
		++m_len;
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
	#endif
}

/**********************************************************/
int Buffer::put(const char* str, char eofChar)
{
	assert(str);
	unsigned i = 0;
	unsigned ind = m_front;
	char data = notCfg;
	int res = resultNG;

	while (str[i] == '\0') { ++i; }

	while (data != eofChar && str[i] != '\0') {
		data = str[i];
		res = put(data);
		++i;
		if (res == resultNG) {
			break;
		}
	}
	if (eofChar == '\0' && str[i] == '\0')
		res = put('\0');
	if (res == resultNG) {
		setIndexForWrite(ind);
		return resultNG;
	}
	return m_front;
}

/**********************************************************/
int Buffer::put(const std::string& str, char eofChar)
{
	return put(str.c_str(), eofChar);
}

/**********************************************************/
char Buffer::get()
{
	#ifdef BUF_TYPE_LINEAR
	char byte;
	if (m_end <= m_front)
	{
		DISABLE_INTERRUPT;
		byte = m_data[m_end];
		++m_end;
		if (m_end == m_front)
			transferOnBeginOfBuffer();
		ENABLE_INTERRUPT;
		return byte;
	}
	return resultNG;

	#elif defined BUF_TYPE_CYCLICAL
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
	#endif
}

/**********************************************************/
char Buffer::get(unsigned& index)
{
	#ifdef BUF_TYPE_LINEAR
	char data = resultNG;
	if (index < m_front)
	{
		data = m_data[index];
		++index;
		return data;
	}
	return resultNG;

	#elif defined BUF_TYPE_CYCLICAL
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

	#endif
}

/**********************************************************/
int Buffer::get(char* str, unsigned sizeStr, char eofChar)
{
	assert(str && sizeStr);
	int i = 0;
	int ind = m_end;
	char data = '\0';
	--sizeStr;
	while (data == '\0') {
		data = get();
		if (data == resultNG) {
			setIndexForRead(ind);
			return resultNG;
		}
	}
	str[i] = data;
	++i;

	while (data != eofChar && data != '\0') {
		data = get();
		if (i == sizeStr || data == resultNG) {
			str[i] = '\0';
			setIndexForRead(ind);
			return resultNG;
		}
		str[i] = data;
		++i;
	}
	str[i] = '\0';
	if (m_data[m_end] == '\0')
		get();

	return m_end;
}

/**********************************************************/
int Buffer::get(std::string& str, char eofChar)
{
	int i = 0;
	int ind = m_end;
	char data = '\0';

	while (data == '\0') {
		data = get();
		if (data == resultNG) {
			setIndexForRead(ind);
			return resultNG;
		}
	}
	str.push_back(data);

	while (data != eofChar && data != '\0') {
		data = get();
		if (data == resultNG) {
			setIndexForRead(ind);
			return resultNG;
		}
		str.push_back(data);
	}
	if (m_data[m_end] == '\0')
		get();
	return m_end;
}

/*****************************************************/
int Buffer::getBufSize() const {
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
	#ifdef BUF_TYPE_LINEAR
	return m_end < m_front;
	#elif defined BUF_TYPE_CYCLICAL
	return (bool)m_len;
	#endif
}

/*****************************************************/
bool Buffer::checkIsEpty() const {
	#ifdef BUF_TYPE_LINEAR
	return m_end >= m_front;
	#elif defined BUF_TYPE_CYCLICAL
	return (bool)!m_len;
	#endif
}

/*****************************************************/
void Buffer::setIndexForWrite(unsigned newIndexForWrite)
{
	if (newIndexForWrite >= 0 && newIndexForWrite < m_size) {
		DISABLE_INTERRUPT;
		m_front = newIndexForWrite;
		#ifdef BUF_TYPE_CYCLICAL
		calcLength();
		#endif
		ENABLE_INTERRUPT;
	}
}

/*****************************************************/
void Buffer::setIndexForRead(unsigned newIndexForRead)
{
	if (newIndexForRead >= 0 && newIndexForRead < m_size) {
		DISABLE_INTERRUPT;
		#ifdef BUF_TYPE_LINEAR
		m_end = newIndexForRead;
		transferOnBeginOfBuffer();

		#elif defined BUF_TYPE_CYCLICAL
		m_end = newIndexForRead;
		calcLength();
		#endif
		ENABLE_INTERRUPT;
	}
}

/*****************************************************/
void Buffer::resetIndex()
{
	DISABLE_INTERRUPT;
	m_front = 0;
	m_end = 0;
	#ifdef BUF_TYPE_CYCLICAL
	m_len = 0;
	#endif
	ENABLE_INTERRUPT;
}

/*****************************************************/
void Buffer::copyIndex(const Buffer& srcBuf)
{
	DISABLE_INTERRUPT;
	m_front = srcBuf.m_front;
	m_end = srcBuf.m_end;
	m_size = srcBuf.m_size;
#ifdef BUF_TYPE_CYCLICAL
	m_len = srcBuf.m_len;
#endif
	ENABLE_INTERRUPT;
}

/*****************************************************/
char Buffer::operator[](unsigned index)
{
	assert(index >= 0 && index < m_size - 1);
	return m_data[index];
}

/*****************************************************/
int Buffer::search(char byte)
{
	unsigned point = m_end, tmp_p;
	while (point != m_front) {
		tmp_p = point;
		if (byte == get(point))
			return tmp_p;
	}
	return resultNG;
}

/*****************************************************/
int Buffer::search(const char* str, unsigned pStartInBuf, bool isReturnIndAfterStr)
{
	assert(str);
	if (!pStartInBuf || pStartInBuf < m_end)
		pStartInBuf = m_end;
	int pFirstInStr = pStartInBuf;
	int tmp_p; 
	int i = 0;
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
		return resultNG;
	}
}

/*****************************************************/
bool Buffer::copyStrTo(Buffer& dstBuf, char eofChar)
{
	unsigned end = m_end;
	unsigned front = dstBuf.m_front;
	char data = '\0';

	while (data == '\0') {
		data = get(end);
		if (data == resultNG) {
			return false;
		}
	}
	dstBuf.put(data);

	while (data != eofChar && data != '\0') {
		data = get(end);
		if (data == resultNG) {
			dstBuf.setIndexForWrite(front);
			return false;
		}
		dstBuf.put(data);
	}
	return true;
}

/*****************************************************/
bool Buffer::transferStrTo(Buffer& dstBuf, char eofChar)
{
	int end = m_end;
	int front = dstBuf.m_front;
	char data = '\0';

	while (data == '\0') {
		data = get();
		if (data == resultNG) {
			setIndexForRead(end);
			return false;
		}
	}
	dstBuf.put(data);

	while (data != eofChar && data != '\0') {
		data = get();
		if (data == resultNG) {
			setIndexForRead(end);
			dstBuf.setIndexForWrite(front);
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
	for (unsigned i = 0; i < m_size; ++i)
		m_data[i] = 0;
	resetIndex();
}

/**********************************************************/
#ifdef BUF_TYPE_LINEAR
void Buffer::transferOnBeginOfBuffer()
{
	unsigned i = 0;
	unsigned k = m_end;
	DISABLE_INTERRUPT;
	while (k < m_front) {
		m_data[i] = m_data[k];
		++i; ++k;
	}
	m_front = i;
	m_end = 0;
	ENABLE_INTERRUPT;
}
#endif

/**********************************************************/
#ifdef BUF_TYPE_CYCLICAL
void Buffer::calcLength()
{
	DISABLE_INTERRUPT;
	m_len = (m_front >= m_end) ? (m_front - m_end) : (m_size - m_end + m_front );
	ENABLE_INTERRUPT;
}
#endif
