#ifndef LINE_H
#define LINE_H


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>    //string stream
#include <iomanip>    //format control

//#include "engine.h"                                                // MATLAB calculating engine

//#pragma comment (lib,"libmat.lib")
//#pragma comment (lib,"libeng.lib")
//#pragma comment (lib,"libmx.lib")

class iLine {
private:
	std::string _aLine_;
public:

	/*---------------------------------------------------------------
		功能：构造函数。
		参数：一行文本
		返回：iLine对象
		示例：iLine("This is a new line to replace the old line.");
	---------------------------------------------------------------*/
	iLine(std::string _aLine_);
	
	//默认构造函数
	iLine() = default;

	/*--------------------------
		功能：返回文本长度。
		参数：无
		返回：文本长度
		示例：lineLength();
	--------------------------*/
	unsigned int lineLength();

	/*---------------------
		功能：获取原字符串。
		参数：无
		返回：字符串
		示例：str();
	---------------------*/
	std::string str();

	/*----------------------------------------------------------
		功能：查询是否存在某个单词关键字。
		参数：关键字
		返回：布尔值，存在为真，不存在为假
		示例：	iLine egLine("This is a test line.");
				std::cout << egLine.isThere("test") << endl;
			The output of the two lines will be "1".
	---------------------------------------------------------*/
	bool isThere(std::string _iKeyword_);

	/*---------------------------------------------------------
		功能：判断是否为空。
		参数：无
		返回：布尔值，空为真，不空为假
		示例：	iLine egLine("This is a test line.");
				std::cout << egLine.isThere("test") << endl;
			The output of the two lines will be "1".
	---------------------------------------------------------*/
	bool isEmpty();

	/*----------------------------------------------------------
	功能：查询是否存在某个单词关键字的位置。
	参数：关键字
	返回：关键字所在位置的序号，序号从1开始
	示例：	iLine egLine("This is a test line.");
			std::cout << egLine.whereIs("test") << endl;
		The output of the two lines will be "11".
	---------------------------------------------------------*/
	unsigned int whereIs(std::string _iKeyword_);

	/*-----------------------------------------------------
		功能：统计单词个数，以空格为分隔符。
		参数：无
		返回：单词个数
		示例：	iLine egLine("This is a test line.");
				std::cout << egLine.countWords() << endl;
			The output of the two lines will be "5".
	------------------------------------------------------*/
	unsigned int countWords();

	/*-----------------------------------------------------------
		功能：搜索某个关键字的序号。
		参数：关键字
		返回：关键字所在序号，从0开始
		示例：	iLine egLine("This is a test line.");
				std::cout << egLine.before(6) << endl;
			The output of the two lines will be "This i".
	-----------------------------------------------------------*/
	unsigned int wordNumber(std::string _iKeyword_);

	/*-------------------------------------------------------
		功能：取某个位置（不含）前的所有字符。
		参数：位置
		返回：子串
		示例：	iLine egLine("This is a test line.");
				std::cout << egLine.before(6) << endl;
			The output of the two lines will be "This i".
	------------------------------------------------------*/
	std::string before(unsigned int _charNumber_);

	/*-------------------------------------------------------
		功能：取某个位置（不含）后的所有字符。
		参数：位置
		返回：子串
		示例：	iLine egLine("This is a test line.");
				std::cout << egLine.after(6) << endl;
			The output of the two lines will be "s a test line.".
	------------------------------------------------------*/
	std::string after(unsigned int _charNumber_);

	/*-----------------------------------------------------
		功能：重载[]运算符，取第i个单词。
		参数：单词序号，从0开始
		返回：单词
		示例：	iLine egLine("This is a test line.");
				std::cout << egLine[3] << endl;
			The output of the two lines will be "test".
	------------------------------------------------------*/
	std::string operator[] (unsigned int _wNumber_);

	/*-----------------------------------------------
		功能：重载流插入运算符，使能直接输出。
		参数：运算符左边是输出流对象，右边是iLine对象
		返回：输出流对象
		示例：std::cout<<myLine<<std::endl;
	-----------------------------------------------*/
	friend std::ostream &operator<< (std::ostream &out, const iLine &_line_);
};

/*---------------------------------------------------------------------
This function calls MATLAB engine to excute the command you enter.
The argument is the command you want to excute.
----------------------------------------------------------------------*/
//bool iMatlab(std::string _matlabCommand_);

#endif // !LINE_H
