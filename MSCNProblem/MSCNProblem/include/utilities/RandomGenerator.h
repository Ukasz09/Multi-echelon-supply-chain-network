#pragma once
#include <random>
#include <ctime>

template <class T=int>
class RandomGenerator {
private:
	unsigned int seed;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	RandomGenerator(unsigned int seed);
	RandomGenerator();
	~RandomGenerator() = default;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	T getRandom(T min, T max);
private:
	double getRandomRealNumber(double min, double max);
};

template <class T>
RandomGenerator<T>::RandomGenerator(unsigned int seed) {
	this->seed = seed;
	srand(seed);
}

template <class T>
RandomGenerator<T>::RandomGenerator() {
	this->seed = time(nullptr);
	srand(seed);
}


template <>
inline float RandomGenerator<float>::getRandom(float min, float max) {
	return getRandomRealNumber(min, max);
}

template <>
inline double RandomGenerator<double>::getRandom(double min, double max) {
	return getRandomRealNumber(min, max);
}

template <class T>
T RandomGenerator<T>::getRandom(T min, T max) {
	int range = max - min + 1;
	return (std::rand() % range) + min;
}

template <class T>
double RandomGenerator<T>::getRandomRealNumber(double min, double max) {
	min *= 100;
	max *= 100;
	int range = max - min + 1;
	return ((std::rand() % range) + min) / 100;
}
