#include <iostream>
#include <string>
#include "MonteCarlo.h"
#include "rand.h"

int main(int argc, char* argv[])
{
	
	std::string inputFileName, outputFileName;
	//读取命令行输入
	if (argc == 2) {
		//只有两个参数时，输出文件使用默认文件名
		inputFileName = argv[1];
		outputFileName = inputFileName + ".out";
	}
	else if (argc == 3) {
		//有三个参数时，输出文件使用用户定义的文件名
		inputFileName = argv[1];
		outputFileName = argv[2];
	}
	//参数个数错误
	else {
		std::cout << "Error! E000:Incorrect intput! Please use \"program_name input_file_name output_file_name\"" << std::endl;
	}

	MonteCarlo mcnt(inputFileName, outputFileName);
	mcnt.run();
	

	//for (int i = 0; i < 100; i++) {
	//	std::cout << random() << std::endl;
	//}
	system("pause");
	return 0;
}

