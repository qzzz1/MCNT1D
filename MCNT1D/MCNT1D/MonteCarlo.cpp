#include "MonteCarlo.h"
#include "particle.h"
#include "neutron.h"
#include "sample.h"

#include <iostream>
#include <numeric>

/*------------------------------------------------------------------
	功能：轮盘赌。
	产生一个0~1的随机数，若大于权重的小数部分，则向上取整；否则向下取整。
	参数：权重
	返回：赌的结果
	示例: roulette(weight);
-------------------------------------------------------------------*/
int roulette(double _weight) {
	if (_weight - (int)_weight < random())return (int)_weight;
	else return (int)(_weight + 1);
}


void MonteCarlo::fission(neutron &_neutron, int _group, std::vector<std::vector<neutron>> &_nextNeutronSourceBank) {
	//计算裂变产物在各群的分布
	for (int iFissionProductGroup = 1; iFissionProductGroup <= groupNumber; iFissionProductGroup++) {
		//计算裂变产物在该群的权重
		double fissionWeight = (this->inputGeometry.geometryCell[this->inputGeometry.getCellID(_neutron.x)]).mat.yield[_group][iFissionProductGroup];
		
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
	_neutron.weight = 0;
}

void MonteCarlo::scatter(neutron &_neutron, int _group, std::vector<std::vector<neutron>> &_currentNeutronSourceBank) {
	//散射到所有群
	for (int iScatterGroup = 1; iScatterGroup <= groupNumber; iScatterGroup++) {
		//散射到iScatterGroup群的权重
		double scatterWeight = _neutron.weight*(this->inputGeometry.geometryCell[this->inputGeometry.getCellID(_neutron.x)]).mat.sigmaS[_group][iScatterGroup];

		if (scatterWeight < weightMin && scatterWeight > 0) {
			//权重过小的轮盘赌决定生死
			if (!roulette(0.1)) {
				//赌赢的增权10倍
				scatterWeight *= 10;
			}
			else {
				//赌输的被杀死
				scatterWeight = 0;
			}
		}

		if (scatterWeight > 0) {
			//抽样产生散射中子
			neutron scatterNeutron(iScatterGroup, _neutron.x, randomDirection(), scatterWeight);

			//存入当前中子库
			multiGroupParticleSourceBank[iScatterGroup].push_back(scatterNeutron);
		}
		
		//杀死散射源中子
		_neutron.weight = 0;
	}
}

bool MonteCarlo::ifNeutronSourceBankEmpty() {
	for (int i = 0; i < this->groupNumber; i++) {
		//中子库某一群不空即返回假
		if (!this->multiGroupParticleSourceBank[i].empty()) return false;
	}
	//全空返回真
	return true;
}

void MonteCarlo::inactiveKeff() {
	int nextParticleNumber = 0;
	double currentKeff = 0;
	//累加各群下一代中子数
	for (int i = 1; i <= this->groupNumber; i++)nextParticleNumber += multiGroupNextParticleSourceBank[i].size();
	//Keff的定义
	currentKeff = nextParticleNumber / this->currentParticleNumber;
}

void MonteCarlo::inactiveFlux() {
	//通量数据存储于MonteCarlo::flux中，第一维是能群号，第二维是栅元号
}

void MonteCarlo::activeKeff() {

}

void MonteCarlo::transport() {
	//源迭代
	for (int iGenerationCount = 0; iGenerationCount < this->totalGenerationNumber; iGenerationCount++) {
		//分群输运
		while (!ifNeutronSourceBankEmpty()) {
			for (int iGroupCount = 1; iGroupCount <= this->groupNumber; iGroupCount++) {
				while (!this->multiGroupParticleSourceBank[iGroupCount].empty()) {
					neutron myNeutron;
					//从粒子源中抽取一个粒子
					myNeutron = sampleFromParticleSourceBank(this->multiGroupParticleSourceBank[iGroupCount]);

					//权重过大的中子进行轮盘赌，分为数个权重为1的中子
					if (myNeutron.weight > weightMax) {
						int iRouletteResult = roulette(myNeutron.weight);
						myNeutron.weight = 0;   //轮盘赌之后杀死原中子
						//根据轮盘赌结果分裂
						for (int i = 0; i < iRouletteResult; i++) {
							neutron newNeutron = neutron(iGroupCount, myNeutron.x, myNeutron.direction, 1);
							multiGroupParticleSourceBank[iGroupCount].push_back(newNeutron);
						}

						continue;              //直接重新抽样
					}

					//权重不为0的中子进行输运模拟
					while (myNeutron.weight > 0) {
						//对径迹长度抽样
						double pathLength = samplePathLength(this->inputGeometry, myNeutron);
						//判断是否穿过栅元边界
						if (this->inputGeometry.ifCrossCellBoundary(myNeutron, pathLength)) {
							//统计通量
							if (myNeutron.direction > 0) {
								double distanceToBoundary = (this->inputGeometry.geometryCell[this->inputGeometry.getCellID(myNeutron.x)]).right - myNeutron.x;
								this->flux[iGroupCount][this->inputGeometry.getCellID(myNeutron.x)] += std::fabs(distanceToBoundary / myNeutron.direction*myNeutron.weight);
							}
							else if (myNeutron.direction < 0) {
								double distanceToBoundary = myNeutron.x - (this->inputGeometry.geometryCell[this->inputGeometry.getCellID(myNeutron.x)]).left;
								this->flux[iGroupCount][this->inputGeometry.getCellID(myNeutron.x)] += std::fabs(distanceToBoundary / myNeutron.direction*myNeutron.weight);
							}
							else {
								std::cout << "Warning! W:Neutron direction = 0!" << std::endl;
							}

							//将飞过栅元边界的中子放在相邻栅元边界处，将飞出几何边界的中子杀死
							this->inputGeometry.setCrossBoundaryPosition(myNeutron, pathLength);

							//将中子放到下一栅元的边界处之后继续对径迹长度抽样，ray-tracking?
							continue;
						}
						else {
							//统计通量
							this->flux[iGroupCount][this->inputGeometry.getCellID(myNeutron.x)] += pathLength * myNeutron.weight;

							//将中子放到碰撞的位置
							myNeutron.x += pathLength * myNeutron.direction;
						} //通量统计完毕，碰撞点确定

						//轮盘赌判断粒子是裂变还是散射
						//计算当地总散射截面
						double nativeSigmaS = std::accumulate(((this->inputGeometry.getMaterial(myNeutron.x)).sigmaS[iGroupCount]).begin(), ((this->inputGeometry.getMaterial(myNeutron.x)).sigmaS[iGroupCount]).end(), 0);
						//获取当地总截面
						double nativeSigmaT = (this->inputGeometry.getMaterial(myNeutron.x)).sigmaT[iGroupCount];
						if (roulette(nativeSigmaS / nativeSigmaT)) { //拿散射的权重赌，赢则返回0
							//返回1说明轮盘赌输掉，进行裂变
							fission(myNeutron, iGroupCount, multiGroupNextParticleSourceBank);
						}
						else {
							//否则散射
							scatter(myNeutron, iGroupCount, multiGroupParticleSourceBank);
						}
					} //权重不为0的中子输运完毕
				} //当前群输运完毕
			} //所有群输运完毕
		} //向上散射后所有群输运完毕

		std::cout << "The " << iGenerationCount << "th generation neutron simulation was finished!" << std::endl;

		
	} //所有代输运完毕
}