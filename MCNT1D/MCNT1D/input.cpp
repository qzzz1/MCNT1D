#include "MonteCarlo.h"
#include "exception.h"
//#include "line.h"
#include "input.h"

#include <sstream>

iLine translate(std::string _line) {
	std::string lineWithoutComment = _line;
	if (_line.empty()) {
		iLine __tempLine("");
		return __tempLine;
	}
	//删去注释
	for (int i = 0; i < _line.length() - 1; i++) {
		if (_line[i] == '/'&&_line[i + 1] == '/') {
			lineWithoutComment = _line.substr(0, i);
			break;
		}
	}
	//全部转为小写
	std::transform(lineWithoutComment.begin(), lineWithoutComment.end(), lineWithoutComment.begin(), ::tolower);
	iLine __tempLine(lineWithoutComment);
	return __tempLine;
}

startFlag getStartKeyword(iLine _line) throw(mcException) {
	if (_line.isEmpty())throw("Warning! W001:Empty line for getStartKeyword!");
	std::string __firstWord = _line[0];
	if (__firstWord == "start_calculationcondition") {
		return CC;
	}
	else if (__firstWord == "start_calculationparameter") {
		return CP;
	}
	else if (__firstWord == "start_material") {
		return MT;
	}
	else if (__firstWord == "start_cell") {
		return CL;
	}
	else {
		throw("Error! E002:Unknow start keyword!");
	}
}



double s2d(std::string _string) {
	std::stringstream __ssTemp;
	double __result = 0;
	__ssTemp << _string;
	__ssTemp >> __result;
	return __result;
}

int s2i(std::string _string) {
	std::stringstream __ssTemp;
	int __result = 0;
	__ssTemp << _string;
	__ssTemp >> __result;
	return __result;
}

void MonteCarlo::readInput() throw(mcException) {
	if (!(this->inputFile)) throw("Error! E001: Cannot open input file!");
	//是否读完一个section的标记
	bool isReadSection = false;
	while (!(this->inputFile).eof()) {
		//从输入文件中读取一行并转为iLine对象
		std::string lineTemp;
		std::getline(this->inputFile, lineTemp);
		iLine lineRead = translate(lineTemp);

		//若是注释行则为空
		while (!lineRead.isEmpty()) {
			//如果读完了一个section，再读一行以跳过结束标记
			if (isReadSection) {
				std::getline(this->inputFile, lineTemp);
				lineRead = translate(lineTemp);
				isReadSection = false;
				break;
			}
			//第一个非注释行，应该是开始标记
			startFlag sf = getStartKeyword(lineRead);
			switch (sf)
			{
			//读取计算条件
			case CC: {
				std::string ccLineTemp;
				iLine ccLineRead;
				std::map<std::string, int> calculationConditionMap;
				do {
					//读取计算条件
					std::getline(this->inputFile, ccLineTemp);
					ccLineRead = translate(ccLineTemp);
					//判断读到的是不是计算条件
					if (ccLineRead.countWords() == 2) {
						//读完计算条件
						calculationConditionMap.insert(getCalculationCondition<int>(ccLineRead));
					}
				} while (ccLineRead[0] != "end_calculationcondition");
				//读完所有计算条件后，对当前MonteCarlo对象初始化
				this->cellNumber = calculationConditionMap["cellnumber"];
				this->groupNumber = calculationConditionMap["groupnumber"];
				this->materialNumber = calculationConditionMap["materialnumber"];
				this->repetitiveNumber = calculationConditionMap["repetitivenumber"];

				//初始化MonteCarlo中的material
				this->inputMaterial.resize(materialNumber + 1);
				for (int i = 0; i <= materialNumber; i++) {
					inputMaterial[i].nuSigmaF.resize(groupNumber + 1);
					inputMaterial[i].sigmaT.resize(groupNumber + 1);
					inputMaterial[i].sigmaS.resize(groupNumber + 1);
					inputMaterial[i].yield.resize(groupNumber + 1);
					//sigmaS和yield是二维向量，groupNumber*groupNumber
					for (int j = 0; j <= groupNumber; j++) {
						inputMaterial[i].sigmaS[j].resize(groupNumber + 1);
						inputMaterial[i].yield[j].resize(groupNumber + 1);
					}
				}

				//初始化MonteCarlo中的geometry和cell
				this->inputcell.resize(cellNumber + 1);

				isReadSection = true;
				//跳出swaitch，回到循环接着找开始标记
				break;
			}
			//读取材料
			case MT: {
				for (int i = 1; i <= this->materialNumber; i++) {
					//读取材料信息并转为iLine
					std::string mtLineTemp;
					std::getline(this->inputFile, mtLineTemp);

					iLine mtLine = translate(mtLineTemp);
					for (int j = 1; j <= this->groupNumber; j++) {
						if (!mtLine.isThere("nusigmaf")) throw("Error! E004: nuSigmaF not found!");
						if (!mtLine.isThere("sigmat")) throw("Error! E005: sigmaT not found!");
						if (!mtLine.isThere("sigmas")) throw("Error! E006: sigmaS not found!");
						if (!mtLine.isThere("yield")) throw("Error! E007: yield not found!");
						//存储读取到的nuSigmaF和sigmaT值
						(this->inputMaterial[i]).nuSigmaF[j] = s2d(mtLine[mtLine.wordNumber("nusigmaf") + j - 1]);
						(this->inputMaterial[i]).sigmaT[j] = s2d(mtLine[mtLine.wordNumber("sigmat") + j - 1]);
						//sigmaS和yield的值是二维的，需要再来一次能群循环
						double sigmaSsum = 0;
						for (int k = 1; k <= this->groupNumber; k++) {
							(this->inputMaterial[i]).sigmaS[j][k] = s2d(mtLine[mtLine.wordNumber("sigmas") + (j-1) * groupNumber + k-1]);
							sigmaSsum += this->inputMaterial[i].sigmaS[j][k];
							(this->inputMaterial[i]).yield[j][k] = s2d(mtLine[mtLine.wordNumber("yield") + (j-1) * groupNumber + k-1]);
						}
						//计算某群总散射截面
						this->inputMaterial[i].sigmaS[j][0] = sigmaSsum;
					}
				}
				std::string endMaterial;
				std::getline(this->inputFile, endMaterial);
				isReadSection = true;

				break;
			}
			//读取重复栅元信息
			case CL: {
				//保存重复栅元的起始坐标
				double repetitiveCellPosition = 0;
				for (int i = 0; i < this->repetitiveNumber; i++) {
					//读取重复栅元信息
					std::string clLineTemp;
					std::getline(this->inputFile, clLineTemp);
					iLine clLine(clLineTemp);

					//读取栅元个数，每次循环更新一次，最后的就是结果
					this->cellNumber = s2i(clLine[2]);

					for (int j = s2i(clLine[1]); j <= s2i(clLine[2]); j++) {
						//初始化MonteCarlo栅元
						this->inputcell[j].left = (j-s2i(clLine[1]))*s2d(clLine[3]) + repetitiveCellPosition;
						this->inputcell[j].right = (j+1-s2i(clLine[1]))*s2d(clLine[3]) + repetitiveCellPosition;
						this->inputcell[j].mat = this->inputMaterial[s2i(clLine[5])];
					}
					//计算下一组重复栅元的起始坐标
					repetitiveCellPosition += (s2i(clLine[2]) - s2i(clLine[1]) + 1)*s2d(clLine[3]);
	
					this->inputGeometry.setRight(repetitiveCellPosition);
				}
				this->inputGeometry.geometryCell = this->inputcell;
				std::string endCell;
				std::getline(this->inputFile, endCell);

				isReadSection = true;
				break;
			}
			//读取计算参数
			case CP: {
				std::string cpLineTemp;
				iLine cpLine;
				std::map<std::string, double> calculationParameterMap;
				do {
					//读取计算条件
					std::getline(this->inputFile, cpLineTemp);
					cpLine= translate(cpLineTemp);
					//若读到的是中子信息
					if (cpLine[0] == "neutron") {
						this->neutronNumber = s2i(cpLine[1]);
						this->totalGenerationNumber = s2i(cpLine[2]);
						this->inactiveGenerationNumber = s2i(cpLine[3]);
					}
					else if (cpLine[0]=="boundarycondition") {
						this->inputGeometry.setLeftBoundaryCondition(s2i(cpLine[1]));
						this->inputGeometry.setRightBoundaryCondition(s2i(cpLine[2]));
					}
					else if (cpLine.countWords()==2) {
						calculationParameterMap.insert(getCalculationCondition<double>(cpLine));
					}
				} while (cpLine[0] != "end_calculationparameter");
				//存储权窗大小
				this->weightMax = calculationParameterMap["weightmax"];
				this->weightMin = calculationParameterMap["weightmin"];
				isReadSection = true;

				break;
			}

			default:
				break;
			}
		}
	}
	this->inputFile.close();
}