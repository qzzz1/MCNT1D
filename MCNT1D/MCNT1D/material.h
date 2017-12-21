#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include <vector>

class material{
public:
    std::vector<double> nuSigmaF;
    std::vector<double> sigmaT;
    std::vector<std::vector<double>> sigmaS;
    std::vector<std::vector<double>> yield;

    material(std::vector<double> _nuSigmaF, std::vector<double> _sigmaT, std::vector<std::vector<double>> _sigmaS, std::vector<std::vector<double>> _yield){
        nuSigmaF=_nuSigmaF;
        sigmaT=_sigmaT;
        sigmaS=_sigmaS;
        yield=_yield;
    }
    material(material &_mat){
        nuSigmaF=_mat.nuSigmaF;
        sigmaT=_mat.sigmaT;
        sigmaS=_mat.sigmaS;
        yield=_mat.yield;
    }
};

#endif
