#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

class particle {
public:
	double x;				//位置
	double direction;       //方向
	double weight;          //权重

	//构造函数
	particle(double _x = 0, double _direction = 1, double _weight = 1) :x(_x), direction(_direction), weight(_weight) {};

	//复制构造函数
	particle(const particle &_p) {
		x = _p.x;
		direction = _p.direction;
		weight = _p.weight;
	}

	//析构函数
	virtual ~particle() {};
};

#endif // !PARTICLE_H
