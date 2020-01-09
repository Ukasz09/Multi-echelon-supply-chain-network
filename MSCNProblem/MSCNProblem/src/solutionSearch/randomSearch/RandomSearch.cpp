#include "../../../include/solutionSearch/randomSearch/RandomSearch.h"

RandomSearch::RandomSearch(MscnProblem* mscnProblem, unsigned long attemptQty) {
	this->mscnProblem = mscnProblem;
	this->attemptQty = attemptQty;
}

double* RandomSearch::findBestSolution() {
	unsigned long actualQty = 0;
	double maxProfit = -DBL_MAX;
	MscnProblem::errorCodes solutionErrorCode;

	double* bestSolution = nullptr;
	double* actualSolution = nullptr;
	while (!finishedSearchingForSolution(actualQty)) {
		if (actualSolution != bestSolution)
			delete actualSolution;
		actualSolution = getRandomSolution();
		const int profit = mscnProblem->getProfit(actualSolution, mscnProblem->getSolutionSize(), solutionErrorCode);
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
	int D = mscnProblem->getSuppliersQty();
	int F = mscnProblem->getFactoriesQty();
	int M = mscnProblem->getStoreQty();
	int S = mscnProblem->getShopsQty();
	double* solution = new double[mscnProblem->getSolutionSize()];
	int nextIndex = 0;
	//xd
	for (int i = 0; i < D; i++)
		for (int j = 0; j < F; j++) {
			solution[nextIndex] =
				random.getRandom(mscnProblem->getQtyOfFeedstockDeliverBySuppliersMin()->get(i, j), mscnProblem->getQtyOfFeedstockDeliverBySuppliersMax()->get(i, j));
			nextIndex++;
		}

	//xf
	for (int i = 0; i < F; i++)
		for (int j = 0; j < M; j++) {
			solution[nextIndex] =
				random.getRandom(mscnProblem->getQtyOfProductDeliverByFactoriesMin()->get(i, j), mscnProblem->getQtyOfProductDeliverByFactoriesMax()->get(i, j));
			nextIndex++;
		}

	//xm
	for (int i = 0; i < M; i++)
		for (int j = 0; j < S; j++) {
			solution[nextIndex] =
				random.getRandom(mscnProblem->getQtyOfProductDeliverByStoreMin()->get(i, j), mscnProblem->getQtyOfProductDeliverByStoreMax()->get(i, j));
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
