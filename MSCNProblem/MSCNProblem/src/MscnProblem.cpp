#include "../include/MscnProblem.h"
#include "../include/utilities/RandomGenerator.h"
#include "../include/utilities/MscGeneratorRanges.h"

MscnProblem::MscnProblem() {
	setDefaultQty();
	makeMatrixes();
	resizeAllVectors();
}

MscnProblem::MscnProblem(double* data, int arrSize, errorCodes& err) {
	err = setValuesFromData(data, arrSize);
	if (err != noErrors)
		MscnProblem();
}

MscnProblem::~MscnProblem() {
	delete cd;
	delete cf;
	delete cm;

	delete xd;
	delete xf;
	delete xm;

	delete xdMin;
	delete xfMin;
	delete xmMin;

	delete xdMax;
	delete xfMax;
	delete xmMax;
}

void MscnProblem::setDefaultQty() {
	dQty = DEFAULT_QTY_OF_D;
	fQty = DEFAULT_QTY_OF_F;
	mQty = DEFAULT_QTY_OF_M;
	sQty = DEFAULT_QTY_OF_S;
}

void MscnProblem::resizeAllVectors() {
	sd.resize(dQty);
	sf.resize(fQty);
	sm.resize(mQty);
	ss.resize(sQty);

	ud.resize(dQty);
	uf.resize(fQty);
	um.resize(mQty);

	ps.resize(sQty);
}

MscnProblem::errorCodes MscnProblem::setValuesFromData(double* data, int arrSize) {
	if (data == nullptr)
		return nullptrSolution;
	if (arrSize < 4)
		return incorrectSize;

	int nextIndex = 0;

	//setting D,F,M,S
	errorCodes err = setDFMS(data);
	nextIndex = 4;
	if (err != noErrors)
		return err;

	if (!validateArrSize(arrSize))
		return incorrectSize;

	//init sd, sf, sm, ss, ud, uf, um, ps
	resizeAllVectors();

	//sd
	for (int i = 0; i < dQty; i++) {
		err = setInSd(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//sf
	for (int i = 0; i < fQty; i++) {
		err = setInSf(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//sm
	for (int i = 0; i < mQty; i++) {
		err = setInSm(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//ss
	for (int i = 0; i < sQty; i++) {
		err = setInSs(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//init cd,cf,cm, other matrixes
	makeMatrixes();

	//cd
	for (int i = 0; i < dQty; i++)
		for (int j = 0; j < fQty; j++) {
			err = setInCd(data[nextIndex], i, j);
			nextIndex++;
			if (err != noErrors)
				return err;
		}
	//cf
	for (int i = 0; i < fQty; i++)
		for (int j = 0; j < mQty; j++) {
			err = setInCf(data[nextIndex], i, j);
			nextIndex++;
			if (err != noErrors)
				return err;
		}
	//cm
	for (int i = 0; i < mQty; i++)
		for (int j = 0; j < sQty; j++) {
			err = setInCm(data[nextIndex], i, j);
			nextIndex++;
			if (err != noErrors)
				return err;
		}

	//ud
	for (int i = 0; i < dQty; i++) {
		err = setInUd(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//uf
	for (int i = 0; i < fQty; i++) {
		err = setInUf(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//um
	for (int i = 0; i < mQty; i++) {
		err = setInUm(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//p
	for (int i = 0; i < sQty; i++) {
		err = setInPs(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//xd - min,max
	for (int i = 0; i < dQty; i++)
		for (int j = 0; j < fQty; j++) {
			xdMin->set(data[nextIndex], i, j);
			nextIndex++;
			xdMax->set(data[nextIndex], i, j);
			nextIndex++;
		}
	//xf - min, max
	for (int i = 0; i < fQty; i++)
		for (int j = 0; j < mQty; j++) {
			xfMin->set(data[nextIndex], i, j);
			nextIndex++;
			xfMax->set(data[nextIndex], i, j);
			nextIndex++;
		}
	// xm - min, max
	for (int i = 0; i < mQty; i++)
		for (int j = 0; j < sQty; j++) {
			xmMin->set(data[nextIndex], i, j);
			nextIndex++;
			xmMax->set(data[nextIndex], i, j);
			nextIndex++;
		}

	return noErrors;
}

bool MscnProblem::validateArrSize(int arrSize) {
	int correctArrSize = 4 + dQty + fQty + mQty + sQty + dQty * fQty + fQty * mQty + mQty * sQty + dQty + fQty + mQty +
		sQty + 2 * dQty * fQty + 2 * fQty * mQty + 2 * mQty * sQty;
	return correctArrSize == arrSize;
}

MscnProblem::errorCodes MscnProblem::setDFMS(double* dfmsData) {
	errorCodes err;
	err = setDQty(dfmsData[0]);
	if (err != noErrors)
		return err;

	err = setFQty(dfmsData[1]);
	if (err != noErrors)
		return err;

	err = setMQty(dfmsData[2]);
	if (err != noErrors)
		return err;

	err = setSQty(dfmsData[3]);
	if (err != noErrors)
		return err;

	return noErrors;
}

void MscnProblem::makeMatrixes() {
	cd = new Matrix<double>(fQty, dQty);
	cf = new Matrix<double>(mQty, fQty);
	cm = new Matrix<double>(sQty, mQty);

	xd = new Matrix<double>(fQty, dQty);
	xf = new Matrix<double>(mQty, fQty);
	xm = new Matrix<double>(sQty, mQty);

	xdMin = new Matrix<double>(fQty, dQty);
	xfMin = new Matrix<double>(mQty, fQty);
	xmMin = new Matrix<double>(sQty, mQty);

	xdMax = new Matrix<double>(fQty, dQty);
	xfMax = new Matrix<double>(mQty, fQty);
	xmMax = new Matrix<double>(sQty, mQty);
}

MscnProblem::errorCodes MscnProblem::setDQty(int qty) {
	if (qty < 0)
		return incorrectSize;
	dQty = qty;
	resizeAllVectors();
	makeMatrixes();
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setFQty(int qty) {
	if (qty < 0)
		return incorrectSize;
	fQty = qty;
	resizeAllVectors();
	makeMatrixes();
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setMQty(int qty) {
	if (qty < 0)
		return incorrectSize;
	mQty = qty;
	resizeAllVectors();
	makeMatrixes();
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setSQty(int qty) {
	if (qty < 0)
		return incorrectSize;
	sQty = qty;
	resizeAllVectors();
	makeMatrixes();
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setInCd(double value, int i, int j) {
	return setInMatrix(*cd, value, i, j);
}

MscnProblem::errorCodes MscnProblem::setInCf(double value, int i, int j) {
	return setInMatrix(*cf, value, i, j);
}

MscnProblem::errorCodes MscnProblem::setInCm(double value, int i, int j) {
	return setInMatrix(*cm, value, i, j);
}

MscnProblem::errorCodes MscnProblem::setInSd(double value, int i) {
	return setInVector(sd, value, i);
}

MscnProblem::errorCodes MscnProblem::setInSf(double value, int i) {
	return setInVector(sf, value, i);
}

MscnProblem::errorCodes MscnProblem::setInSm(double value, int i) {
	return setInVector(sm, value, i);
}

MscnProblem::errorCodes MscnProblem::setInSs(double value, int i) {
	return setInVector(ss, value, i);
}

MscnProblem::errorCodes MscnProblem::setInPs(double value, int i) {
	return setInVector(ps, value, i);
}

MscnProblem::errorCodes MscnProblem::setInUd(double value, int i) {
	return setInVector(ud, value, i);
}

MscnProblem::errorCodes MscnProblem::setInUf(double value, int i) {
	return setInVector(uf, value, i);
}

MscnProblem::errorCodes MscnProblem::setInUm(double value, int i) {
	return setInVector(um, value, i);
}

int MscnProblem::getDQty() const {
	return dQty;
}

int MscnProblem::getFQty() const {
	return fQty;
}

int MscnProblem::getMQty() const {
	return mQty;
}

int MscnProblem::getSQty() const {
	return sQty;
}

std::vector<double> MscnProblem::getSd() const {
	return sd;
}

std::vector<double> MscnProblem::getSf() const {
	return sf;
}

std::vector<double> MscnProblem::getSm() const {
	return sm;
}

std::vector<double> MscnProblem::getSs() const {
	return ss;
}

std::vector<double> MscnProblem::getUd() const {
	return ud;
}

std::vector<double> MscnProblem::getUf() const {
	return uf;
}

std::vector<double> MscnProblem::getUm() const {
	return um;
}

std::vector<double> MscnProblem::getPs() const {
	return ps;
}

Matrix<double>* MscnProblem::getCd() const {
	return cd;
}

Matrix<double>* MscnProblem::getCf() const {
	return cf;
}

Matrix<double>* MscnProblem::getCm() const {
	return cm;
}

Matrix<double>* MscnProblem::getXd() const {
	return xd;
}

Matrix<double>* MscnProblem::getXf() const {
	return xf;
}

Matrix<double>* MscnProblem::getXm() const {
	return xm;
}

Matrix<double>* MscnProblem::getXdMin() const {
	return xdMin;
}

Matrix<double>* MscnProblem::getXfMin() const {
	return xfMin;
}

Matrix<double>* MscnProblem::getXmMin() const {
	return xmMin;
}

Matrix<double>* MscnProblem::getXdMax() const {
	return xdMax;
}

double MscnProblem::getMaxX(int offset) {
	int lastXdIndex = dQty * fQty - 1;
	int lastXfIndex = lastXdIndex + fQty * mQty;
	int lastXmIndex = lastXfIndex + mQty * sQty;

	if (offset <= lastXdIndex) {
		int yInMatrix = offset % fQty;
		int xInMatrix = offset / dQty;
		return xdMax->get(xInMatrix, yInMatrix);
	}
	else if (offset <= lastXfIndex) {
		int yInMatrix = offset % mQty;
		int xInMatrix = offset / fQty;
		return xfMax->get(xInMatrix, yInMatrix);
	}
	else {
		int yInMatrix = offset % sQty;
		int xInMatrix = offset / mQty;
		return xmMax->get(xInMatrix, yInMatrix);
	}
}

Matrix<double>* MscnProblem::getXfMax() const {
	return xfMax;
}

Matrix<double>* MscnProblem::getXmMax() const {
	return xmMax;
}

double MscnProblem::getQuality(double* solutionArr, int solutionArrSize, errorCodes& errorCode) {
	if ((errorCode = validateSolution(solutionArr, solutionArrSize)) != noErrors)
		return 0;
	for (int i = 0; i < dQty; i++)
		for (int j = 0; j < fQty; j++)
			xd->set(solutionArr[getIndexInArrAs2D(i, fQty, j)], i, j);
	for (int i = 0; i < fQty; i++)
		for (int j = 0; j < mQty; j++)
			xf->set(solutionArr[xd->getSize() + getIndexInArrAs2D(i, mQty, j)], i, j);
	for (int i = 0; i < mQty; i++)
		for (int j = 0; j < sQty; j++)
			xm->set(solutionArr[xd->getSize() + xf->getSize() + getIndexInArrAs2D(i, sQty, j)], i, j);
	return getProfit(*xd, *xf, *xm);
}

double MscnProblem::getProfit(Matrix<double>& xd, Matrix<double>& xf, Matrix<double>& xm) {
	return calculateP(xm) - calculateKt(xd, xf, xm) - calculateKu(xd, xf, xm);
}

double MscnProblem::calculateP(Matrix<double>& xm) {
	double result = 0;
	for (int i = 0; i < mQty; ++i)
		for (int j = 0; j < sQty; ++j)
			result += ps[j] * xm.get(i, j);
	return result;
}

double MscnProblem::calculateKt(Matrix<double>& xd, Matrix<double>& xf, Matrix<double>& xm) {
	double result = 0;
	const double dToFCost = fromDtoFcostKt(xd);
	const double fToMCost = fromFtoMcostKt(xf);
	const double mToSCost = fromMtoScostKt(xm);
	return dToFCost + fToMCost + mToSCost;
}

double MscnProblem::fromDtoFcostKt(Matrix<double>& xd) {
	double result = 0;
	for (int i = 0; i < dQty; i++)
		for (int j = 0; j < fQty; j++)
			if (xd.get(i, j) > 0)
				result += (*cd).get(i, j) * xd.get(i, j);
	return result;
}

double MscnProblem::fromFtoMcostKt(Matrix<double>& xf) {
	double result = 0;
	for (int i = 0; i < fQty; i++)
		for (int j = 0; j < mQty; j++)
			if (xf.get(i, j) > 0)
				result += (*cf).get(i, j) * xf.get(i, j);
	return result;
}

double MscnProblem::fromMtoScostKt(Matrix<double>& xm) {
	double result = 0;
	for (int i = 0; i < mQty; i++)
		for (int j = 0; j < sQty; j++)
			if (xm.get(i, j) > 0)
				result += (*cm).get(i, j) * xm.get(i, j);
	return result;
}

double MscnProblem::calculateKu(Matrix<double>& xd, Matrix<double>& xf, Matrix<double>& xm) {
	double result = 0;
	for (int i = 0; i < dQty; i++)
		if (xd.atLeastOneBiggerValThan0InRow(i))
			result += ud[i];
	for (int i = 0; i < fQty; i++)
		if (xf.atLeastOneBiggerValThan0InRow(i))
			result += uf[i];
	for (int i = 0; i < mQty; i++)
		if (xm.atLeastOneBiggerValThan0InRow(i))
			result += um[i];
	return result;
}

bool MscnProblem::constraintsSatisfied(double* solution, int arrSize, errorCodes& errorCode) {
	if ((errorCode = validateSolution(solution, arrSize)) != noErrors)
		return false;

	Matrix<double> xd(fQty, dQty);
	Matrix<double> xf(mQty, fQty);
	Matrix<double> xm(sQty, mQty);

	for (int i = 0; i < dQty; i++)
		for (int j = 0; j < fQty; j++)
			xd.set(solution[getIndexInArrAs2D(i, fQty, j)], i, j);
	for (int i = 0; i < fQty; i++)
		for (int j = 0; j < mQty; j++)
			xf.set(solution[xd.getSize() + getIndexInArrAs2D(i, mQty, j)], i, j);
	for (int i = 0; i < mQty; ++i)
		for (int j = 0; j < sQty; ++j)
			xm.set(solution[xd.getSize() + xf.getSize() + getIndexInArrAs2D(i, sQty, j)], i, j);

	for (int i = 0; i < dQty; i++)
		if (xd.sumInTheRow(i) > sd[i])
			return false;
	for (int i = 0; i < fQty; i++)
		if (xf.sumInTheRow(i) > sf[i])
			return false;
	for (int i = 0; i < mQty; i++)
		if (xm.sumInTheRow(i) > sm[i])
			return false;
	for (int i = 0; i < sQty; i++)
		if (xm.sumInColumn(i) > ss[i])
			return false;
	for (int j = 0; j < fQty; j++)
		if (xd.sumInColumn(j) < xf.sumInTheRow(j))
			return false;
	for (int i = 0; i < mQty; i++)
		if (xf.sumInColumn(i) < xm.sumInTheRow(i))
			return false;

	return true;
}

std::vector<double> MscnProblem::minValuesRange() {
	std::vector<double> v;
	for (int i = 0; i < dQty; i++)
		for (int j = 0; j < fQty; j++)
			v.push_back(xdMin->get(i, j));
	for (int i = 0; i < fQty; i++)
		for (int j = 0; j < mQty; j++)
			v.push_back(xfMin->get(i, j));
	for (int i = 0; i < mQty; i++)
		for (int j = 0; j < sQty; j++)
			v.push_back(xmMin->get(i, j));
	return v;
}

std::vector<double> MscnProblem::maxValuesRange() {
	std::vector<double> v;
	for (int i = 0; i < dQty; i++)
		for (int j = 0; j < fQty; j++)
			v.push_back(xdMax->get(i, j));
	for (int i = 0; i < fQty; i++)
		for (int j = 0; j < mQty; j++)
			v.push_back(xfMax->get(i, j));
	for (int i = 0; i < mQty; i++)
		for (int j = 0; j < sQty; j++)
			v.push_back(xmMax->get(i, j));
	return v;
}

MscnProblem::errorCodes MscnProblem::setDFMSqty(int qtyD, int qtyF, int qtyM, int qtyS) {
	MscnProblem::errorCodes errCode;
	//work proper because of lazy evaluation 
	if (
		(errCode = setDQty(qtyD)) != noErrors
		|| (errCode = setFQty(qtyF)) != noErrors
		|| (errCode = setMQty(qtyM)) != noErrors
		|| (errCode = setSQty(qtyS)) != noErrors
	)
		return errCode;
	return noErrors;
}

void MscnProblem::setMinMax(double* minMaxData, int startIndex) {
	//xd - min,max
	int nextIndex = startIndex;
	//xd - min,max
	for (int i = 0; i < dQty; i++)
		for (int j = 0; j < fQty; j++) {
			xdMin->set(minMaxData[nextIndex], i, j);
			nextIndex++;
			xdMax->set(minMaxData[nextIndex], i, j);
			nextIndex++;
		}
	//xf - min, max
	for (int i = 0; i < fQty; i++)
		for (int j = 0; j < mQty; j++) {
			xfMin->set(minMaxData[nextIndex], i, j);
			nextIndex++;
			xfMax->set(minMaxData[nextIndex], i, j);
			nextIndex++;
		}
	// xm - min, max
	for (int i = 0; i < mQty; i++)
		for (int j = 0; j < sQty; j++) {
			xmMin->set(minMaxData[nextIndex], i, j);
			nextIndex++;
			xmMax->set(minMaxData[nextIndex], i, j);
			nextIndex++;
		}
}

void MscnProblem::setMinMax() {
	setMinMax(getDefaultMinMaxData(), 0);
}

int MscnProblem::getSolutionSize() {
	int solutionSize = dQty * fQty + fQty * mQty + mQty * sQty;
	return solutionSize;
}

double* MscnProblem::getDefaultMinMaxData() {
	int size = (dQty * fQty + fQty * mQty + mQty * sQty) * 2;
	double* minMax = new double[size];
	for (int i = 0; i < size; i++)
		minMax[i] = i % 2 != 0 ? 100 : 0;
	return minMax;
}

MscnProblem::errorCodes MscnProblem::generateInstance(const unsigned int instanceSeed) {
	return randomProblemProperty(instanceSeed);
}

MscnProblem::errorCodes MscnProblem::randomProblemProperty(const unsigned int instanceSeed) {
	RandomGenerator<double> random(instanceSeed);
	errorCodes errorCode;
	if (
		(errorCode = setRandomCd(random)) != noErrors
		|| (errorCode = setRandomCf(random)) != noErrors
		|| (errorCode = setRandomCm(random)) != noErrors
	)
		return errorCode;
	if (
		(errorCode = setRandomUd(random)) != noErrors
		|| (errorCode = setRandomUf(random)) != noErrors
		|| (errorCode = setRandomUm(random)) != noErrors
	)
		return errorCode;
	if (
		(errorCode = setRandomSd(random)) != noErrors
		|| (errorCode = setRandomSf(random)) != noErrors
		|| (errorCode = setRandomSm(random)) != noErrors
		|| (errorCode = setRandomSs(random)) != noErrors
	)
		return errorCode;

	errorCode = setRandomPs(random);

	return errorCode;
}

MscnProblem::errorCodes MscnProblem::setRandomCd(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < dQty; i++)
		for (int j = 0; j < fQty; j++) {
			err = setInCd(random.getRandom(MscGeneratorRanges::MIN_CD, MscGeneratorRanges::MAX_CD), i, j);
			if (err != noErrors)
				return err;
		}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomCf(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < fQty; i++)
		for (int j = 0; j < mQty; j++) {
			err = setInCf(random.getRandom(MscGeneratorRanges::MIN_CF, MscGeneratorRanges::MAX_CF), i, j);
			if (err != noErrors)
				return err;
		}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomCm(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < mQty; i++)
		for (int j = 0; j < sQty; j++) {
			err = setInCm(random.getRandom(MscGeneratorRanges::MIN_CM, MscGeneratorRanges::MAX_CM), i, j);
			if (err != noErrors)
				return err;
		}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomUd(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < dQty; i++) {
		err = setInUd(random.getRandom(MscGeneratorRanges::MIN_UD, MscGeneratorRanges::MAX_UD), i);
		if (err != noErrors)
			return err;
	}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomUf(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < fQty; i++) {
		err = setInUf(random.getRandom(MscGeneratorRanges::MIN_UF, MscGeneratorRanges::MAX_UF), i);
		if (err != noErrors)
			return err;
	}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomUm(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < mQty; i++) {
		err = setInUm(random.getRandom(MscGeneratorRanges::MIN_UM, MscGeneratorRanges::MAX_UM), i);
		if (err != noErrors)
			return err;
	}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomSd(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < dQty; i++) {
		err = setInSd(random.getRandom(MscGeneratorRanges::MIN_SD, MscGeneratorRanges::MAX_SD), i);
		if (err != noErrors)
			return err;
	}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomSf(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < fQty; i++) {
		err = setInSf(random.getRandom(MscGeneratorRanges::MIN_SF, MscGeneratorRanges::MAX_SF), i);
		if (err != noErrors)
			return err;
	}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomSm(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < mQty; i++) {
		err = setInSm(random.getRandom(MscGeneratorRanges::MIN_SM, MscGeneratorRanges::MAX_SM), i);
		if (err != noErrors)
			return err;
	}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomSs(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < sQty; i++) {
		err = setInSs(random.getRandom(MscGeneratorRanges::MIN_SS, MscGeneratorRanges::MAX_SS), i);
		if (err != noErrors)
			return err;
	}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomPs(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < sQty; i++) {
		err = setInPs(random.getRandom(MscGeneratorRanges::MIN_PS, MscGeneratorRanges::MAX_PS), i);
		if (err != noErrors)
			return err;
	}
	return noErrors;
}

int MscnProblem::getIndexInArrAs2D(const int row, const int matrixWidth, const int column) {
	return row * matrixWidth + column;
}

MscnProblem::errorCodes MscnProblem::validateSolution(double* solution, const int arrSize) {
	if (solution == nullptr) return nullptrSolution;
	const int requiredSize = dQty * fQty + fQty * mQty + mQty * sQty;
	if (arrSize != requiredSize) return incorrectSize;

	for (int i = 0; i < arrSize; i++)
		if (solution[i] < 0) return incorrectCost;

	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setInMatrix(Matrix<double>& matrix, const double value, const int i, const int j) {
	if (value < 0) return incorrectCost;
	matrix.set(value, i, j);
	return noErrors;

}

MscnProblem::errorCodes MscnProblem::setInVector(std::vector<double>& vector, const double value, const int i) {
	if (value < 0) return incorrectCost;

	vector[i] = value;
	return noErrors;
}
