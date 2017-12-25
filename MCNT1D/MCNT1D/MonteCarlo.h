#pragma once
#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <vector>
#include <fstream>
#include <string>

//#include "particle.h"
#include "neutron.h"
#include "material.h"
#include "cell.h"
#include "geometry.h"

//Monte Carlo类：保存计算条件，输运计算，结果输出
class MonteCarlo {
private:
	//保存用户输入的计算条件
	int groupNumber;                                         //能群数
	int cellNumber;                                          //栅元数
	int materialNumber;                                      //材料数
	int neutronNumber;                                       //模拟中子数
	int inactiveGenerationNumber;                            //非活跃代数
	int totalGenerationNumber;                               //总代数
	geometry inputGeometry;                                  //几何
	std::vector<cell> inputcell;                             //栅元
	std::vector<material> inputMaterial;                     //材料


	//计算过程中的变量
	int currentParticleNumber;

	//template<class P>
	std::vector<std::vector<neutron>> multiGroupParticleSourceBank;          //源粒子库
	//template<class P>
	std::vector<std::vector<neutron>> multiGroupNextParticleSourceBank;      //子代粒子库
	
	std::vector<std::vector<double>> flux;                             //总通量
	std::vector<std::vector<double>> averageFlux;                      //活跃代平均通量
	double Keff;                                                       //当前代有效增殖系数
	double averageKeff;                                                //活跃代平均有效增殖系数

	std::string inputFileName;                                         //输入文件名
	std::string outputFileName;                                        //输出文件名
	std::ifstream inputFile;                                           //输入文件流
	std::ofstream outputFile;                                          //输出文件流

	double random() { return rand() / (double)RAND_MAX; }                   //生成一个0~1之间的随机数
	double randomDirection() { return 2 * rand() / (double)RAND_MAX - 1; }  //生成一个-1~1之间的随机数

	double weightMax;
	double weightMin;

	/*-------------------------------------------------------------------------
		功能：模拟一个中子到各群的裂变过程。
		参数：要裂变的中子（引用），所属能群，下一代中子库（引用）
		返回：无
		示例：fission(myNeutron, groupID, multiGroupNextNeutronSourceBank);
	-------------------------------------------------------------------------*/
	void fission(neutron &_neutron, int _group, std::vector<std::vector<neutron>> &_nextNeutronSourceBank);

	/*-------------------------------------------------------------------------
		功能：模拟一个中子到各群的散射过程。
		参数：要散射的中子（引用），所属能群，当前中子库（引用）
		返回：无
		示例：scatter(myNeutron, groupID, multiGroupNeutronSourceBank);
	-------------------------------------------------------------------------*/
	void scatter(neutron &_neutron, int _group, std::vector<std::vector<neutron>> &_currentNeutronSourceBank);

	/*---------------------------------------
		功能：判断中子库是否全空。
		参数：无
		返回：全空返回真，不空返回假
		示例：ifNeutronSourceBankEmpty();
	---------------------------------------*/
	bool ifNeutronSourceBankEmpty();

	void inactiveKeff();

	void inactiveFlux();

	void activeKeff();

	void activeFlux();

public:
	//void init();
	void readInput();
	void transport();
	void output();
};

#endif // !MONTECARLO_H
