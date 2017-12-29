#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "line.h"
#include "exception.h"

#include <algorithm>
#include <map>
#include <sstream>

/*--------------------------------------------------------------------------------------------------
	表示输入文件中输入卡开始标记的枚举类。CC表示计算条件卡，CP表示计算参数卡，MT表示材料卡，CL表示栅元卡。
---------------------------------------------------------------------------------------------------*/
typedef enum { CC, CP, MT, CL } startFlag;

/*--------------------------------------------------------------------------
	功能：将一行文本转为iLine对象，删去“//”后面的注释内容，并将全部字符转为小写。
	参数：一行文本
	返回：iLine对象
	示例：translate(str);
---------------------------------------------------------------------------*/
iLine translate(std::string _line);

/*---------------------------------------
	功能：读取输入卡开始标记。
	参数：转为iLine的一行文本
	返回：开始标记的枚举类对象
	示例：getStartKeyword(ilineTemp)；
----------------------------------------*/
startFlag getStartKeyword(iLine _line) throw(mcException);

/*---------------------------------------------------
	功能：读取一行文本中的计算条件。
	参数：一行文本
	返回：一个映射，包含计算条件关键字和对应的数字
	示例：getCalculationCondition(line);
---------------------------------------------------*/
//std::pair<std::string, int> getCalculationCondition(iLine _line) throw(mcException);
template<typename T>
std::pair<std::string, T> getCalculationCondition(iLine _line) throw(mcException) {
	if (_line.countWords() != 2) throw("Error! E003: Syntax error: illegal parameter number for calculation condition!");
	std::stringstream __ssTemp;
	__ssTemp << _line[1];
	T __numberTemp;
	//将第二个单词对应的值转为int
	__ssTemp >> __numberTemp;
	std::pair<std::string, T> __conditionValueTemp(_line[0], __numberTemp);
	//将计算条件和数值存入映射容器
	//make_pair(_line[0], __numberTemp));
	return __conditionValueTemp;
}

/*---------------------------------
	功能：将字符串转为双精度浮点数。
	参数：字符串
	返回：双精度浮点数
	示例：s2d("2.33");
----------------------------------*/
double s2d(std::string _string);

/*---------------------------------
	功能：将字符串转为整数。
	参数：字符串
	返回：整数
	示例：s2i("233");
----------------------------------*/
int s2i(std::string _string);

#endif // !INPUT_H

