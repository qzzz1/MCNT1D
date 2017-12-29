#include <iostream>

#include "matlab.h"

bool iMatlab::open() {
	std::cout << "The program is launching Matlab R2016a engine..." << std::endl;

	if (!(this->engine = engOpen(NULL))) {            // test if successfully launched matlab engine
		std::cout << "Can't start Matlab engine!" << std::endl;
		return false;
	}
	else return true;
}

bool iMatlab::run() {
	return engEvalString(this->engine, this->matlabCmd.c_str());
}

void iMatlab::close() {
	engClose(this->engine);
}