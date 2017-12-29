#pragma once
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <stdexcept>

class mcException {
private:
	std::string message;         //异常信息
public:
	//构造函数
	mcException(const std::string &_message) :message(_message) {}

	//析构函数
	~mcException() = default;
	
	//查看异常信息
	const std::string &getMessage() const { return message; }
};

#endif // !EXCEPTION_H

