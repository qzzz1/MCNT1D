#include "geometry.h"
#include "rand.h"

#include <iostream>

void geometry::setLeftBoundaryCondition(int _boundaryCondition) {
	switch (_boundaryCondition)
	{
	case 0:
		leftCondition = A;
		break;
	case 1:
		leftCondition = R;
		break;
	case 2:
		leftCondition = W;
		break;
	default:
		break;
	}
}

void geometry::setRightBoundaryCondition(int _boundaryCondition) {
	switch (_boundaryCondition)
	{
	case 0:
		rightCondition = A;
		break;
	case 1:
		rightCondition = R;
		break;
	case 2:
		rightCondition = W;
		break;
	default:
		break;
	}
}

int geometry::getCellID(double _x) {
	for (int i = 0; i < this->geometryCell.size(); i++) {
		if (_x >= geometryCell[i].left&&_x < geometryCell[i].right)return i;
	}
	//如果位置_x恰好在几何右边界
	if (_x = geometryCell[this->geometryCell.size() - 1].right) return this->geometryCell.size() - 1;
}

material geometry::getMaterial(double _x) {
	return this->geometryCell[getCellID(_x)].mat;
}

bool geometry::ifCrossCellBoundary(neutron _neutron, double _pathLength) {
	double newX = _neutron.x + _neutron.direction*_pathLength;  //运动指定径迹长度后的位置
	//若超出几何边界，直接返回真
	if (newX<0 || newX>this->right) return true;
	return newX < this->geometryCell[getCellID(_neutron.x)].left || newX > this->geometryCell[getCellID(_neutron.x)].right;
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
