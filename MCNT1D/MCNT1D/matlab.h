#pragma once
#ifndef MATLAB_H
#define MATLAB_H

#include <string>

#include "engine.h"    // MATLAB calculating engine

#pragma comment (lib,"libmat.lib")
#pragma comment (lib,"libeng.lib")
#pragma comment (lib,"libmx.lib")

class iMatlab {
private:
	std::string matlabCmd;  //存储要执行的matlab命令
	Engine *engine;         //matlab引擎
public:
	//构造函数
	iMatlab(std::string _matlabCmd) :matlabCmd(_matlabCmd) {}

	bool open();

	/*---------------------------------------------
		功能：运行matlab引擎。
		参数：无
		返回：布尔值，运行成功返回真，运行失败返回假
		示例：mt.run();
	--------------------------------------------*/
	bool run();

	void close();
};

#endif // !MATLAB_H