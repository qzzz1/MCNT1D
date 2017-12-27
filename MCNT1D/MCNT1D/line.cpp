#include "line.h"
#pragma warning(disable:4996)

iLine::iLine(std::string _string_):_aLine_(_string_){}

unsigned int iLine::lineLength()
{
	return (this->_aLine_).length();
}

std::string iLine::str()
{
	return this->_aLine_;
}

unsigned int iLine::countWords()
{
	std::string _sentence_ = this->_aLine_;
	unsigned int wordNumber = 0;
	bool spaceFlag = false, countingFlag = false;
	if (_sentence_[0] == ' ') {
		spaceFlag = true;
	}
	else {
		spaceFlag = false;
		wordNumber++;
		countingFlag = true;
	}
	unsigned int _sentence_length = (unsigned int)_sentence_.length();
	for (unsigned int i = 1; i<_sentence_length; i++)
	{
		if (_sentence_[i] == ' ') {
			if (countingFlag) {
				countingFlag = false;
			}
			else {
				continue;
			}
		}
		else {
			if (countingFlag) {
				continue;
			}
			else {
				wordNumber++;
				countingFlag = true;
			}
		}
	}

	return wordNumber;
}

unsigned int iLine::whereIs(std::string _iKeyword_)
{
	std::string buff = this->_aLine_;
	char *buf;
	buf = (char *)malloc((this->_aLine_).length() * sizeof(char));
	(this->_aLine_).copy(buf, (this->_aLine_).length());
	std::string token = _iKeyword_;
	char *p;
	int s = -1, len = token.length(), pos = -1;
	p = buf;

	while (*p) {
		if (*p == token[0] && s == -1) {
			s = 0;
		}
		else if (*p == token[s + 1]) {
			s++;
		}
		else {
			s = -1;
		}
		p++;
		if (s == len - 1) {
			s = -1;
			return p - buf - len;
		}
		//	s = -1;
	}
	return 0;
}

bool iLine::isThere(std::string _iKeyword_)
{
	std::istringstream aLine(this->_aLine_);
	std::string aWord;
	unsigned int iLineWordNumber = this->countWords();
	for (unsigned int i = 0; i < iLineWordNumber; i++)
	{
		aLine >> aWord;
		if (aWord == _iKeyword_) {
			return true;
		}
		else {
			continue;
		}
	}
	return false;
}

bool iLine::isEmpty() {
	for (int i = 0; i < this->lineLength(); i++) if(this->_aLine_[i] != ' ') return false;
	return true;
}

unsigned int iLine::wordNumber(std::string _iKeyword_)
{
	std::string aWord;
	std::istringstream aLine(this->_aLine_);
	unsigned int iLineWordNumber = this->countWords();
	for (unsigned int i = 1; i <= iLineWordNumber; i++)
	{
		aLine >> aWord;
		if (aWord == _iKeyword_) {
			return i;
		}
		else {
			continue;
		}
		return 0;
	}
}

std::string iLine::before(unsigned int _charNumber_)
{
	if (_charNumber_ <= 0 || _charNumber_ >= (this->_aLine_).length()) {
		std::cout << "ERROR! Not in range!" << std::endl;
		system("pause");
	}
	else {
		std::string thisLine = this->_aLine_;
		std::string beforeLine = thisLine.erase(_charNumber_, thisLine.length() - 1);
		return beforeLine;
	}
}

std::string iLine::after(unsigned int _charNumber_)
{
	if (_charNumber_ <= 0 || _charNumber_ >= (this->_aLine_).length()) {
		std::cout << "ERROR! Not in range!" << std::endl;
		system("pause");
	}
	else {
		std::string thisLine = this->_aLine_;
		std::string afterLine = thisLine.erase(0, _charNumber_);
		return afterLine;
	}
}

std::string iLine::operator[] (unsigned int _wNumber_)
{
	std::istringstream thisLine(this->_aLine_);
	std::string wNumberWord;
	unsigned int cntWords = this->countWords();

	if (_wNumber_ >= cntWords || _wNumber_ < 0) {
		std::cout << "ERROR! Subscript out of range!" << std::endl;
		system("pause");
	}
	else {
		unsigned int i = 0;
		do {
			i++;
			thisLine >> wNumberWord;
		} while (i <= _wNumber_);
		return wNumberWord;
	}
}

std::ostream &operator<< (std::ostream &out, const iLine &_line_) {
	out << _line_._aLine_;
	return out;
}

/*---------------------------------------------------------------------
	This function calls MATLAB engine to excute the command you enter.
	The argument is the command you want to excute.
----------------------------------------------------------------------*/
/*
bool iMatlab(std::string _matlabCommand_)
{
	std::cout << "The program is launching Matlab R2016a engine..." << std::endl;
	Engine *ep;
	if (!(ep = engOpen(NULL))) {                                         // test if successfully launched matlab engine
		std::cout << "Can't start Matlab engine!" << std::endl;
		return false;
	}
	else {
		std::cout << "Matlab engine was launched successfully!" << std::endl;
		engEvalString(ep, _matlabCommand_.c_str());
		return true;
	}
}
*/