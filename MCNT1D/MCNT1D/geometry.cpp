#include "geometry.h"
#include "rand.h"

#include <iostream>

int geometry::getCellID(double _x) {
	for (int i = 0; i < this->geometryCell.size(); i++) {
		if (_x >= geometryCell[i].left&&_x < geometryCell[i].right)return i + 1;
	}
}

material geometry::getMaterial(double _x) {
	return this->geometryCell[getCellID(_x) - 1].mat;
}

bool geometry::ifCrossCellBoundary(neutron _neutron, double _pathLength) {
	double newX = _neutron.x + _neutron.direction*_pathLength;  //运动指定径迹长度后的位置
	return newX < this->geometryCell[getCellID(newX)].left || newX > this->geometryCell[getCellID(newX)].right;
}

bool geometry::ifBeyondGeometry(neutron _neutron, double _pathLength) {
	double newX = _neutron.x + _neutron.direction*_pathLength;  //运动指定径迹长度后的位置
	return newX < this->left || newX > this->right;
}

void geometry::setCrossBoundaryPosition(neutron &_neutron, double _pathLength) {
	//判断是否飞出几何边界
	if (ifBeyondGeometry(_neutron, _pathLength)) {
		if (_neutron.direction > 0) {
			//飞出几何右边界
			if (this->rightCondition == A) {                   //右边界为吸收
				_neutron.weight = 0;
			}
			else if (this->rightCondition == R) {              //右边界为全反射
				_neutron.x = this->right - INF_MIN;
				_neutron.direction = -_neutron.direction;
			}
			else if (this->rightCondition == W) {              //右边界为白边界
				_neutron.x = this->right - INF_MIN;
				_neutron.direction = -random();
			}
			else {
				std::cout << "Error! E:Undefined right boundary condition!" << std::endl;
			}
		}
		else if (_neutron.direction < 0) {
			//飞出几何左边界
			if (this->leftCondition == A) {                    //左边界为吸收
				_neutron.weight = 0;
			}
			else if (this->leftCondition == R) {               //左边界为全反射
				_neutron.x = this->left + INF_MIN;
				_neutron.direction = -_neutron.direction;
			}
			else if (this->leftCondition == W) {               //左边界为白边界
				_neutron.x = this->left + INF_MIN;
				_neutron.direction = random();
			}
			else {
				std::cout << "Error! E:Undefined left boundary condition!" << std::endl;
			}
		}
		else {
			std::cout << "Warning! W:Neutron direction = 0!" << std::endl;
			_neutron.weight = 0;
		}
	}
	//未飞出几何边界，即只飞出了某个栅元
	else {
		if (_neutron.direction > 0) {
			_neutron.x = (this->geometryCell[this->getCellID(_neutron.x) + 1]).left + INF_MIN;
		}
		else if (_neutron.direction < 0) {
			_neutron.x = (this->geometryCell[this->getCellID(_neutron.x) - 1]).right - INF_MIN;
		}
		else {
			std::cout << "Warning! W:Neutron direction = 0!" << std::endl;
			_neutron.weight = 0;
		}
	}
}
