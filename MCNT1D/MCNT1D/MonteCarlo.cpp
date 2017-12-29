#include "MonteCarlo.h"
#include "particle.h"
#include "neutron.h"
#include "sample.h"

#include <iostream>
#include <numeric>                      

void MonteCarlo::run() {
	this->readInput();
	this->init();
	this->transport();
	this->output();
}

MonteCarlo::MonteCarlo(const std::string _inFileName, const std::string _outFileName) {
	this->inputFileName = _inFileName;
	this->outputFileName = _outFileName;
	this->inputFile.open(_inFileName);
	//检查输入文件是否存在
	if (!inputFile) {
		std::cout << "Error! E001: Can't open file \"" << _inFileName << "\"!" << std::endl;
		return;
	}
	this->outputFile.open(_outFileName);
	//检查输出文件是否能打开
	if (!outputFile) {
		std::cout << "Error! E001: Can't open file \"" << _outFileName << "\"!" << std::endl;
		return;
	}
}

int MonteCarlo::roulette(double _weight) {
	if (_weight - (int)_weight < random())return (int)_weight;
	else return (int)(_weight + 1);
}

void MonteCarlo::fission(neutron &_neutron, int _group,double _fissionWeight, std::vector<std::vector<neutron>> &_nextNeutronSourceBank) {
	//计算裂变产物在各群的分布
	for (int iFissionProductGroup = 1; iFissionProductGroup <= groupNumber; iFissionProductGroup++) {
		//计算裂变产物在该群的权重
		double fissionWeight = (this->inputGeometry.geometryCell[this->inputGeometry.getCellID(_neutron.x)]).mat.yield[_group][iFissionProductGroup] * _neutron.weight*_fissionWeight;
		
		//轮盘赌确定裂变产生的中子数
		int fissionNeutronNumber = roulette(fissionWeight);

		//将裂变产生的中子存入下一代中子库
		for (int i = 0; i < fissionNeutronNumber; i++) {
			//抽样产生一个裂变产物，群号为当前循环的裂变群，方向随机抽样，权重为1
			neutron fissionNeutronProduct(_group, _neutron.x, randomDirection(), 1);

			//将裂变产生的中子存入下一代中子库
			_nextNeutronSourceBank[iFissionProductGroup].push_back(fissionNeutronProduct);
		}
	}

	//杀死裂变源中子
	//_neutron.weight = 0;
}

void MonteCarlo::scatter(neutron &_neutron, int _group, std::vector<std::vector<neutron>> &_currentNeutronSourceBank) {
	//散射到所有群
	for (int iScatterGroup = 1; iScatterGroup <= groupNumber; iScatterGroup++) {
		//散射到iScatterGroup群的权重
		double scatterWeight = _neutron.weight*(this->inputGeometry.geometryCell[this->inputGeometry.getCellID(_neutron.x)]).mat.sigmaS[_group][iScatterGroup]/ (this->inputGeometry.geometryCell[this->inputGeometry.getCellID(_neutron.x)]).mat.sigmaT[_group];

		if (scatterWeight > 0) {
			//抽样产生散射中子
			neutron scatterNeutron(iScatterGroup, _neutron.x, randomDirection(), scatterWeight);
			//存入当前中子库
			multiGroupParticleSourceBank[iScatterGroup].push_back(scatterNeutron);
		}

		if (scatterWeight < weightMin && scatterWeight > 0) {
			//权重过小的轮盘赌决定生死
			if (roulette(0.1)) {
				//赌赢的增权10倍
				scatterWeight *= 10;
			}
			else {
				//赌输的被杀死
				scatterWeight = 0;
			}
		}
	}
	//杀死散射源中子
	_neutron.weight = 0;
}

bool MonteCarlo::ifNeutronSourceBankEmpty() {
	for (int i = 1; i <= this->groupNumber; i++) {
		//中子库某一群不空即返回假
		if (!this->multiGroupParticleSourceBank[i].empty()) return false;
	}
	//全空返回真
	return true;
}

void MonteCarlo::currentKeff(int _iterationNumber) {
	int nextParticleNumber = 0;
	
	//累加各群下一代中子数
	for (int i = 1; i <= this->groupNumber; i++)nextParticleNumber += multiGroupNextParticleSourceBank[i].size();
	//Keff的计算:非活跃代只计算Keff，Keff平均值和统计误差均记为0，活跃代调用此函数时平均值和统计误差会重新更新
	this->Keff[_iterationNumber] = (nextParticleNumber+0.0) / this->currentParticleNumber;
	this->averageKeff[_iterationNumber] = 0;
	this->KeffStandardDeviation[_iterationNumber] = 0;
}

void MonteCarlo::currentFlux(int _iterationNumber) {
	//通量数据存储于MonteCarlo::flux中，第一维是能群号，第二维是栅元号
}

void MonteCarlo::activeKeff(int _iterationNumber) {
	//计算当前代Keff并存储
	currentKeff(_iterationNumber);

	double accmKeff = std::accumulate(Keff.begin() + inactiveGenerationNumber, Keff.begin() + _iterationNumber, 0.0);
	averageKeff[_iterationNumber] = accmKeff / (_iterationNumber - this->inactiveGenerationNumber);

	//计算Keff的统计误差
	double standardDeviation = 0;
	if (_iterationNumber > this->inactiveGenerationNumber + 1) {
		standardDeviation = sqrt((this->accumulateKeffSquare / (_iterationNumber - this->inactiveGenerationNumber) - (this->averageKeff[_iterationNumber - 1])*(this->averageKeff[_iterationNumber - 1])) / (_iterationNumber - this->inactiveGenerationNumber - 1)) / this->averageKeff[_iterationNumber - 1];
	}
	//计算Keff统计误差并存储
	this->KeffStandardDeviation[_iterationNumber] = standardDeviation;
}

void MonteCarlo::activeFlux(int _iterationNumber) {
	for (int iGroup = 1; iGroup <= this->groupNumber; iGroup++) {
		for (int iCell = 1; iCell <= this->cellNumber; iCell++) {
			this->accumulateFlux[iGroup][iCell] += this->flux[iGroup][iCell];
			this->accumulateFluxSquare[iGroup][iCell] += (this->flux[iGroup][iCell])*(this->flux[iGroup][iCell]);
			this->averageFlux[iGroup][iCell] = this->accumulateFlux[iGroup][iCell] / (_iterationNumber - this->inactiveGenerationNumber);
			//第一个活跃代没有统计误差
			if(_iterationNumber!=this->inactiveGenerationNumber + 1) this->fluxStandardDeviation[iGroup][iCell] = sqrt(((this->accumulateFluxSquare[iGroup][iCell] / (_iterationNumber - this->inactiveGenerationNumber) - (this->averageFlux[iGroup][iCell])*(this->averageFlux[iGroup][iCell])) / (_iterationNumber - this->inactiveGenerationNumber - 1))) / this->averageFlux[iGroup][iCell];
			else this->fluxStandardDeviation[iGroup][iCell] = 0;
		}
	}
}
