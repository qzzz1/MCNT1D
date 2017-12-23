#pragma once
#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <vector>
#include <fstream>
#include <string>

#include "particle.h"
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
	template<class P>
	std::vector<std::vector<P>> multiGroupParticleSourceBank;          //源粒子库
	template<class P>
	std::vector<std::vector<P>> multiGroupNextParticleSourceBank;      //子代粒子库
	
	std::vector<std::vector<double>> flux;                             //总通量
	std::vector<std::vector<double>> averageFlux;                      //平均通量
	double totalKeff;                                                  //总有效增殖系数

	std::string inputFileName;                                         //输入文件名
	std::string outputFileName;                                        //输出文件名
	std::ifstream inputFile;                                           //输入文件流
	std::ofstream outputFile;                                          //输出文件流
	


public:
	void init();
	void readInput();
	void transport();
	void output();
};

#endif // !MONTECARLO_H
