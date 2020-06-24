#pragma once
/*******************************************************
	����� ��� ����������� ���������� ������



*/

#include <string>
#include <cassert>
#include <stdint.h>

#define bufResultIsNG		-1
#define bufResultNG			-1

// minimum buffer size. default for dynamic data
#define MIN_BUFFER_SIZE	512

#ifndef DISABLE_INTERRUPT
#define DISABLE_INTERRUPT
#endif
#ifndef ENABLE_INTERRUPT
#define ENABLE_INTERRUPT
#endif


class Buffer
{
/*****			PUBLIC SECTION			 ******/
/**********************************************/
/*****	  Constructors & destructors	 ******/
/**********************************************/
public:

	/****************************************************
	*****	����������� ��������	*********************
	��� ������ ������������ �� ��������� ��������� �����
	� �������� MIN_BUFFER_SIZE
	��� �������� ������� ������� �������� ++m_objectCounter
	����:	char* _buf - ��������� �� ��� ��������� ������
			char* _buf = 0 - �� ��� ����� ����� �������� ����������� (�� ���������)
			int _size - ������ ������� ��� ������������� ��������� (=0 �� ���������)
	�������� �������:
	Buffer buf;	Buffer buf(NULL);	- � ������������ ����������, ������ MIN_BUFFER_SIZE ���� 
	Buffer buf(NULL, 100);			- � ������������ ����������, ������ 100 ���� 
	Buffer buf(buffer, sizeof(buffer));		- �������� ����� �������� ������, ������ 100 ����
	*****************************************************/
	Buffer(char* _buf = 0, int size = MIN_BUFFER_SIZE);
	// Copyer constructor - not for use 
 	Buffer(const Buffer& buf) = delete;
	// Destructor 
	/****************************************************
	*****		Destructor			*********************
	���� ���� �������� �� ����������� - �����������
	*****************************************************/
	~Buffer();

	/***************	Put section		**************/
	/*************************************************/

	/****************************************************
	*****	������ ������ � ����� 
	*****************************************************/
	int put(const char byte);
	/****************************************************
	*****	������ ������ � c_style � �����		*********
	���� ������ ���������� � '\0' - ������������
	������ ���������� ������ ��������������, ���� !='\0'
	����:	const char* str - ����� ���� buf[] ��� "Hellow"
			char StrEndSymbol - ������� ����� ������ (�� ��������� '\0')
	�����:	������ ������ ��� ��������� ������ ��� bufResultIsNG (=-1) - �� �������
	�����:	buf.put("Hellow\n", '\n');	���		buf.put("Hellow");
	*****************************************************/
	int put(const char* str, char StrEndSymbol='\0');
	/****************************************************
	*****	������ ������ � std::string � �����		*****
	���� ������ ���������� � '\0' - ������������
	������ ���������� ������ ��������������, ���� !='\0'
	����:	std::string& str - class std::string
			char StrEndSymbol - ������� ����� ������ (�� ��������� '\0')
	�����:	������ ������ ��� ��������� ������ ��� bufResultIsNG (=-1) - �� �������
	*****************************************************/
	int put(std::string& str, char StrEndSymbol = '\0');

	/************	Get section		******************/
	/*************************************************/

	/****************************************************
	*****	Return buffer;s size in byte	****************/
	int getBufSize()	const { return m_size; }

	/****************************************************
	*****	����� �� ������ ���� (char), ���� ����� �� ������	*****
	�����:	���� ��� bufResultIsNG (=-1), ���� ����� ������
	*****************************************************/
	char get();
	/****************************************************
	*****	����� �� ������ ���� (char) �� �������� �������, 
	���� �� �������� ������ ������	*****
	�� �������� ���������� ������
	����:	int& index - & �� ������� ����������, ���������������� �� ������
	�����:	���� ��� bufResultIsNG (=-1), ���� �������� ������ ������
	*****************************************************/
	char get(int& index);
	/****************************************************
	*****	����� �� ������ ������ � strEndSymbol ������������.
	������-��������� - � cstyle
	� ������ ��������� ������ ������ (��������� ����� ������ �� ������ 
	strEndSymbol ��� ��������� ��� ����� ����������) - c�����-��������� ����������� '\0',
	������������ bufResultNG
	����:	char* str - c�����-���������
			int sizeStr - ������ ������
			char strEndSymbol - ������ ����� ������ (='\0' �� ���������)
	�����:	������ ������ ��� ���������� ������ ���
			bufResultNG, ���� ������ ��������
	�����:	index = buf.get(buffer, sizeof(buffer), '\n');
	*****************************************************/
	int get(char* str, int sizeStr, char strEndSymbol = '\0');
	/****************************************************
	*****	����� �� ������ ������ � strEndSymbol ������������.
	������-��������� - std::string
	� ������ ��������� ������ ������ (��������� ����� ������ �� ������
	strEndSymbol) - ������������ bufResultNG
	����:	string& str - c�����-���������
			char strEndSymbol - ������ ����� ������ (='\0' �� ���������)
	�����:	������ ������ ��� ���������� ������ ���
			bufResultNG, ���� ������ ��������
	�����:	index = buf.get(string, '\n');
	*****************************************************/
	int get(std::string& str, char strEndSymbol = '\0');
	/* Get buffer index for Write operation*/
	inline int getIndexForWrite() const	{ return m_front; }
	/* Get buffer index for Read operation*/
	inline int getIndexForRead() const	{ return m_end; }
	/* Get number of created buffers */
	inline static int getObjectCounter() { return m_objectCounter;}
	/* Chech buffer. Return true, if not enpty*/
	inline bool checkIsNotEpty()	{ return (bool)m_len; }
	/* Chech buffer. Return true, if enpty*/
	inline bool checkIsEpty()			{ return !(bool)m_len; }
	/* Set buffer index for Write operation without check*/
	inline void setIndexForWrite(int newFrontIndex) 
		{
		m_front = newFrontIndex;
		calcLength();
		}
	/* Set buffer index for Read operation without check*/
	inline void setIndexForRead(int newEndIndex)  {
		m_end = newEndIndex;
		calcLength();
	}
	/***************************************************************/
	// ����������� ������ ���������, ��������� ������� �� ������ ������
	void resetIndex();



	/****   Operators overflow	******/

	/*	������ operator= - �� �� ��� � put(), �� � 
		��������������� �������� ������ */
	int operator=(const char byte);
	int operator=(const char* str);
	int operator=(std::string& str);
	// ����������� �������� ���������, � ��� ����� �����
	// ����������� �����������
	Buffer& operator=(const Buffer& buf) = delete;
	/*	������ operator+= - ���������� � put()	*/
	int operator+=(const char byte);
	int operator+=(const char* str);
	int operator+=(std::string& str);
	// ����� ���� �� ������ �� �������
	char operator[](int index);

	/**********	 Other function		******************/
	/*************************************************/

	/****************************************************
	*****	����� ������� �� ������ ������ (������������� �������).
	�� ������ ���������� ������.
	����:	char byte - ������ ��� ������
	�����:	������ ������ ��� ��������� ������� ��� 
			bufResultNG (=-1) - ������ �� ������
	�����:	index = buf.search('\n');
	*****************************************************/
	int search(char byte);
	/****************************************************
	*****	����� ������ � ������ �� ������ ������ (������������� �������).
	����� ������� �� '\0' � ������� ������ str, ������� � ������,
	�.�. � ������� �������������� ������. 
	�� ������ ���������� ������.
	����:	const char* str - ������ ��� ������
			bool isReturnIndAfterStr - ���������� ��� ���������� �-�: 
				false - ������ ������ ������ str � ������
				true - ������ ������ ����� ��������� ������ � ������ 
				�� ��������� = false
			int pStartInBuf - ��������� ������ ������. �� ��������� =0 � ���������� �� ������ 
				�������������� ������ (�� ����� ������)
	�����:	������ ������ ������ ��������� ������ (���� isReturnIndAfterStr=false) ���
			������ ������ ����� ��������� ������ � ������ (���� isReturnIndAfterStr=true) ���
			bufResultNG (=-1) - ������ �� �������
	�����:	index = buf.search("Hellow");	indexAfter = buf.search("Hellow", true);
	*****************************************************/
	int search(const char* str, bool isReturnIndAfterStr = false, int pStartInBuf = 0 );
	/****************************************************
	*****	������ �� ������ � ���������� ���������� ������
	������������: 
		����������� ������ - ���� isUnconditionalJump = 1 (�� ���������). 
			numSteps - ����� ������ ������. ������ ������������ ������� �������������� ������.
		���������������� ������ - isUnconditionalJump = 0
			numSteps ���������� ���-�� ����� ��������� ������ ������ (�����������
			�� ������). ��� ���������� ������ ������ - ������� � �������� ��������� 
			� �-� ���������� bufResultNG (=-1)
	����:	int numSteps - ����� �������� ������ ������ (��� ������������ ������).
				������ ������������ ������� �������������� ������.
				��� ���-�� ����� ��� ��������� (���� ���������� �����������)
			bool isUnconditionalJump - ���������� ����� ������: ����������� (�� ���������)
				��� ����������������
	�����:	����� �������� ������ ������ bufResultNG (=-1) - ������ �� ��������
	�����:	index = buf.jump(45);	index = buf.search(-10, false);
	*****************************************************/
	int jump(int numSteps, bool isUnconditionalJump = true);


	
/*****			PROTECTED SECTION			 ******/
/**************************************************/
protected:

/*****			PRIVAT SECTION			 **********/
/**************************************************/
private:
	//���������� ��������� ������
	typedef enum BUF_STATE_ENUM
	{
		EMPTY,	//������
		NORMAL,	//���������� ������ ������ ��� ������������
		OVF		//����� ��������� �����
	}BUF_STATE_ENUM;
	// ������� ��������� ��������
	static char m_objectCounter;

	BUF_STATE_ENUM volatile m_state = EMPTY;  //���������� ��������� ������
	//��������� �� ��������� ������ ����� ������ � �����
	//���������� ����� ������ ������ � �����
	int volatile m_front = 0;
	//��������� �� ������ ���� ���. ����� ������� (�������������� ������)
	//���������� ��� ���������� � ������
	int volatile m_end = 0;
	//����� ������� �������. ��������� � ��������� bufCalcLength
	int volatile m_len = 0;
	//������������ ����� ������� �������. ��� ������� ���������� ������
	int volatile m_maxLen = 0;
	int m_size = 0;						// ������ ������ ( ��� �-� bufClear � bufCalcLength ) �.�. sizeof(Buf)=2
	char* m_data = nullptr;		// ������ ������ ������
	bool isDynamic_m_data = false;	// =1 - ������ ���� �������� �����������
	/**********   Functions   ************/
	/***************************************************************/
	// ������ ������� ������ � �������������� ������. ��������� ������ - �� ������.
	void clear();
	/***************************************************************/
	// ���������� ��������� ��� ������� �� ������, �������� ����� �������� ���������� ������
	// ������� ������� ����� ����������� end � front, ������������ ������ ������
	void calcLength();
};




/********************************************************
* ����������� � ������ ������ � ������ �� ���������� �������
* (�� ��������������). ��������� ������ \r\n � ���������, ���� ����
* ����:	*srcBufFl - ����� ���������
* 			*dstBufFl - ����� ����������
*				u08 symbol - �� ����� ������� ������������
*				u08 newStrFl - !=0 -��������� � ����� ������ "\r\n"
* �����: 	HAL_OK (0) - ����������� �������
*					HAL_ERROR (1) - ����������� �� �������. ��������� ������� ���������� � ���.
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
			// ������ �����������. �� ����� �� symbol, � ����� ��� ������
			// ��� ������������ ���������.
			// ������� ����������, ����� � �������
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
* ����� ������� � ������ � �������� �� ������
* ����:	BUFFER_STRUCT *BufFl - ����� ��� ����
* 			u08 Byte - ��� ����
* �����:	res = Byte - ������ ������
* 				res = 0xFF - ������ �� ������ � ����� ������
* 				BufFl->End �� ������ ��������� �� ���� �� ���������
*********************************************************/
/*u08 bufSearchByte(BUFFER_STRUCT* BufFl, u08 Byte)
{
	u08 res = 0;
	while ((res = bufGetByte(BufFl)) != Byte && res != 0xFF);
	return res;
}



/********************************************************
* *********** ��������� ��������� End �����f.	*************
* C ������������������ ��������� ���������� ������
* ����:		BUFFER_STRUCT *BufFl - ��������� �� ����� ��� �������
* 				u16 newEnd - ����� �������� ��������� End
* �����:	bufChangeEnd(&uart[ch].rxBufFl, point);
*********************************************************/
/*void bufChangeEnd(BUFFER_STRUCT* BufFl, u16 newEnd)
{
	BufFl->End = newEnd;
	bufCalcLength(BufFl);
}

/************************ (C) **END OF FILE****/


