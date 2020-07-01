#pragma once
/*******************************************************
	Класс для организации кольцевого буфера



*/

#include "SysConst.h"
#include <stdint.h>
#include <string>

#define bufResultIsNG		-1
#define bufResultNG			-1

// minimum buffer size. default for dynamic data
#define MIN_BUFFER_SIZE	512

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
	Buffer(char* buf = 0, int size = MIN_BUFFER_SIZE);

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
	 * @brief	Put c_str with terminator in buffer.
	 * If string starts with 0 - not copy.
	 * If function return bufResultNG, the buffer returns 
	 * to previous state.
	 * @param str <const char*> - input c_style string
	 * @return next index of buffer for write operation
				or bufResultNG, if buffer is full
	*****************************************************/
	int put(const char* str);

	/****************************************************
	 * @brief	Put std::string in buffer with terminator.
	 * If string starts with 0 - not copy.
	 * If function return bufResultNG, the buffer returns
	 * to previous state.
	 * @param str <std::string> - input string
	 * @return next index of buffer for write operation
				or bufResultNG, if buffer is full
	*****************************************************/
	int put(std::string& str);

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
	 * @brief	Get string from buffer.
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
	 * @brief operator= group is similar of put() with StrEndSymbol='\0', 
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
	 * @brief	Put c_str with terminator in buffer.
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
	 * @brief	Put std::strind in buffer.
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
	* @brief operator+= group is similar of put() with StrEndSymbol='\0'
	************************************************************/

	/****************************************************
	* @brief	Put char in buffer.
	* @param byte <const char> - input char
	* @return next index of buffer for write operation
			or bufResultNG, if buffer is full
	*****************************************************/
	int operator+=(const char byte);

	/****************************************************
	* @brief	Put c_str in buffer.
	* If string starts with 0 - not copy.
	* @param str <const char*> - input string
	* @return next index of buffer for write operation
			or bufResultNG, if buffer is full
	*****************************************************/
	int operator+=(const char* str);

	/****************************************************
	* @brief	Put std::string in buffer.
	* If string starts with 0 - not copy.
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
	 * @param isReturnIndAfterStr <bool>:
	 *		if = false or not defined - the function return index in buffer where found start of input string 
	 *		if = true - the function return next index in buffer where found end of input string
	 * @param pStartInBuf <int> - start index for search. If 0 or not defined - the seach begins
	 *		with index for read operation
	 * @return <int> - index of buffer or bufResultNG if not found
	*****************************************************/
	int search(const char* str, bool isReturnIndAfterStr = false, int pStartInBuf = 0 );


	/****************************************************
	/*********			PROTECTED SECTION			 *************/
	/***************************************************/
protected:


	/****************************************************
	/*********			PRIVATE SECTION			 *************/
	/***************************************************/
private:

	//Переменная состояния буфера
	typedef enum BUF_STATE_ENUM
	{
		EMPTY,	//пустой
		NORMAL,	//нормальная работа буфера без переполнения
		OVF		//буфер полностью забит
	}BUF_STATE_ENUM;

	BUF_STATE_ENUM volatile m_state = EMPTY;  //Переменная состояния буфера
	//указывает на свободную ячейку после записи в буфер
	//изменяется после каждой записи в буфер
	int volatile m_front = 0;
	//указывает на первый байт кот. нужно считать (необработанные данные)
	//изменяется при считывании с буфера
	int volatile m_end = 0;
	//длина занятой области. Считается в программе bufCalcLength
	int volatile m_len = 0;
	//максимальная длина занятой области. для анализа заполнения буфера
	int volatile m_maxLen = 0;
	int m_size = 0;						// размер буфера ( для ф-й bufClear и bufCalcLength ) т.к. sizeof(Buf)=2
	char* m_data = nullptr;		// массив данных буфера
	bool isDynamic_m_data = false;	// =1 - память была выделена динамически
	/**********   Functions   ************/
	/***************************************************************/
	// Полной очистка буфера с инициализацией нулями. Указатели буфера - на начало.
	void clear();
	/***************************************************************/
	// Необходимо применять при прыжках по буферу, например после изменеия указателей буфера
	// Подсчет разницы между указателями end и front, формирование флагов буфера
	void calcLength();
};




/********************************************************
* Копирование с одного буфера в другой до указанного символа
* (не переписывается). Завершает строку \r\n в приемнике, если надо
* Вход:	*srcBufFl - буфер источника
* 			*dstBufFl - буфер приеммника
*				u08 symbol - до этого символа переписываем
*				u08 newStrFl - !=0 -вставляем в конец символ "\r\n"
* Выход: 	HAL_OK (0) - копирвоание успешно
*					HAL_ERROR (1) - копирвоание не успешно. Указатели буферов возвращены в исх.
*********************************************************/
/*u08 bufCopy(BUFFER_STRUCT* srcBufFl, BUFFER_STRUCT* dstBufFl, u08 symbol, u08 newStrFl)
{
	u16 end = srcBufFl->End;
	u16 front = dstBufFl->Front;
	while (srcBufFl->Data[srcBufFl->End] != symbol) {
		if (srcBufFl->State != Empty) {
			bufPutByte(dstBufFl, bufGetByte(srcBufFl));
		}
		else if (dstBufFl->State == Overflow) {
			// ошибка копирования. не дошли до symbol, а буфер уже пустой
			// или переполнение приемника.
			// возврат указателей, выход с ошибкой
			srcBufFl->End = end;
			dstBufFl->Front = front;
			bufCalcLength(srcBufFl);
			bufCalcLength(dstBufFl);
			return HAL_ERROR;
		}
	}
	if (newStrFl) { bufPutNewStr(dstBufFl); }
	if (dstBufFl->State == Overflow) {
		srcBufFl->End = end;
		dstBufFl->Front = front;
		bufCalcLength(srcBufFl);
		bufCalcLength(dstBufFl);
		return HAL_ERROR;
	}
	return HAL_OK;
}

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


