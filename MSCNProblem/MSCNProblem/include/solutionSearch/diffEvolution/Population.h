#pragma once
#include <vector>
#include "Individual.h"
#include "../../utilities/RandomGenerator.h"
#include "../../MscnProblem.h"

class Population {
private:
	static const float CROSS_PROB;
	static const float DIFF_WEIGHT;

	std::vector<Individual*> individuals;
	RandomGenerator<float> random;
	MscnProblem* mscnProblem;
public:
	Population(MscnProblem* mscnProblem);
	~Population() = default;
public:
	void initIndividuals(MscnProblem* mscnProblem, int individualsQty);
	void tryToFitnessEveryIndividual();
	Individual* getBestIndividualFromPopulation();
private:
	bool individualsAreDifferent(Individual* ind1, Individual* ind2, Individual* ind3, Individual* ind4);
	double fitnesse(Individual* ind) const;
	double getNewGeneValue(double oldGeneVal, int geneOffset, double baseIndGeneVal, double diffWeight,
	                       double addInd0GeneVal, double addInd1GeneVal);
	Individual* makeNewIndividual(int solutionSize, Individual* fitnessedInd, Individual* baseInd, Individual* addInd0,
	                              Individual* addInd1);
};
