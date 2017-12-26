#pragma once
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <stdexcept>

class mcException {
private:
	std::string message;
public:
	mcException(const std::string &_message) :message(_message) {}
	~mcException() = default;
	const std::string &getMessage() const { return message; }
};

#endif // !EXCEPTION_H

