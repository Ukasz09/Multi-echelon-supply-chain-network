#pragma once
#include "../../MscnProblem.h"
#include "Population.h"

class DiffEvolution {
private:
	static const int STARTED_INDIVIDUALS_QTY;

	MscnProblem* mscnProblem;
	Population* population;
	int callCounter;
	int stopCounterValue;
public:
	DiffEvolution(MscnProblem* mscnProblem, int stopCounterValue);
	~DiffEvolution() = default;
public:
	double* findBestSolution();
private:
	void initPopulation();
	bool shouldStopSearching() const;
};
