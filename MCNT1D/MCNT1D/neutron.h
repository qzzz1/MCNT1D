#pragma once
#ifndef NEUTRON_H
#define NEUTRON_H

#include "particle.h"

class neutron : virtual public particle {
private:
	int group;            //群号
public:
	//构造函数
	neutron(int _group, double _x, double _direction, double _weight) {		
		this->group = _group;
		this->x = _x;
		this->direction = _direction;
		this->weight = _weight;
	}
	//使用默认的构造函数
	neutron() = default;
};

#endif // !NEUTRON_H
