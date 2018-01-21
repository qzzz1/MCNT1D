#include <string>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>

#include "MonteCarlo.h"
#include "matlab.h"


/*------------------------------------------------------------------
	功能：获取命令窗口某个命令的执行结果。
	参数：命令
	返回：执行结果（若结尾有回车，则去除回车）
	示例：cmdExecuteResult("cd");
-------------------------------------------------------------------*/
std::string cmdExecuteResult(const char* cmd)
{
	std::array<char, 256> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) throw std::runtime_error("popen() failed!");
	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 256, pipe.get()) != NULL)
			result += buffer.data();
	}
	int resultLength = result.length();
	if (result[resultLength - 1] == '\n') {
		result.erase(resultLength - 1);
	}
	return result;
}

void MonteCarlo::output() {
	//检查能否打开输出文件
	if (!this->outputFile) {
		std::cout << "Error! E008: Can't open file \"" << this->outputFileName << "\"!" << std::endl;
	}
	else {
		std::ofstream matlabPlotData("data.temp");
		if (!matlabPlotData) {
			std::cout << "Error! E009: Can't open file \"data.temp\"!" << std::endl;
		}
		//输出非活跃代有效增殖系数
		outputFile << "Inactive Keff:" << std::endl
			<< "Cycle       Keff" << std::endl;
		for (int iGenerationCount = 1; iGenerationCount <= this->inactiveGenerationNumber; iGenerationCount++) {
			outputFile << std::setw(5) << iGenerationCount << "     " << std::setw(8)<<std::setprecision(6)<<Keff[iGenerationCount] << std::endl;
			matlabPlotData << std::setw(5) << iGenerationCount << "     " << std::setw(8)<<std::setprecision(6)<<Keff[iGenerationCount] << std::endl;
		}
		//输出活跃代有效增殖系数
		outputFile << "Active cycle Keff:" << std::endl
			<< "Cycle       Keff         Average           St.d" << std::endl;
		for (int iGenerationCount = inactiveGenerationNumber + 1; iGenerationCount <= totalGenerationNumber; iGenerationCount++) {
			outputFile << std::setw(5) << iGenerationCount << "     "
				<< std::setw(8) << std::setprecision(6) << Keff[iGenerationCount] << "     "
				<< std::setw(8) << std::setprecision(8) << averageKeff[iGenerationCount] << "     "
				<< std::setw(8) << std::setprecision(8) << this->KeffStandardDeviation[iGenerationCount] << std::endl;
			matlabPlotData << std::setw(5) << iGenerationCount << "     " << std::setw(8)<<std::setprecision(6) << averageKeff[iGenerationCount] << std::endl;
		}

		//输出通量
		outputFile << "Flux:" << std::endl;
		for (int i = 1; i <= this->groupNumber; i++) {
			outputFile << "Group " << i << ":" << std::endl;
			for (int j = 1; j <= this->cellNumber; j++) {
				outputFile << "Cell " << std::setw(3) << j << ": "
					<< std::setw(8) << std::setprecision(6) << this->averageFlux[i][j] << "       "
					<< std::setw(8) << std::setprecision(6) << this->fluxStandardDeviation[i][j] << std::endl;
			}
		}
	}
	outputFile.close();
	
	std::ofstream matlabScript("scriptTemp.m");
	//获取当前路径
	std::string dir = cmdExecuteResult("cd");
	matlabScript<<"a=load('"<<dir<<"\\data.temp');\n"
	//matlabScript << "a=load('C:\\Users\\LI Jin\\Documents\\GitHub\\MCNT1D\\MCNT1D\\MCNT1D\\data.temp');\n"
		<< "plot(a(:,1),a(:,2));\n"
		<< "xlabel('Generation Number');\n"
		<< "ylabel('Keff');\n"
		<< "legend('Keff');\n";
	matlabScript.close();
	//调用matlab输出Keff
	std::string commandTemp = "run('" + dir + "\\scriptTemp.m');";
	//std::string commandTemp = "run('C:\\Users\\LI Jin\\Documents\\GitHub\\MCNT1D\\MCNT1D\\MCNT1D\\scriptTemp.m');";
	iMatlab mtlb(commandTemp);
	mtlb.open();
	mtlb.run();
	std::system("pause");
	//关闭引擎
	mtlb.close();

	//system("del *.temp");           //删除临时文件
	//system("del scriptTemp.m");     //删除临时脚本
}