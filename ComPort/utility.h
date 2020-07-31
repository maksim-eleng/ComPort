#pragma once

#include <vector>
#include <string>

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
 *								false - module of res > INT_MAX. In this case res = 0.
***********************************************/
bool convStrToInt(const char* str, int& res);

/************************************************
* @brief Convert std::string to integer value. 
* @param str <char*> - string c_style.
* @param res <int> - out integer
*					If str="-XXXX" - res is negative
* @return <bool> true - OK
*								false - module of res > INT_MAX. In this case res = 0.
***********************************************/
bool convStrToInt(std::string& str, int& res);


/************************************************
* @brief Convert argument of main() to vector<int> 
* @param standart of main arguments
* @return std::vector<int> with integer value. If field of vector == 0 - 
*					parameter can't be converted to int
***********************************************/
std::vector<int> convArgumentsOfMainToInt(int argc, char* argv[]);