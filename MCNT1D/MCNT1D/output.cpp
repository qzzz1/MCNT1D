#include <string>

#include "MonteCarlo.h"
#include "matlab.h"

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
			<< "Cycle       Keff       Average" << std::endl;
		for (int iGenerationCount = inactiveGenerationNumber + 1; iGenerationCount <= totalGenerationNumber; iGenerationCount++) {
			outputFile << std::setw(5) << iGenerationCount << "     " << std::setw(8)<<std::setprecision(6) << Keff[iGenerationCount] << "     " << std::setw(8) <<std::setprecision(8)<< averageKeff[iGenerationCount] << std::endl;
			matlabPlotData << std::setw(5) << iGenerationCount << "     " << std::setw(8)<<std::setprecision(6) << averageKeff[iGenerationCount] << std::endl;
		}
	}
	outputFile.close();
	
	std::ofstream matlabScript("scriptTemp.m");
	matlabScript << "a=load('C:\\Users\\LI Jin\\Documents\\GitHub\\MCNT1D\\MCNT1D\\MCNT1D\\data.temp');\n"
		<< "plot(a(:,1),a(:,2));\n"
		<< "xlabel('Generation Number');\n"
		<< "ylabel('Keff');\n"
		<< "legend('Keff');\n";
	matlabScript.close();
	//调用matlab输出Keff
	std::string commandTemp = "run('C:\\Users\\LI Jin\\Documents\\GitHub\\MCNT1D\\MCNT1D\\MCNT1D\\scriptTemp.m');";
	iMatlab mtlb(commandTemp);
	mtlb.open();
	mtlb.run();
	std::system("pause");
	mtlb.close();
}