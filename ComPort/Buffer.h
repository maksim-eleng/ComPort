#pragma once
/*******************************************************
	Класс для организации кольцевого буфера



*/

#include "SysConst.h"
#include <stdint.h>
#include <string>

// uncomment for use cyclical type of buffer
//#define BUF_TYPE_CYCLICAL

#ifndef BUF_TYPE_CYCLICAL
#define BUF_TYPE_LINEAR
#endif

#define bufResultNG			-1
#define BUF_NOT_CFG			-1

// minimum buffer size. default for dynamic data
#define MIN_BUF_SIZE	512

// disable/enuble interrupt for controller system
#ifndef DISABLE_INTERRUPT
#define DISABLE_INTERRUPT
#endif
#ifndef ENABLE_INTERRUPT
#define ENABLE_INTERRUPT
#endif


class Buffer
{
/*****			PUBLIC SECTION			 ******/
public:

	/****************************************************
	 * @brief	Constructor. By default create buffer with MIN_BUFFER_SIZE size
	 * @param buf <char*> - external buffer. If not defined or ==0 - dynamic memory allocation with size=size
	 * @param size <int> - size of buffer
	 * @Create of object:	Buffer buf;	Buffer buf(NULL); - dynamic memory allocation with size=MIN_BUFFER_SIZE
	 *										Buffer buf(NULL, 100); - dynamic memory allocation with size=100
	 *										Buffer buf(buffer, sizeof(buffer)); - for external buffer.
	*****************************************************/
	Buffer(char* buf = 0, int size = MIN_BUF_SIZE);

	// Copying of objects is prohibited
 	Buffer(const Buffer& buf) = delete;

	/****************************************************
	 * @brief	Destructor. If nenory for buffer was allocated dynamically - release
	*****************************************************/
	~Buffer();

	/****************************************************
	 * @brief	Put char in buffer
	 * @param byte <const char> - input char
	 * @return next index of buffer for write operation 
				or bufResultNG, if buffer is full
	*****************************************************/
	int put(const char byte);

	/****************************************************
	 * @brief	Put string to buffer from external buffer while EOF not
	 * will be copied to buffer or terminator not detected in input str
	 * (terminator not copying, if not equal of EOF char)
	 * If data in input str starts with '\0' - not copy.
	 * If function complete with error, the buffer returns
	 * to previous state.
	 * @param str <char*> - external buffer for copy to buffer.
	 * @param eofChar <char> - EOF char
	 * @return next buffer's index for read operation
	 *						or 	bufResultIsNG - terminator not
	 *						found before buffer is empty
	*****************************************************/
	int put(const char* str, char eofChar = '\0');

	/****************************************************
	 * @brief	Put string to buffer from external std::string while EOF not
	 * will be copied to buffer or terminator not detected in input str
	 * (terminator not copying, if not equal of EOF char)
	 * If data in input str starts with '\0' - not copy.
	 * If function complete with error, the buffer returns
	 * to previous state.
	 * @param str <std::string> - external std::string for copy to buffer.
	 * @param eofChar <char> - EOF char
	 * @return next buffer's index for read operation
	 *						or 	bufResultIsNG - terminator not
	 *						found before buffer is empty
	*****************************************************/
	int put(std::string& str, char eofChar = '\0');

	/****************************************************
	 * @brief	Get byte from buffer, if buffer is not empty
	 * @return <char> or bufResultIsNG if buffer is empty
	*****************************************************/
	char get();
	
	/****************************************************
	 * @brief	Get byte from buffer, if buffer is not empty
	 * with using external integer index as buffer's index 
	 * for read operation. Buffer's index does not change.
	 * @return <char> or bufResultIsNG if buffer is empty
	*****************************************************/
	char get(int& index);

	/****************************************************
	 * @brief	Get string from buffer to external buffer while EOF not
	 * will be copied to out str or terminator not detected in buffer 
	 * (terminator not copying, if not equal of EOF char)
	 * If data in buffer starts with '\0' - not copy.
	 * If function complete with error, the buffer returns
	 * to previous state.
	 * @param str <char*> - external buffer for destination.
	 * @param sizeStr <int> - size of external buffer
	 * @return next buffer's index for read operation
	 *						or 	bufResultIsNG - terminator not 
	 *						found before buffer is empty
	*****************************************************/
	int get(char* str, int sizeStr, char eofChar = '\0');

	/****************************************************
	 * @brief	Get string from std::string to external buffer while EOF not
	 * will be copied to out str or terminator not detected in buffer
	 * (terminator not copying, if not equal of EOF char)
	 * If data in buffer starts with '\0' - not copy.
	 * If function complete with error, the buffer returns
	 * to previous state.
	 * @param str <std::string> - external string for destination.
	 * @return next buffer's index for read operation
	 *						or 	bufResultIsNG - terminator not
	 *						found before buffer is empty
	*****************************************************/
	int get(std::string& str, char eofChar = '\0');

	/****************************************************
	 * @brief Get size of buffer.
	*****************************************************/
	int getBufSize() const;

	/****************************************************
	 * @brief Get buffer index for Write operation
	*****************************************************/
	int getIndexForWrite() const;

	/****************************************************
	 * @brief Get buffer index for Read operation
	*****************************************************/
	int getIndexForRead() const;

	/****************************************************
	 * @brief Chech buffer. Return true, if not enpty
	*****************************************************/
	bool checkIsNotEpty() const;

	/****************************************************
	 * @brief Chech buffer. Return true, if enpty
	*****************************************************/
	bool checkIsEpty() const;

	/***********************************************************
	 * @brief Set buffer index for Write operation, if index in buffer range.
	 * @param newIndexForWrite <int> - new index. Must be >=0 && < size of buffer
	************************************************************/
	void setIndexForWrite(int newIndexForWrite);

	/***********************************************************
	 * @brief Set buffer index for Read operation, if index in buffer range.
	 * @param newIndexForWrite <int> - new index. Must be >=0 && < size of buffer
	************************************************************/
	void setIndexForRead(int newIndexForRead);

	/***********************************************************
	 * @brief Reset all buffer's index in start of buffer state.
	************************************************************/
	void resetIndex();

	/****   Operators overflow	******/

	/***********************************************************
	 * @brief operator= group is similar of put(),
	 * but with clear of buffer
	************************************************************/
	
	/****************************************************
	* @brief	Put char in buffer. 
	* The buffer will be cleared before put.
	* @param byte <const char> - input char
	* @return next index of buffer for write operation
			or bufResultNG, if buffer is full
	*****************************************************/
	int operator=(const char byte);

	/****************************************************
	 * @brief	Put string to buffer from external buffer while EOF not
	 * will be copied to buffer or terminator not detected in input str
	 * (terminator not copying, if not equal of EOF char)
	 * The buffer will be cleared before put.
	 * If data in input str starts with '\0' - not copy.
	 * If function complete with error, the buffer returns
	 * to previous state.
	 * @param str <char*> - external buffer for copy to buffer.
	 * @param eofChar <char> - EOF char
	 * @return next buffer's index for read operation
	 *						or 	bufResultIsNG - terminator not
	 *						found before buffer is empty
	*****************************************************/
	int operator=(const char* str);

	/****************************************************
	 * @brief	Put string to buffer from external std::string while EOF not
	 * will be copied to buffer or terminator not detected in input str
	 * (terminator not copying, if not equal of EOF char)
	 * The buffer will be cleared before put.
	 * If data in input str starts with '\0' - not copy.
	 * If function complete with error, the buffer returns
	 * to previous state.
	 * @param str <std::string> - external std::string for copy to buffer.
	 * @param eofChar <char> - EOF char
	 * @return next buffer's index for read operation
	 *						or 	bufResultIsNG - terminator not
	 *						found before buffer is empty
	*****************************************************/
	int operator=(std::string& str);

	/****************************************************
	* @brief	Copying of objects is prohibited,
	* including copy constructor
	*****************************************************/
	Buffer& operator=(const Buffer& buf) = delete;

	/***********************************************************
	* @brief operator+= group is similar of put()
	************************************************************/

	/****************************************************
	* @brief	Put char in buffer.
	* @param byte <const char> - input char
	* @return next index of buffer for write operation
			or bufResultNG, if buffer is full
	*****************************************************/
	int operator+=(const char byte);

	/****************************************************
	 * @brief	Put string to buffer from external buffer while EOF not
	 * will be copied to buffer or terminator not detected in input str
	 * (terminator not copying, if not equal of EOF char)
	 * The buffer will be cleared before put.
	 * If data in input str starts with '\0' - not copy.
	 * If function complete with error, the buffer returns
	 * to previous state.
	 * @param str <char*> - external buffer for copy to buffer.
	 * @param eofChar <char> - EOF char
	 * @return next buffer's index for read operation
	 *						or 	bufResultIsNG - terminator not
	 *						found before buffer is empty
	*****************************************************/
	int operator+=(const char* str);

	/****************************************************
	 * @brief	Put string to buffer from external std::string while EOF not
	 * will be copied to buffer or terminator not detected in input str
	 * (terminator not copying, if not equal of EOF char)
	 * If data in input str starts with '\0' - not copy.
	 * If function complete with error, the buffer returns
	 * to previous state.
	 * @param str <std::string> - external std::string for copy to buffer.
	 * @param eofChar <char> - EOF char
	 * @return next buffer's index for read operation
	 *						or 	bufResultIsNG - terminator not
	 *						found before buffer is empty
	*****************************************************/
	int operator+=(std::string& str);

	/****************************************************
	 * @brief	Get byte from buffer, with using external
	 * integer index. This index and buffer's index does not change.
	 * @return <char> from buffer[index]
	*****************************************************/
	char operator[](int index);

	/**********	 Other function		******************/
	/*************************************************/

	/****************************************************
	 * @brief	Search symbol in buffer in not readed range. 
	 * Buffer's indexes don't change.
	 * @param byte <char> - char for search
	 * @return <int> - index of buffer or bufResultNG if not found
	*****************************************************/
	int search(char byte);

	/****************************************************
	 * @brief	Search c_style string in buffer in not readed range.
	 * Buffer's indexes don't change.
	 * @param str <const char* str> - string for search
	 * @param pStartInBuf <int> - start index for search. If 0 or not defined - the seach begins
	 *		with index for read operation
	 * @param isReturnIndAfterStr <bool>:
	 *		if = false or not defined - the function return index in buffer where found start of input string 
	 *		if = true - the function return next index in buffer where found end of input string
	 * @return <int> - index of buffer or bufResultNG if not found
	*****************************************************/
	int search(const char* str, int pStartInBuf = 0, bool isReturnIndAfterStr = false);

	/*****************************************************
	 * @brief	Copy to another buffer while EOF not will be copied 
	 * to destination buffer or terminator not detected in source buffer
	 * (terminator not copying, if not equal of EOF char)
	 * If data in sourse buffer starts with '\0' - not copy.
	 * If function complete with error, the buffer returns
	 * to previous state.
	 * @param dstBuf <Buffer> - buffer of receiver
	 * @param eofChar <char> - EOF char
	 * @return	true - Copy success
	 *					false - the EOF char not found in buffer of
	 *						source while copy operation. All buffers
	 *						will be return to previous state
	******************************************************/
	bool copyStrTo(Buffer& dstBuf, char eofChar = '\0');

	/*****************************************************
	 * @brief	Transfer to another buffer while EOF not will be copied
	 * to destination buffer or terminator not detected in source buffer
	 * (terminator not copying, if not equal of EOF char)
	 * If data in sourse buffer starts with '\0' - not copy.
	 * If function complete with error, the buffer returns
	 * to previous state.
	 * @param dstBuf <Buffer> - buffer of receiver
	 * @param eofChar <char> - EOF char
	 * @return	true - Copy success
	 *					false - the EOF char not found in buffer of
	 *						source while copy operation. All buffers
	 *						will be return to previous state
	******************************************************/
	bool transferStrTo(Buffer& dstBuf, char eofChar = '\0');


	/****************************************************
	/*********			PROTECTED SECTION			 *************/
	/***************************************************/
protected:


	/****************************************************
	/*********			PRIVATE SECTION			 *************/
	/***************************************************/
private:

	//pointer for next write operation in buffer
	int volatile m_front = 0;
	//pointer for next read operation from buffer
	int volatile m_end = 0;
	#ifdef BUF_TYPE_CYCLICAL
	// leng of data occupied area
	int volatile m_len = 0;
	#endif
	// size of buffer
	int m_size = 0;						// size of buffer
	char* m_data = nullptr;		// pointer to data area (may be dynamically or external)
	bool isDynamic_m_data = false;	// =1 - the memory for buffer has been allocated dynamically

	/**********   Functions   ************/
	/***************************************************************/
	// Full clear buffer with wrute 0 in data area and return buffer's 
	// pointers to the beginning
	void clear();
	/***************************************************************/
	// Calculation parameters of buffer as length. Must be call after 
	// change pointer of buffer, like call setIndexForRead()
	void calcLength();

	/*****************************************************************
	 * @brief Transfer not readed path in buffer to start of buffer
	 (for linear buffer only)
	****************************************************************/
	#ifdef BUF_TYPE_LINEAR
	void transferOnBeginOfBuffer();
	#endif
};

/************************ (C) **END OF FILE****/


