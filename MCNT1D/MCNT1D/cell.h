#pragma once
#ifndef CELL_H
#define CELL_H

#include "material.h"

class cell {
public:
	double left;    //栅元左边界
	double right;   //栅元右边界
	material mat;   //栅元材料

	//使用默认的构造函数
	cell() = default;

	//构造函数
	cell(double _left, double _right, material _mat) :left(_left), right(_right), mat(_mat) {}

	//复制构造函数
	cell(const cell &_c) {
		left = _c.left;
		right = _c.right;
		mat = _c.mat;
	}
};

#endif // !CELL_H
