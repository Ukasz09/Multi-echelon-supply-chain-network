#include "../../../include/solutionSearch/diffEvolution/Population.h"
#include "../../../include/solutionSearch/randomSearch/RandomSearch.h"
#include "../../../include/utilities/RandomGenerator.h"

const float Population::CROSS_PROB = 0.9;
const float Population::DIFF_WEIGHT = 0.8;

Population::Population(MscnProblem* mscnProblem) {
	this->mscnProblem = mscnProblem;
}

void Population::initIndividuals(MscnProblem* mscnProblem, int individualsQty) {
	RandomSearch radomSearch(mscnProblem, 1);
	for (int i = 0; i < individualsQty; i++) {
		double* randSolution = radomSearch.findSolution();
		individuals.push_back(new Individual(randSolution, mscnProblem->getSolutionSize()));
	}
}

void Population::tryToFitnessEveryIndividual() {
	RandomGenerator<int> randomInt;
	for (int i = 0; i < individuals.size(); i++) {
		Individual* baseInd = individuals[randomInt.getRandom(0, individuals.size() - 1)];
		Individual* addInd0 = individuals[randomInt.getRandom(0, individuals.size() - 1)];
		Individual* addInd1 = individuals[randomInt.getRandom(0, individuals.size() - 1)];

		if (individualsAreDifferent(individuals[i], baseInd, addInd0, addInd1)) {
			Individual* indNew = makeNewIndividual(individuals[i]->getSolutionSize(), individuals[i], baseInd, addInd0,
			                                       addInd1);
			if (fitnesse(indNew) > fitnesse(individuals[i])) {
				delete individuals[i];
				individuals[i] = indNew;
			}
			else delete indNew;
		}
	}
}

bool Population::individualsAreDifferent(Individual* ind1, Individual* ind2, Individual* ind3, Individual* ind4) {
	if (ind1 == ind2 || ind1 == ind3 || ind1 == ind4)
		return false;
	if (ind2 == ind3 || ind2 == ind4)
		return false;
	if (ind3 == ind4)
		return false;
	return true;
}

Individual* Population::makeNewIndividual(int solutionSize, Individual* fitnessedInd, Individual* baseInd,
                                          Individual* addInd0, Individual* addInd1) {
	double* indNewSolution = new double[solutionSize];
	Individual* indNew = new Individual(indNewSolution, solutionSize);

	for (int geneOffset = 0; geneOffset < solutionSize; geneOffset++) {
		const float randomReal = random.getRandom(0, 1);
		if (randomReal < CROSS_PROB) {
			const double oldGene = fitnessedInd->getSolution()[geneOffset];
			const double baseIndGene = baseInd->getSolution()[geneOffset];
			const double addInd0Gene = addInd0->getSolution()[geneOffset];
			const double addInd1Gene = addInd1->getSolution()[geneOffset];
			const double newGeneValue = getNewGeneValue(oldGene, geneOffset, baseIndGene, DIFF_WEIGHT, addInd0Gene,
			                                            addInd1Gene);
			indNew->getSolution()[geneOffset] = newGeneValue;
		}
		else indNew->getSolution()[geneOffset] = fitnessedInd->getSolution()[geneOffset];
	}
	return indNew;
}

double Population::getNewGeneValue(double oldGeneVal, int geneOffset, double baseIndGeneVal, double diffWeight,
                                   double addInd0GeneVal, double addInd1GeneVal) {
	double newGeneVal = baseIndGeneVal + diffWeight * (addInd0GeneVal - addInd1GeneVal);
	return (newGeneVal > mscnProblem->getMaxX(geneOffset)) ? oldGeneVal : newGeneVal;
}

double Population::fitnesse(Individual* ind) const {
	MscnProblem::errorCodes errCode;
	return mscnProblem->getQuality(ind->getSolution(), ind->getSolutionSize(), errCode);
}

Individual* Population::getBestIndividualFromPopulation() {
	double bestProfit = fitnesse(individuals[0]);
	int indexOfBest = 0;
	for (int i = 1; i < individuals.size(); i++) {
		const double actualProfit = fitnesse(individuals[i]);
		if (actualProfit > bestProfit) {
			bestProfit = actualProfit;
			indexOfBest = i;
		}
	}
	Individual* bestIndividual = new Individual(*individuals[indexOfBest]);
	return bestIndividual;
}
