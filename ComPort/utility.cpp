#include "utility.h"
#include <stdlib.h>

/***********************************************/
void convIntToStr(int num, char* res)
{
	int i = 0;
	int k = 0;
	char buf[12] = { 0 };
	if (num < 0) {
		res[i++] = '-';
		num = abs(num);
	}
	while (num) {
		buf[k++] = num % 10 + '0';
		num /= 10;
	}
	while (k) {
		res[i++] = buf[--k];
	}
	res[i] = '\0';
}

/***********************************************/
bool convStrToInt(const char* str, int& res)
{
	unsigned int tmpRes = 0;
	int ind = 0;
	res = 0;
	if (str[ind] == '-')
		++ind;
	while (str[ind] != '\0') {
		tmpRes *= 10;
		tmpRes += str[ind] - '0';
		if (tmpRes > INT_MAX || str[ind] < '0' || str[ind] > '9') {
			res = 0;
			return false;
		}
		++ind;
	}
	res = tmpRes;
	if (str[0] == '-')
		res = -res;
	return true;
}

/***********************************************/
bool convStrToInt(std::string& str, int& res)
{
	return convStrToInt(str.c_str(), res);
}

/***********************************************/
std::vector<int> convArgumentsOfMainToInt(int argc, char* argv[])
{
	std::vector<int>res;
	// first of argument can not be integer
	if (argc > 1) {
		int intRes = -1;
		for (int i = 0; i < argc; ++i) {
			convStrToInt(argv[i], intRes);
			res.emplace_back(intRes);
		}
	}
	return std::move(res);
}
