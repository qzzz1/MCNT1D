#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

class particle {
public:
	double x;
	double direction;
	double weight;

	particle(double _x = 0, double _direction = 1, double _weight = 1) :x(_x), direction(_direction), weight(_weight) {};
	particle(const particle &_p) {
		x = _p.x;
		direction = _p.direction;
		weight = _p.weight;
	}

	virtual ~particle() {};
	//void transport();
};

#endif // !PARTICLE_H
