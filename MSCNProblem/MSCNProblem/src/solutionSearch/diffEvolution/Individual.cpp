#include "../../../include/solutionSearch/diffEvolution/Individual.h"

Individual::Individual(double* solution, int solutionSize) {
	this->solution = solution;
	this->solutionSize = solutionSize;
}

Individual::Individual(const Individual& other) {
	this->solutionSize = other.solutionSize;
	this->solution = new double[solutionSize];
	for (int i = 0; i < solutionSize; i++)
		this->solution[i] = other.solution[i];
}

Individual::~Individual() {
	delete solution;
}

void Individual::setSolution(double* solution, int solutionSize) {
	this->solution = solution;
	this->solutionSize = solutionSize;
}

double* Individual::getSolution() const {
	return solution;
}

int Individual::getSolutionSize() const {
	return solutionSize;
}
