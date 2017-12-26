#include "MonteCarlo.h"
#include "sample.h"

#include <iostream>
#include <numeric>

void MonteCarlo::transport() {
	//源迭代
	for (int iGenerationCount = 1; iGenerationCount <= this->totalGenerationNumber; iGenerationCount++) {
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
						double pathLength = samplePathLength(iGroupCount, this->inputGeometry, myNeutron);
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

		//计算有效增殖系数和通量
		if (iGenerationCount <= this->inactiveGenerationNumber) {
			//非活跃代
			currentKeff(iGenerationCount);
			currentFlux(iGenerationCount);
		}
		else {
			//活跃代
			activeKeff(iGenerationCount);
			activeFlux(iGenerationCount);
		}

		//更新中子源
		int nextGenerationNeutronNumber = 0;
		for (int i = 0; i < this->groupNumber; i++)nextGenerationNeutronNumber += this->multiGroupNextParticleSourceBank[i].size();

		int nextNeutronWeight = this->currentParticleNumber / nextGenerationNeutronNumber;
		//更新中子库
		multiGroupParticleSourceBank = multiGroupNextParticleSourceBank;
		for (int i = 0; i < this->groupNumber; i++) {
			if (multiGroupParticleSourceBank[i].size > 0) {
				for (int j = 0; j < multiGroupParticleSourceBank[i].size(); j++)multiGroupParticleSourceBank[i][j].weight = nextNeutronWeight;
			}
			//清空缓存的中子库
			this->multiGroupNextParticleSourceBank[i].clear();
		}

	} //所有代输运完毕
}
