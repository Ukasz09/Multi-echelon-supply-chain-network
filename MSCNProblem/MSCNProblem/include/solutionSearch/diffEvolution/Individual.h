#pragma once
class Individual {
private:
	double* solution;
	int solutionSize;
public:
	Individual(double* solution, int solutionSize);
	Individual(const Individual& other);
	~Individual();
public:
	void setSolution(double* solution, int solutionSize);
	double* getSolution() const;
	int getSolutionSize() const;
};


