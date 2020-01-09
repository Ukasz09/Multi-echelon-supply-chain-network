#pragma once
#include <vector>
#include "utilities/RandomGenerator.h"
#include "utilities/Matrix.h"

class MscnProblem {
public:
	enum errorCodes {
		nullptrSolution,
		incorrectSize,
		incorrectCost,
		noErrors
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	static const int DEFAULT_QTY_OF_D = 6;
	static const int DEFAULT_QTY_OF_F = 7;
	static const int DEFAULT_QTY_OF_M = 8;
	static const int DEFAULT_QTY_OF_S = 9;

	int dQty, fQty, mQty, sQty;
	std::vector<double> sd, sf, sm, ss;
	std::vector<double> ud, uf, um;
	std::vector<double> ps;
	Matrix<double>* cd;
	Matrix<double>* cf;
	Matrix<double>* cm;

	Matrix<double>* xd;
	Matrix<double>* xf;
	Matrix<double>* xm;

	Matrix<double>* xdMin;
	Matrix<double>* xfMin;
	Matrix<double>* xmMin;

	Matrix<double>* xdMax;
	Matrix<double>* xfMax;
	Matrix<double>* xmMax;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	MscnProblem();
	MscnProblem(double* data, int arrSize, errorCodes& err);
	~MscnProblem();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	errorCodes setDQty(int qty);
	errorCodes setFQty(int qty);
	errorCodes setMQty(int qty);
	errorCodes setSQty(int qty);

	errorCodes setInCd(double value, int i, int j);
	errorCodes setInCf(double value, int i, int j);
	errorCodes setInCm(double value, int i, int j);

	errorCodes setInSd(double value, int i);
	errorCodes setInSf(double value, int i);
	errorCodes setInSm(double value, int i);
	errorCodes setInSs(double value, int i);

	errorCodes setInPs(double value, int i);

	errorCodes setInUd(double value, int i);
	errorCodes setInUf(double value, int i);
	errorCodes setInUm(double value, int i);


	int getDQty() const;
	int getFQty() const;
	int getMQty() const;
	int getSQty() const;
	std::vector<double> getSd() const;
	std::vector<double> getSf() const;
	std::vector<double> getSm() const;
	std::vector<double> getSs() const;
	std::vector<double> getUd() const;
	std::vector<double> getUf() const;
	std::vector<double> getUm() const;
	std::vector<double> getPs() const;
	Matrix<double>* getCd() const;
	Matrix<double>* getCf() const;
	Matrix<double>* getCm() const;
	Matrix<double>* getXd() const;
	Matrix<double>* getXf() const;
	Matrix<double>* getXm() const;
	Matrix<double>* getXdMin() const;
	Matrix<double>* getXfMin() const;
	Matrix<double>* getXmMin() const;
	Matrix<double>* getXdMax() const;
	double getMaxX(int offset);
	Matrix<double>* getXfMax() const;
	Matrix<double>* getXmMax() const;
	double getQuality(double* solutionArr, int solutionArrSize, errorCodes& errorCode);
	bool constraintsSatisfied(double* solution, int arrSize, errorCodes& errorCode);
	std::vector<double> minValuesRange();
	std::vector<double> maxValuesRange();
	errorCodes generateInstance(unsigned int instanceSeed);
	errorCodes setDFMSqty(int qtyD, int qtyF, int qtyM, int qtyS);
	void setMinMax(double* minMaxData, int startIndex);
	void setMinMax();
	int getSolutionSize();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	double* getDefaultMinMaxData();
	errorCodes randomProblemProperty(unsigned int instanceSeed);
	errorCodes setRandomCd(RandomGenerator<double>& random);
	errorCodes setRandomCf(RandomGenerator<double>& random);
	errorCodes setRandomCm(RandomGenerator<double>& random);
	errorCodes setRandomUd(RandomGenerator<double>& random);
	errorCodes setRandomUf(RandomGenerator<double>& random);
	errorCodes setRandomUm(RandomGenerator<double>& random);
	errorCodes setRandomSd(RandomGenerator<double>& random);
	errorCodes setRandomSf(RandomGenerator<double>& random);
	errorCodes setRandomSm(RandomGenerator<double>& random);
	errorCodes setRandomSs(RandomGenerator<double>& random);
	errorCodes setRandomPs(RandomGenerator<double>& random);

	void setDefaultQty();
	bool validateArrSize(int arrSize);
	errorCodes setDFMS(double* dfmsData);
	void makeMatrixes();
	void resizeAllVectors();
	errorCodes setValuesFromData(double* data, int arrSize);
	errorCodes setInMatrix(Matrix<double>& matrix, double value, int i, int j);
	errorCodes setInVector(std::vector<double>& vector, const double value, const int i);
	errorCodes validateSolution(double* solution, int arrSize);
	double fromDtoFcostKt(Matrix<double>& xd);
	double fromFtoMcostKt(Matrix<double>& xf);
	double fromMtoScostKt(Matrix<double>& xm);
	double calculateKu(Matrix<double>& xd, Matrix<double>& xf, Matrix<double>& xm);
	static int getIndexInArrAs2D(int row, int matrixWidth, int column);
	double calculateP(Matrix<double>& xm);
	double calculateKt(Matrix<double>& xd, Matrix<double>& xf, Matrix<double>& xm);
	double getProfit(Matrix<double>& xd, Matrix<double>& xf, Matrix<double>& xm);
};
