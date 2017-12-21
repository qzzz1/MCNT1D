#pragma once
#ifndef CELL_H
#define CELL_H

#include "material.h"

class cell {
public:
	double left;
	double right;
	material mat;

	cell(double _left, double _right, material _mat) :left(_left), right(_right), mat(_mat) {}
};

#endif // !CELL_H
