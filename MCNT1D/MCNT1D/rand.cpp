#include <random>

#include "rand.h"

double random() {
	std::random_device rd;
	double randTemp = rd() / (double)rd.max();
	while (randTemp == 0) {
		randTemp = rd() / (double)rd.max();
	}
	return randTemp;
}

double randomDirection() {
	return random() * 2 - 1;
}