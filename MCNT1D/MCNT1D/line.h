#ifndef LINE_H
#define LINE_H
#endif // !LINE_H


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

	/*-----------------------------------------------------------------------------
	Construct function.
	The argument is the line.
	eg:		iLine egLine("This is a test line.");
	egLine iLine("This is a new line to replace the old line.");
	------------------------------------------------------------------------------*/
	iLine(std::string _aLine_);

	iLine() = default;

	/*--------------------------------------------------
	This functions returns length of the line.
	eg:		iLine egLine("This is a test line.");
	std::cout << egLine.lineLength() << endl;
	The output of the two lines will be "20".
	---------------------------------------------------*/
	unsigned int lineLength();

	/*----------------------------------------------------------------------------------------------------
	This function returs the line as a string.
	eg:		iLine egLine("This is a test line.");
	std::cout << egLine.str() << endl;
	The output of the two lines will be "This is a test line.", as an object of "std::string" class.
	------------------------------------------------------------------------------------------------------*/
	std::string str();

	/*-----------------------------------------------------------------------------
	This function tests if there is some keyword in a sentence.
	The argument is the keyword to be found.
	eg:		iLine egLine("This is a test line.");
	std::cout << egLine.isThere("This") << endl;
	The output of the two lines will be "1".
	------------------------------------------------------------------------------*/
	bool isThere(std::string _iKeyword_);

	bool isEmpty();

	/*--------------------------------------------------------------------------------------------------------------
	This function tests where is some keyword in a sentence.
	Return the position, which starts from "1" instead of "0", of the first letter of the keyword.
	The argument is the keyword to be found.
	eg:		iLine egLine("This is a test line.");
	std::cout << egLine.whereIs("test") << endl;
	The output of the two lines will be "11".
	--------------------------------------------------------------------------------------------------------------*/
	unsigned int whereIs(std::string _iKeyword_);

	/*-------------------------------------------------------------------------------
	This function counts how many words there is in a sentence, divided by space.
	eg:		iLine egLine("This is a test line.");
	std::cout << egLine.countWords() << endl;
	The output of the two lines will be "5".
	-------------------------------------------------------------------------------*/
	unsigned int countWords();

	/*--------------------------------------------------------------------------------------------------------------
	This function tests where is some keyword in a sentence.
	Return the sequence number, which starts from "1" instead of "0", of the keyword in a sentence.
	The argument is the keyword to be found.
	eg:		iLine egLine("This is a test line.");
	std::cout << egLine.wordNumber("is") << endl;
	The output of the two lines will be "2".
	--------------------------------------------------------------------------------------------------------------*/
	unsigned int wordNumber(std::string _iKeyword_);

	/*--------------------------------------------------------------------------------------------------------------------------
	This function returns a string from beginning to the char position(begins from "0") designated, without the designated char.
	The argument is the char position in the sentence.
	eg:		iLine egLine("This is a test line.");
	std::cout << egLine.before(6) << endl;
	The output of the two lines will be "This i".
	--------------------------------------------------------------------------------------------------------------------------*/
	std::string before(unsigned int _charNumber_);

	/*----------------------------------------------------------------------------------------------------------------------------------------
	This function returns a string from the char position(begins from "0") designated to the end of the sentence, include the designated char.
	The argument is the char position in the sentence.
	eg:		iLine egLine("This is a test line.");
	std::cout << egLine.after(6) << endl;
	The output of the two lines will be "s a test line.".
	-----------------------------------------------------------------------------------------------------------------------------------------*/
	std::string after(unsigned int _charNumber_);

	/*----------------------------------------------------------------------------
	This operator [] returns the "i"th word in a sentence, which starts from "0".
	The argument is the word number.
	eg:		iLine egLine("This is a test line.");
	std::cout << egLine[3] << endl;
	The output of the two lines will be "test".
	-----------------------------------------------------------------------------*/
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
