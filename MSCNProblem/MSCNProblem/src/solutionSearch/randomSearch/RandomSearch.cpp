#include "../../../include/solutionSearch/randomSearch/RandomSearch.h"

RandomSearch::RandomSearch(MscnProblem* mscnProblem, unsigned long attemptQty) {
	this->mscnProblem = mscnProblem;
	this->attemptQty = attemptQty;
}

double* RandomSearch::findSolution() {
	unsigned long actualQty = 0;
	double maxProfit = -DBL_MAX;
	MscnProblem::errorCodes solutionErrorCode;

	double* bestSolution = nullptr;
	double* actualSolution = nullptr;
	while (!finishedSearchingForSolution(actualQty)) {
		if (actualSolution != bestSolution)
			delete actualSolution;
		actualSolution = getRandomSolution();
		const int profit = mscnProblem->getQuality(actualSolution, mscnProblem->getSolutionSize(), solutionErrorCode);
		if (profit > maxProfit) {
			delete bestSolution;
			bestSolution = actualSolution;
			maxProfit = profit;
		}
		actualQty++;
	}
	return bestSolution;
}

double* RandomSearch::getRandomSolution() {
	int D = mscnProblem->getDQty();
	int F = mscnProblem->getFQty();
	int M = mscnProblem->getMQty();
	int S = mscnProblem->getSQty();
	double* solution = new double[mscnProblem->getSolutionSize()];
	int nextIndex = 0;
	//xd
	for (int i = 0; i < D; i++)
		for (int j = 0; j < F; j++) {
			solution[nextIndex] =
				random.getRandom(mscnProblem->getXdMin()->get(i, j), mscnProblem->getXdMax()->get(i, j));
			nextIndex++;
		}

	//xf
	for (int i = 0; i < F; i++)
		for (int j = 0; j < M; j++) {
			solution[nextIndex] =
				random.getRandom(mscnProblem->getXfMin()->get(i, j), mscnProblem->getXfMax()->get(i, j));
			nextIndex++;
		}

	//xm
	for (int i = 0; i < M; i++)
		for (int j = 0; j < S; j++) {
			solution[nextIndex] =
				random.getRandom(mscnProblem->getXmMin()->get(i, j), mscnProblem->getXmMax()->get(i, j));
			nextIndex++;
		}

	return solution;
}

bool RandomSearch::finishedSearchingForSolution(unsigned long actualAttempt) const {
	return actualAttempt >= attemptQty;
}

void RandomSearch::setMscnProblem(MscnProblem* mscnProblem) {
	this->mscnProblem = mscnProblem;
}

void RandomSearch::setAttemptQty(unsigned long attemptQty) {
	this->attemptQty = attemptQty;
}
