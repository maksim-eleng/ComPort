#pragma once

#include <fstream>

class EEPROMWin32
{
public:

	EEPROMWin32(const char* fileName = 0)
	{
		if (fileName)
			m_fileName = *fileName;
	}
	~EEPROMWin32() = default;

	EEPROMWin32(const EEPROMWin32&) = delete;
	EEPROMWin32(EEPROMWin32&&) = delete;
	EEPROMWin32& operator=(const EEPROMWin32&) = delete;
	EEPROMWin32& operator=(EEPROMWin32&&) = delete;

protected:

	/********************************************************/
	bool writeFile(const void* pData, size_t size)
	{
		std::ofstream file(m_fileName, std::ios::binary);
		if (file) {
			uint8_t* data = (uint8_t*)(pData);
			for (size_t i = 0; i < size; ++i) {
				file.put(data[i]);
			}
			return true;
		}
		return false;
	}

	/********************************************************/
	bool readFile(std::string& raw)
	{
		std::ifstream file(m_fileName, std::ios::binary);
		if (file) {
			raw.reserve(128);
			while (file) {
				raw += file.get();
			}
			return true;
		}
		return false;
	}

	/********************************************************/
	bool readBlock(const size_t startInd, std::vector<char>& dst)
	{
		std::ifstream file(m_fileName, std::ios::binary);
		if (file) {
			file.seekg(startInd);
			for (size_t i = dst.size(); i < dst.capacity(); ++i) {
				dst.emplace_back(file.get());
				if (!file.good())
					return false;
			}
			return true;
		}
		return false;
	}

	/********************************************************/
	bool writeBlock(const std::vector<char>& src, const size_t startInd)
	{
		std::ofstream file(m_fileName, std::ios::binary);
		if (file) {
			file.seekp(startInd);
			file.write(src.data(), src.size());
			return true;
		}
		return false;
	}

private:
	std::string m_fileName = "";
};

