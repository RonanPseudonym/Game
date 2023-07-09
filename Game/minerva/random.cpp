#include "include.h"

int Minerva::Random::Range(int min, int max) {
	return (rand() % (max + 1 - min)) + min;
}

int Minerva::Random::Range(int max) {
	return Range(0, max);
}

double Minerva::Random::Range(double min, double max) {
	return ((double)rand() / ((double)RAND_MAX / (max - min))) + min;
}

double Minerva::Random::Range(double max) {
	return Range(0.0, max);
}

int Minerva::Random::SplitRange(int max) {
	return Minerva::Random::Range(-max, max);
}

double Minerva::Random::SplitRange(double max) {
	return Minerva::Random::Range(-max, max);
}