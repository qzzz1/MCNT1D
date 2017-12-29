#pragma once
#ifndef PHOTON_H
#define PHOTON_H

#include "particle.h"

typedef enum { X, gamma }photonType;

class photon : virtual public particle {
private:
	double energy;            //能量
	photonType type;
public:
	//构造函数
	photon(double _energy, photonType _type, double _x, double _direction, double _weight) {
		this->energy = _energy;
		this->type = _type;
		this->x = _x;
		this->direction = _direction;
		this->weight = _weight;
	}
	//使用默认的构造函数
	photon() = default;
};

#endif // !PHOTON_H
