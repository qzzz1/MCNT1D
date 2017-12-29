#pragma once
#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <vector>
#include <fstream>
#include <string>

#include "neutron.h"
#include "material.h"
#include "cell.h"
#include "geometry.h"
#include "input.h"

//Monte Carlo类：保存计算条件，输运计算，结果输出
class MonteCarlo {
private:
	//保存用户输入的计算条件
	int groupNumber;                                         //能群数
	int cellNumber;                                          //栅元数
	int materialNumber;                                      //材料数
	int repetitiveNumber;                                    //重复栅元组数
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
	
	std::vector<std::vector<double>> flux;                             //总通量，第一维是能群号，第二维是栅元号
	std::vector<std::vector<double>> averageFlux;                      //活跃代平均通量
	std::vector<std::vector<double>> accumulateFlux;                   //活跃代通量累积
	std::vector<std::vector<double>> accumulateFluxSquare;             //通量平方累加，用于计算方差
	std::vector<std::vector<double>> fluxStandardDeviation;            //各通量方差

	std::vector<double> Keff;                                          //各代有效增殖系数
	std::vector<double> averageKeff;                                   //各代平均有效增殖系数，非活跃代记为0
	double accumulateKeffSquare;                                       //有效增殖系数平方累加，用于计算方差
	std::vector<double> KeffStandardDeviation;                         //各代Keff统计误差

	std::string inputFileName;                                         //输入文件名
	std::string outputFileName;                                        //输出文件名
	std::ifstream inputFile;                                           //输入文件流
	std::ofstream outputFile;                                          //输出文件流

	//double random() { return rand() / (double)RAND_MAX; }                   //生成一个0~1之间的随机数
	//double randomDirection() { return 2 * rand() / (double)RAND_MAX - 1; }  //生成一个-1~1之间的随机数

	double weightMax;
	double weightMin;

	/*-------------------------------------------------------------------------
		功能：模拟一个中子到各群的裂变过程。
		参数：要裂变的中子（引用），所属能群，下一代中子库（引用）
		返回：无
		示例：fission(myNeutron, groupID, multiGroupNextNeutronSourceBank);
	-------------------------------------------------------------------------*/
	void fission(neutron &_neutron, int _group, double _fissionWeight, std::vector<std::vector<neutron>> &_nextNeutronSourceBank);

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

	/*----------------------------------------------
		功能：计算当前代有效增殖系数并存储进Keff向量。
		参数：源迭代代数编号
		返回：无
		示例：currentKeff(interationNumber);
	----------------------------------------------*/
	void currentKeff(int _iterationNumber);

	/*--------------------------------------------------
		功能：计算当前代各能群各栅元通量并存储到二维向量中。
		参数：源迭代代数编号
		返回：无
		示例：currentFlux(interationNumber);
	--------------------------------------------------*/
	void currentFlux(int _iterationNumber);

	/*----------------------------------------------
		功能：计算活跃代有效增殖系数并存储进Keff向量。
		参数：源迭代代数编号
		返回：无
		示例：currentKeff(interationNumber);
	----------------------------------------------*/
	void activeKeff(int _iterationNumber);

	/*---------------------------------------------------
		功能：计算活跃代各能群各栅元通量并存储到二维向量中。
		参数：源迭代代数编号
		返回：无
		示例：currentKeff(interationNumber);
	---------------------------------------------------*/
	void activeFlux(int _iterationNumber);

	/*------------------------------------------------------------------
	功能：轮盘赌。
	产生一个0~1的随机数，若大于权重的小数部分，则向上取整；否则向下取整。
	参数：权重
	返回：赌的结果
	示例: roulette(weight);
	-------------------------------------------------------------------*/
	int roulette(double _weight);

public:
	/*-------------------------------------------------
		功能：构造函数。
		参数：输入文件文件名，输出文件文件名
		返回：MonteCarlo对象
		示例：MonteCarlo mcnt("file.in", "file.out");
	--------------------------------------------------*/
	MonteCarlo(const std::string _inFileName, const std::string _outFileName = ".out");

	/*-----------------------
		功能：蒙卡模拟运行。
		参数：无
		返回：无
		示例：mcnt.run();
	-----------------------*/
	void run();

	/*--------------------------------------------------
		功能：读取计算条件。友元函数，定义在input.cpp中。
		参数：MonteCarlo对象（引用），读取到的文本
		返回：无
		示例：getCalculationCondition(mcnt, lineRead);
	---------------------------------------------------*/
	friend void getCalculationCondition(MonteCarlo &_mc, iLine _line) throw(mcException);

	/*-------------------------------
		功能：初始化。为变量分配内存等。
		参数：无
		返回：无
		示例：mcnt.init();
	--------------------------------*/
	void init();

	/*------------------------------
		功能：读入输入文件。
		参数：无
		返回：无
		示例：mcnt.readInput();
	------------------------------*/
	void readInput();

	/*-----------------------------
		功能：蒙卡粒子输运过程。
		参数：无
		返回：无
		示例：mcnt.transport();
	-----------------------------*/
	void transport();

	/*-----------------------------
		功能：输出蒙卡输运计算结果。
		参数：无
		返回：无
		示例：mcnt.output();
	-----------------------------*/
	void output();
};

#endif // !MONTECARLO_H
