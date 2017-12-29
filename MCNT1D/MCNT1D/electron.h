#pragma once
#ifndef ELECTRON_H
#define ELECTRON_H

#include "particle.h"

class electron : virtual public particle {
private:
	double energy;            //能量
public:
	//构造函数
	electron(double _energy, double _x, double _direction, double _weight) {
		this->energy = _energy;
		this->x = _x;
		this->direction = _direction;
		this->weight = _weight;
	}
	//使用默认的构造函数
	electron() = default;
};

#endif // !ELECTRON_H
