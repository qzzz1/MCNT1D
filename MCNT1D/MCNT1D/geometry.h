#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include "cell.h"
#include "material.h"
#include "neutron.h"

//无穷小量
#define INF_MIN 0.000000001  

//边界条件：A-吸收，R-反射， W-白边界
typedef enum { A, R, W } boundaryCondition;

class geometry {
	double left;                       //几何左边界
	double right;                      //几何右边界
	boundaryCondition leftCondition;   //左边界条件
	boundaryCondition rightCondition;  //右边界条件
	int cellNumber;                    //栅元数

public:
	std::vector<cell> geometryCell;    //包含的所有栅元

	//默认构造函数
	geometry() = default;

	//设置左边界位置
	void setLeft(double _leftPosition) { left = _leftPosition; }

	//设置右边界位置
	void setRight(double _rightPosition) { right = _rightPosition; }

	/*------------------------------------------------------
		功能：设置左边界条件。
		参数：表示边界条件的数字，吸收为0，全反射为1，白边界为2
		返回：无
		示例：setLeftBoundaryCondition(1);
	-------------------------------------------------------*/
	void setLeftBoundaryCondition(int _boundaryCondition);

	/*------------------------------------------------------
		功能：设置右边界条件。
		参数：表示边界条件的数字，吸收为0，全反射为1，白边界为2
		返回：无
		示例：setRighttBoundaryCondition(1);
	-------------------------------------------------------*/
	void setRightBoundaryCondition(int _boundaryCondition);

	//构造函数
	geometry(double _left, double _right, std::vector<cell> _geomCell) :left(_left), right(_right), geometryCell(_geomCell) {}
	
	/*------------------------------
		功能：给出指定位置所属的栅元编号。
		参数：坐标
		返回：栅元编号
		示例: getCellID(x);
	------------------------------*/
	int getCellID(double _x);

	/*------------------------------
		功能：给出指定位置所属的材料。
		参数：坐标
		返回：材料对象
		示例: getMaterial(x);
	------------------------------*/
	material getMaterial(double _x);

	/*----------------------------------------------------
		功能：判断中子是否穿过栅元边界。
		参数：中子，径迹长度
		返回：穿过为真，未穿过为假
		示例：ifCrossCellBoundary(myNeutron, myPath);
	-----------------------------------------------------*/
	bool ifCrossCellBoundary(neutron _neutron, double _pathLength);

	/*----------------------------------------------------
		功能：判断中子是否飞出几何。
		参数：中子，径迹长度
		返回：飞出为真，未飞出为假
		示例：ifBeyondGeometry(myNeutron, myPath);
	-----------------------------------------------------*/
	bool ifBeyondGeometry(neutron _neutron, double _pathLength);

	/*-----------------------------------------------------
		功能：处理穿过栅元边界或几何边界的中子。
		参数：中子（引用），径迹长度
		返回：无
		示例：setCrossBoundaryPosition(myNeutron, myPath);
	------------------------------------------------------*/
	void setCrossBoundaryPosition(neutron &_neutron, double _pathLength);

};

#endif // !GEOMETRY_H
