#include "MonteCarlo.h"
#include "rand.h"

void MonteCarlo::init() {
	//初始化中子源
	this->currentParticleNumber = this->neutronNumber;
	this->multiGroupParticleSourceBank.resize(groupNumber + 1);
	this->multiGroupNextParticleSourceBank.resize(groupNumber + 1);
	int particleNumberForEachGroup = neutronNumber / groupNumber;
	for (int i = 1; i <= groupNumber; i++) {
		for (int j = 0; j < particleNumberForEachGroup; j++) {
			//随机产生一个中子
			//在几何边界内随机产生位置
			double __randPosition = random()*this->inputGeometry.geometryCell[this->cellNumber].right;
			neutron __initNeutronTemp(i, __randPosition, randomDirection(), 1.0);
			multiGroupParticleSourceBank[i].push_back(__initNeutronTemp);
		}
		//当前群源中子产生完毕
	}
	//零头全部放在第1群里
	for (int i = 0; i < neutronNumber - particleNumberForEachGroup * groupNumber; i++) {
			double __randPosition = random()*this->inputGeometry.geometryCell[this->cellNumber].right;
			neutron __initNeutronTemp(1, __randPosition, randomDirection(), 1.0);
			multiGroupParticleSourceBank[1].push_back(__initNeutronTemp);
	}
	//源中子全部产生完毕

	//为第一维分配空间，第一维表示群号，从1开始，第0个元素不用
	this->flux.resize((groupNumber+1));
	this->averageFlux.resize(groupNumber+1);
	this->accumulateFlux.resize(groupNumber+1);
	this->accumulateFluxSquare.resize(groupNumber+1);
	this->fluxStandardDeviation.resize(groupNumber+1);
	//为第二维分配空间，第二维表示栅元号，从1开始，第0个元素不用
	for (int i = 0; i <= groupNumber; i++) {
		this->flux[i].resize((cellNumber + 1));
		this->averageFlux[i].resize(cellNumber + 1);
		this->accumulateFlux[i].resize(cellNumber + 1);
		this->accumulateFluxSquare[i].resize(cellNumber + 1);
		this->fluxStandardDeviation[i].resize(cellNumber + 1);
	}

	//代号从1开始，第0个元素不用
	this->Keff.resize(totalGenerationNumber + 1);
	this->averageKeff.resize(totalGenerationNumber + 1);
	this->accumulateKeffSquare = 0;
	this->KeffStandardDeviation.resize(totalGenerationNumber + 1);

}