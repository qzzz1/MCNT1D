#pragma once
#ifndef SAMPLE_H
#define SAMPLE_H

#include <cmath>

#include "cell.h"
#include "neutron.h"
#include "rand.h"
#include "geometry.h"

/*---------------------------------------------------------------------------------------------------
	功能：从多群粒子源中的某一群中抽取一个粒子。
	参数：某群粒子库
	返回：抽样得到的一个粒子
	示例： sampleFromParticleSourceBank(multiGroupParticleSourceBank[groupNumber]);
---------------------------------------------------------------------------------------------------*/
template <class P>
P sampleFromParticleSourceBank(std::vector<P> &_sourceBank) {
	P sampledParticle = _sourceBank.back();
	_sourceBank.pop_back();
	return sampledParticle;
}

/*----------------------------------------------------------
	根据能群和栅元抽取径迹长度。
	参数：能群号，栅元号
	返回：径迹长度
	eg: samplePathLength(geometry,neutron);
----------------------------------------------------------*/
double samplePathLength(int _group, geometry _geometry, neutron _neutron);

#endif // !SAMPLE_H
