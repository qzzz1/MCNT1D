#pragma once
#ifndef NEUTRON_H
#define NEUTRON_H

#include "particle.h"

class neutron : virtual public particle {
private:
	int group;            //ÈººÅ
public:

	neutron(int _group, double _x, double _direction, double _weight) {		// :group(_group), x(_x), direction(_direction), weight(_weight) {}
		this->group = _group;
		this->x = _x;
		this->direction = _direction;
		this->weight = _weight;
	}
	neutron() = default;
};

#endif // !NEUTRON_H
