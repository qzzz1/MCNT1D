#include "sample.h"

double samplePathLength(int _group, geometry _geometry, neutron _neutron) {
	double __x = _neutron.x;
	double __sigmaT = (_geometry.getMaterial(_geometry.getCellID(__x))).sigmaT[_group];
	return -log(random()) / __sigmaT;
}