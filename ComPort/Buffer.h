#pragma once
/*******************************************************
	Класс для организации кольцевого буфера



*/

#include "SysConst.h"
#include <stdint.h>
#include <string>

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
	 * @brief	Put c_str in buffer without terminator or with. 
	 * If string starts with 0 - not copy.
	 * If function return bufResultNG, the buffer returns 
	 * to previous state.
	 * @param str <const char*> - input c_style string
	 * @param fPutTerminator <bool> = true - put str + terminator
	 * @return next index of buffer for write operation
				or bufResultNG, if buffer is full
	*****************************************************/
	int put(const char* str, bool fPutTerminator = false);

	/****************************************************
	 * @brief	Put std::string in buffer without terminator or with.
	 * If string starts with 0 - not copy.
	 * If function return bufResultNG, the buffer returns
	 * to previous state.
	 * @param str <std::string> - input string
	 * @param fPutTerminator <bool> = true - put str + terminator
	 * @return next index of buffer for write operation
				or bufResultNG, if buffer is full
	*****************************************************/
	int put(std::string& str, bool fPutTerminator = false);

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
	 * @brief	Get string in c_style format from buffer. 
	 * If data in buffer starts with '\0' - not copy.
	 * If function complete with error, the buffer returns
	 * to previous state.
	 * @param str <char*> - external buffer for destination.
	 * @param sizeStr <int> - size of external buffer
	 * @return next buffer's index for read operation
	 *						or 	bufResultIsNG - terminator not 
	 *						found before buffer is empty
	*****************************************************/
	int get(char* str, int sizeStr);

	/****************************************************
	 * @brief	Get std::string from buffer.
	 * If data in buffer starts with '\0' - not copy.
	 * If function complete with error, the buffer returns
	 * to previous state.
	 * @param str <std::string> - external string for destination.
	 * @return next buffer's index for read operation
	 *						or 	bufResultIsNG - terminator not
	 *						found before buffer is empty
	*****************************************************/
	int get(std::string& str);

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
	 * @brief	Put c_str in buffer until the user EOF char
	 * (if defined) or terminator is writes to buffer.
	 * The buffer will be cleared before put.
	 * If string starts with 0 - not copy.
	 * If function return bufResultNG, the buffer returns
	 * to previous state.
	 * @param str <const char*> - input c_style string
	 * @return next index of buffer for write operation
				or bufResultNG, if buffer is full
	*****************************************************/
	int operator=(const char* str);

	/****************************************************
	 * @brief	Put std::string in buffer until the user EOF char
	 * (if defined) or terminator is writes to buffer.
	 * The buffer will be cleared before put.
	 * If string starts with 0 - not copy.
	 * If function return bufResultNG, the buffer returns
	 * to previous state.
	 * @param str <std::string> - input string
	 * @return next index of buffer for write operation
				or bufResultNG, if buffer is full
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
	 * @brief	Put c_str in buffer until the user EOF char
	 * (if defined) or terminator is writes to buffer.
	 * If string starts with 0 - not copy.
	 * If function return bufResultNG, the buffer returns
	 * to previous state.
	 * @param str <const char*> - input c_style string
	 * @return next index of buffer for write operation
				or bufResultNG, if buffer is full
	*****************************************************/
	int operator+=(const char* str);

	/****************************************************
	 * @brief	Put std::string in buffer until the user EOF char
	 * (if defined) or terminator is writes to buffer.
	 * If string starts with 0 - not copy.
	 * If function return bufResultNG, the buffer returns
	 * to previous state.
	 * @param str <std::string> - input string
	 * @return next index of buffer for write operation
				or bufResultNG, if buffer is full
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
	 * @brief Copy to another buffer until the user EOF char
	 * (if defined) or terminator is writes to destination.
	 * @param dstBuf <Buffer> - buffer of receiver
	 * @return	true - Copy success
	 *					false - the EOF char not found in buffer of 
	 *						source while copy operation. All buffers 
	 *						will be return to previous state
	******************************************************/
	bool copyStrTo(Buffer& dstBuf);

	/*****************************************************
	 * @brief Transfer to another buffer until the user EOF char
	 * (if defined) or terminator is writes to destination.
	 * @param dstBuf <Buffer> - buffer of receiver
	 * @return	true - Transfer success
	 *					false - the EOF char not found in buffer of
	 *						source while copy operation. All buffers
	 *						will be return to previous state
	******************************************************/
	bool transferStrTo(Buffer& dstBuf);


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
	// leng of data occupied area
	int volatile m_len = 0;
	// maximum leng of data occupied area
	int volatile m_maxLen = 0;
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
};





/********************************************************
* Поиск символа в буфере с выборкой из буфера
* Вход:	BUFFER_STRUCT *BufFl - буфер где ищем
* 			u08 Byte - что ищем
* Выход:	res = Byte - символ найден
* 				res = 0xFF - символ не найден и буфер пустой
* 				BufFl->End на выходе указывает на байт за найденным
*********************************************************/
/*u08 bufSearchByte(BUFFER_STRUCT* BufFl, u08 Byte)
{
	u08 res = 0;
	while ((res = bufGetByte(BufFl)) != Byte && res != 0xFF);
	return res;
}





/************************ (C) **END OF FILE****/


