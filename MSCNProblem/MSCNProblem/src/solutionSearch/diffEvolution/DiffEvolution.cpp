#include "../../../include/solutionSearch/diffEvolution/DiffEvolution.h"

const int DiffEvolution::STARTED_INDIVIDUALS_QTY = 2000;

DiffEvolution::DiffEvolution(MscnProblem* mscnProblem, int stopCounterValue) {
	this->mscnProblem = mscnProblem;
	population = new Population(mscnProblem);
	callCounter = 0;
	this->stopCounterValue = stopCounterValue;
}

double* DiffEvolution::findBestSolution() {
	initPopulation();
	while (!shouldStopSearching()) {
		population->tryToFitnessEveryIndividual();
		callCounter++;
	}
	return population->getBestIndividualFromPopulation()->getSolution();
}

void DiffEvolution::initPopulation() {
	population->initIndividuals(mscnProblem, STARTED_INDIVIDUALS_QTY);
	callCounter = 0;
}

bool DiffEvolution::shouldStopSearching() const {
	return callCounter >= stopCounterValue;
}
