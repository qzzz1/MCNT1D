#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

class particle {
public:
	//int id;
	double energy;
	double x;
	double direction;
	double weight;

	particle(double _energy = 0, double _x = 0, double _direction = 1, double _weight = 1) :energy(_energy), x(_x), direction(_direction), weight(_weight) {};
	particle(const particle &_p) {
		energy = _p.energy;
		x = _p.x;
		direction = _p.direction;
		weight = _p.weight;
	}
};

#endif // !PARTICLE_H
