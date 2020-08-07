#pragma once

#include <fstream>

class EEPROMWin32
{
public:

	EEPROMWin32(const char* fileName = nullptr)
	{
		if(fileName)	m_fileName = fileName;
	}


	~EEPROMWin32() = default;

	EEPROMWin32(const EEPROMWin32&) = delete;
	EEPROMWin32(EEPROMWin32&&) = delete;
	EEPROMWin32& operator=(const EEPROMWin32&) = delete;
	EEPROMWin32& operator=(EEPROMWin32&&) = delete;

	/********************************************************/
	bool save(const void* pData, unsigned size)
	{
		//std::ofstream file(m_fileName, std::ios::binary);
		//if (file) {
		//	uint8_t* data = (uint8_t*)(pData);
		//	for (unsigned i = 0; i < size; ++i) {
		//		file.put(data[i]);
		//	}
		//	return true;
		//}
		return false;
	}

	/********************************************************/
	bool read(void* pData, unsigned size)
	{
		//std::ifstream file(m_fileName, std::ios::binary);
		//if (file) {
		//	uint8_t* data = (uint8_t*)(pData);
		//	for (unsigned i = 0; i < size; ++i) {
		//		data[i] = file.get();
		//	}
		//	return true;
		//}
		return false;
	}

	/********************************************************/
	std::string& getFileName() {
		return m_fileName;
	}


private:
	//static std::string m_fileName;
	std::string m_fileName = "";
};

