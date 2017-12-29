#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include <vector>

class material{
public:
    std::vector<double> nuSigmaF;                      //裂变截面，序号为能群号
    std::vector<double> sigmaT;                        //总截面，序号为能群号
    std::vector<std::vector<double>> sigmaS;           //散射截面，第一维是能群，第二维是散射产物能群
    std::vector<std::vector<double>> yield;            //产率，第一维是能群，第二维是裂变产物能群

	//默认构造函数
	material() = default;

	//构造函数
    material(std::vector<double> _nuSigmaF, std::vector<double> _sigmaT, std::vector<std::vector<double>> _sigmaS, std::vector<std::vector<double>> _yield){
        nuSigmaF=_nuSigmaF;
        sigmaT=_sigmaT;
        sigmaS=_sigmaS;
        yield=_yield;
    }

	//复制构造函数
    material(const material &_mat){
        nuSigmaF=_mat.nuSigmaF;
        sigmaT=_mat.sigmaT;
        sigmaS=_mat.sigmaS;
        yield=_mat.yield;
    }
};

#endif
