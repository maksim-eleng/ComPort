#pragma once

/************************************************
 * @brief Convert integer value to c_style string
 * @param num <int> - input integer
 * @param res <char*> - external buffer for c_style res[12] maximum for -INT_MAX value
 *					If num < 0 - res="-XXXX"
***********************************************/
void convIntToStr(int num, char* res);

/************************************************
 * @brief Convert c_style string to integer value. 
 * @param str <char*> - string c_style.
 * @param res <int> - out integer
 *					If str="-XXXX" - res is negative
 * @return <bool> true - OK
 *								false - res > INT_MAX
***********************************************/
bool convStrToInt(const char* str, int& res);