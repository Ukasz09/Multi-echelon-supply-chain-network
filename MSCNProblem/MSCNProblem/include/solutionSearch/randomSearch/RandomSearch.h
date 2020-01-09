#pragma once
#include "../../MscnProblem.h"
#include "../../utilities/RandomGenerator.h"

class RandomSearch {
private:
	MscnProblem* mscnProblem;
	unsigned long attemptQty;
	RandomGenerator<float> random;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	RandomSearch(MscnProblem* mscnProblem, unsigned long attemptQty);
	~RandomSearch() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	double* findSolution();
	void setMscnProblem(MscnProblem* mscnProblem);
	void setAttemptQty(unsigned long attemptQty);
private:
	bool finishedSearchingForSolution(unsigned long actualAttempt) const;
	double* getRandomSolution();
};
