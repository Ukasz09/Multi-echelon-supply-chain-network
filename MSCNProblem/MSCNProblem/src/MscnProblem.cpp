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
	delete fromSuppliersToFactoriesCost;
	delete fromFactoriesToStoreCost;
	delete fromStoreToShopsCost;

	delete qtyOfFeedstockDeliverBySuppliers;
	delete qtyOfProductDeliverByFactories;
	delete qtyOfProductDeliverByStore;

	delete qtyOfFeedstockDeliverBySuppliersMin;
	delete qtyOfProductDeliverByFactoriesMin;
	delete qtyOfProductDeliverByStoreMin;

	delete qtyOfFeedstockDeliverBySuppliersMax;
	delete qtyOfProductDeliverByFactoriesMax;
	delete qtyOfProductDeliverByStoreMax;
}

void MscnProblem::setDefaultQty() {
	suppliersQty = DEFAULT_SUPPLIERS_QTY;
	factoriesQty = DEFAULT_FACTORIES_QTY;
	storeQty = DEFAULT_STORE_QTY;
	shopsQty = DEFAULT_SHOPS_QTY;
}

void MscnProblem::resizeAllVectors() {
	suppliersManufacturingCapacity.resize(suppliersQty);
	factoriesManufacturingCapacity.resize(factoriesQty);
	storeManufacturingCapacity.resize(storeQty);
	shopsManufacturingCapacity.resize(shopsQty);

	oneTimeCostForSupplier.resize(suppliersQty);
	oneTimeCostForFactories.resize(factoriesQty);
	oneTimeCostForStore.resize(storeQty);

	profitFromOnePieceInShops.resize(shopsQty);
}

MscnProblem::errorCodes MscnProblem::setValuesFromData(double* data, int arrSize) {
	if (data == nullptr)
		return nullptrSolution;
	if (arrSize < 4)
		return incorrectSize;

	int nextIndex = 0;

	//setting D,F,M,S
	errorCodes err = setInventory(data);
	nextIndex = 4;
	if (err != noErrors)
		return err;

	if (!validateArrSize(arrSize))
		return incorrectSize;

	//init sd, sf, sm, ss, ud, uf, um, ps
	resizeAllVectors();

	//sd
	for (int i = 0; i < suppliersQty; i++) {
		err = setSuppliersManufacturingCapacity(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//sf
	for (int i = 0; i < factoriesQty; i++) {
		err = setFactoriesManufacturingCapacity(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//sm
	for (int i = 0; i < storeQty; i++) {
		err = setStoreManufacturingCapacity(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//ss
	for (int i = 0; i < shopsQty; i++) {
		err = setShopsManufacturingCapacity(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//init cd,cf,cm, other matrixes
	makeMatrixes();

	//cd
	for (int i = 0; i < suppliersQty; i++)
		for (int j = 0; j < factoriesQty; j++) {
			err = setFromSuppliersToFactoriesCost(data[nextIndex], i, j);
			nextIndex++;
			if (err != noErrors)
				return err;
		}
	//cf
	for (int i = 0; i < factoriesQty; i++)
		for (int j = 0; j < storeQty; j++) {
			err = setInFromFactoriesToStoreCost(data[nextIndex], i, j);
			nextIndex++;
			if (err != noErrors)
				return err;
		}
	//cm
	for (int i = 0; i < storeQty; i++)
		for (int j = 0; j < shopsQty; j++) {
			err = setFromStoreToShopsCost(data[nextIndex], i, j);
			nextIndex++;
			if (err != noErrors)
				return err;
		}

	//ud
	for (int i = 0; i < suppliersQty; i++) {
		err = setOneTimeCostForSupplier(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//uf
	for (int i = 0; i < factoriesQty; i++) {
		err = setOneTimeCostForFactories(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//um
	for (int i = 0; i < storeQty; i++) {
		err = setOneTimeCostForStore(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//p
	for (int i = 0; i < shopsQty; i++) {
		err = setProfitFromOnePieceInShops(data[nextIndex], i);
		nextIndex++;
		if (err != noErrors)
			return err;
	}

	//xd - min,max
	for (int i = 0; i < suppliersQty; i++)
		for (int j = 0; j < factoriesQty; j++) {
			qtyOfFeedstockDeliverBySuppliersMin->set(data[nextIndex], i, j);
			nextIndex++;
			qtyOfFeedstockDeliverBySuppliersMax->set(data[nextIndex], i, j);
			nextIndex++;
		}
	//xf - min, max
	for (int i = 0; i < factoriesQty; i++)
		for (int j = 0; j < storeQty; j++) {
			qtyOfProductDeliverByFactoriesMin->set(data[nextIndex], i, j);
			nextIndex++;
			qtyOfProductDeliverByFactoriesMax->set(data[nextIndex], i, j);
			nextIndex++;
		}
	// xm - min, max
	for (int i = 0; i < storeQty; i++)
		for (int j = 0; j < shopsQty; j++) {
			qtyOfProductDeliverByStoreMin->set(data[nextIndex], i, j);
			nextIndex++;
			qtyOfProductDeliverByStoreMax->set(data[nextIndex], i, j);
			nextIndex++;
		}

	return noErrors;
}

bool MscnProblem::validateArrSize(int arrSize) {
	int correctArrSize = 4 + suppliersQty + factoriesQty + storeQty + shopsQty + suppliersQty * factoriesQty + factoriesQty * storeQty + storeQty * shopsQty + suppliersQty + factoriesQty + storeQty +
		shopsQty + 2 * suppliersQty * factoriesQty + 2 * factoriesQty * storeQty + 2 * storeQty * shopsQty;
	return correctArrSize == arrSize;
}

MscnProblem::errorCodes MscnProblem::setInventory(double* dfmsData) {
	errorCodes err;
	err = setSuppliersQty(dfmsData[0]);
	if (err != noErrors)
		return err;

	err = setFactoriesQty(dfmsData[1]);
	if (err != noErrors)
		return err;

	err = setStoreQty(dfmsData[2]);
	if (err != noErrors)
		return err;

	err = setShopsQty(dfmsData[3]);
	if (err != noErrors)
		return err;

	return noErrors;
}

void MscnProblem::makeMatrixes() {
	fromSuppliersToFactoriesCost = new Matrix<double>(factoriesQty, suppliersQty);
	fromFactoriesToStoreCost = new Matrix<double>(storeQty, factoriesQty);
	fromStoreToShopsCost = new Matrix<double>(shopsQty, storeQty);

	qtyOfFeedstockDeliverBySuppliers = new Matrix<double>(factoriesQty, suppliersQty);
	qtyOfProductDeliverByFactories = new Matrix<double>(storeQty, factoriesQty);
	qtyOfProductDeliverByStore = new Matrix<double>(shopsQty, storeQty);

	qtyOfFeedstockDeliverBySuppliersMin = new Matrix<double>(factoriesQty, suppliersQty);
	qtyOfProductDeliverByFactoriesMin = new Matrix<double>(storeQty, factoriesQty);
	qtyOfProductDeliverByStoreMin = new Matrix<double>(shopsQty, storeQty);

	qtyOfFeedstockDeliverBySuppliersMax = new Matrix<double>(factoriesQty, suppliersQty);
	qtyOfProductDeliverByFactoriesMax = new Matrix<double>(storeQty, factoriesQty);
	qtyOfProductDeliverByStoreMax = new Matrix<double>(shopsQty, storeQty);
}

MscnProblem::errorCodes MscnProblem::setSuppliersQty(int qty) {
	if (qty < 0)
		return incorrectSize;
	suppliersQty = qty;
	resizeAllVectors();
	makeMatrixes();
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setFactoriesQty(int qty) {
	if (qty < 0)
		return incorrectSize;
	factoriesQty = qty;
	resizeAllVectors();
	makeMatrixes();
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setStoreQty(int qty) {
	if (qty < 0)
		return incorrectSize;
	storeQty = qty;
	resizeAllVectors();
	makeMatrixes();
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setShopsQty(int qty) {
	if (qty < 0)
		return incorrectSize;
	shopsQty = qty;
	resizeAllVectors();
	makeMatrixes();
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setFromSuppliersToFactoriesCost(double value, int i, int j) {
	return setInMatrix(*fromSuppliersToFactoriesCost, value, i, j);
}

MscnProblem::errorCodes MscnProblem::setInFromFactoriesToStoreCost(double value, int i, int j) {
	return setInMatrix(*fromFactoriesToStoreCost, value, i, j);
}

MscnProblem::errorCodes MscnProblem::setFromStoreToShopsCost(double value, int i, int j) {
	return setInMatrix(*fromStoreToShopsCost, value, i, j);
}

MscnProblem::errorCodes MscnProblem::setSuppliersManufacturingCapacity(double value, int i) {
	return setInVector(suppliersManufacturingCapacity, value, i);
}

MscnProblem::errorCodes MscnProblem::setFactoriesManufacturingCapacity(double value, int i) {
	return setInVector(factoriesManufacturingCapacity, value, i);
}

MscnProblem::errorCodes MscnProblem::setStoreManufacturingCapacity(double value, int i) {
	return setInVector(storeManufacturingCapacity, value, i);
}

MscnProblem::errorCodes MscnProblem::setShopsManufacturingCapacity(double value, int i) {
	return setInVector(shopsManufacturingCapacity, value, i);
}

MscnProblem::errorCodes MscnProblem::setProfitFromOnePieceInShops(double value, int i) {
	return setInVector(profitFromOnePieceInShops, value, i);
}

MscnProblem::errorCodes MscnProblem::setOneTimeCostForSupplier(double value, int i) {
	return setInVector(oneTimeCostForSupplier, value, i);
}

MscnProblem::errorCodes MscnProblem::setOneTimeCostForFactories(double value, int i) {
	return setInVector(oneTimeCostForFactories, value, i);
}

MscnProblem::errorCodes MscnProblem::setOneTimeCostForStore(double value, int i) {
	return setInVector(oneTimeCostForStore, value, i);
}

int MscnProblem::getSuppliersQty() const {
	return suppliersQty;
}

int MscnProblem::getFactoriesQty() const {
	return factoriesQty;
}

int MscnProblem::getStoreQty() const {
	return storeQty;
}

int MscnProblem::getShopsQty() const {
	return shopsQty;
}

std::vector<double> MscnProblem::getSuppliersManufacturingCapacity() const {
	return suppliersManufacturingCapacity;
}

std::vector<double> MscnProblem::getFactoriesManufacturingCapacity() const {
	return factoriesManufacturingCapacity;
}

std::vector<double> MscnProblem::getStoreManufacturingCapacity() const {
	return storeManufacturingCapacity;
}

std::vector<double> MscnProblem::getShopsManufacturingCapacity() const {
	return shopsManufacturingCapacity;
}

std::vector<double> MscnProblem::getOneTimeCostForSupplier() const {
	return oneTimeCostForSupplier;
}

std::vector<double> MscnProblem::getOneTimeCostForFactories() const {
	return oneTimeCostForFactories;
}

std::vector<double> MscnProblem::getOneTimeCostForStore() const {
	return oneTimeCostForStore;
}

std::vector<double> MscnProblem::getProfitFromOnePieceInShops() const {
	return profitFromOnePieceInShops;
}

Matrix<double>* MscnProblem::getFromSuppliersToFactoriesCost() const {
	return fromSuppliersToFactoriesCost;
}

Matrix<double>* MscnProblem::getFromFactoriesToStoreCost() const {
	return fromFactoriesToStoreCost;
}

Matrix<double>* MscnProblem::getFromStoreToShopsCost() const {
	return fromStoreToShopsCost;
}

Matrix<double>* MscnProblem::getQtyOfFeedstockDeliverBySuppliers() const {
	return qtyOfFeedstockDeliverBySuppliers;
}

Matrix<double>* MscnProblem::getQtyOfProductDeliverByFactories() const {
	return qtyOfProductDeliverByFactories;
}

Matrix<double>* MscnProblem::getQtyOfProductDeliverByStore() const {
	return qtyOfProductDeliverByStore;
}

Matrix<double>* MscnProblem::getQtyOfFeedstockDeliverBySuppliersMin() const {
	return qtyOfFeedstockDeliverBySuppliersMin;
}

Matrix<double>* MscnProblem::getQtyOfProductDeliverByFactoriesMin() const {
	return qtyOfProductDeliverByFactoriesMin;
}

Matrix<double>* MscnProblem::getQtyOfProductDeliverByStoreMin() const {
	return qtyOfProductDeliverByStoreMin;
}

Matrix<double>* MscnProblem::getQtyOfFeedstockDeliverBySuppliersMax() const {
	return qtyOfFeedstockDeliverBySuppliersMax;
}

double MscnProblem::getMaxDeliverQty(int offset) {
	int lastXdIndex = suppliersQty * factoriesQty - 1;
	int lastXfIndex = lastXdIndex + factoriesQty * storeQty;
	int lastXmIndex = lastXfIndex + storeQty * shopsQty;

	if (offset <= lastXdIndex) {
		int yInMatrix = offset % factoriesQty;
		int xInMatrix = offset / suppliersQty;
		return qtyOfFeedstockDeliverBySuppliersMax->get(xInMatrix, yInMatrix);
	}
	else if (offset <= lastXfIndex) {
		int yInMatrix = offset % storeQty;
		int xInMatrix = offset / factoriesQty;
		return qtyOfProductDeliverByFactoriesMax->get(xInMatrix, yInMatrix);
	}
	else {
		int yInMatrix = offset % shopsQty;
		int xInMatrix = offset / storeQty;
		return qtyOfProductDeliverByStoreMax->get(xInMatrix, yInMatrix);
	}
}

Matrix<double>* MscnProblem::getQtyOfProductDeliverByFactoriesMax() const {
	return qtyOfProductDeliverByFactoriesMax;
}

Matrix<double>* MscnProblem::getQtyOfProductDeliverByStoreMax() const {
	return qtyOfProductDeliverByStoreMax;
}

double MscnProblem::getProfit(double* solutionArr, int solutionArrSize, errorCodes& errorCode) {
	if ((errorCode = validateSolution(solutionArr, solutionArrSize)) != noErrors)
		return 0;
	for (int i = 0; i < suppliersQty; i++)
		for (int j = 0; j < factoriesQty; j++)
			qtyOfFeedstockDeliverBySuppliers->set(solutionArr[getIndexInArrAs2D(i, factoriesQty, j)], i, j);
	for (int i = 0; i < factoriesQty; i++)
		for (int j = 0; j < storeQty; j++)
			qtyOfProductDeliverByFactories->set(solutionArr[qtyOfFeedstockDeliverBySuppliers->getSize() + getIndexInArrAs2D(i, storeQty, j)], i, j);
	for (int i = 0; i < storeQty; i++)
		for (int j = 0; j < shopsQty; j++)
			qtyOfProductDeliverByStore->set(solutionArr[qtyOfFeedstockDeliverBySuppliers->getSize() + qtyOfProductDeliverByFactories->getSize() + getIndexInArrAs2D(i, shopsQty, j)], i, j);
	return getProfit(*qtyOfFeedstockDeliverBySuppliers, *qtyOfProductDeliverByFactories, *qtyOfProductDeliverByStore);
}

double MscnProblem::getProfit(Matrix<double>& qtyOfFeedstockDeliverBySuppliers, Matrix<double>& qtyOfProductDeliverByFactories, Matrix<double>& qtyOfProductDeliverByStore) {
	return calculateAllSalesProfit(qtyOfProductDeliverByStore) - calculateAllTransportCosts(qtyOfFeedstockDeliverBySuppliers, qtyOfProductDeliverByFactories, qtyOfProductDeliverByStore) - calculateOnceCosts(qtyOfFeedstockDeliverBySuppliers, qtyOfProductDeliverByFactories, qtyOfProductDeliverByStore);
}

double MscnProblem::calculateAllSalesProfit(Matrix<double>& qtyOfProductDeliverByStore) {
	double result = 0;
	for (int i = 0; i < storeQty; ++i)
		for (int j = 0; j < shopsQty; ++j)
			result += profitFromOnePieceInShops[j] * qtyOfProductDeliverByStore.get(i, j);
	return result;
}

double MscnProblem::calculateAllTransportCosts(Matrix<double>& qtyOfFeedstockDeliverBySuppliers, Matrix<double>& qtyOfProductDeliverByFactories, Matrix<double>& qtyOfProductDeliverByStore) {
	double result = 0;
	const double dToFCost = feedstockDeliverBySuppliersTransportCost(qtyOfFeedstockDeliverBySuppliers);
	const double fToMCost = productDeliverByFactoriesTransportCost(qtyOfProductDeliverByFactories);
	const double mToSCost = productDeliverByStoreTransportCost(qtyOfProductDeliverByStore);
	return dToFCost + fToMCost + mToSCost;
}

double MscnProblem::feedstockDeliverBySuppliersTransportCost(Matrix<double>& qtyOfFeedstockDeliverBySuppliers) {
	double result = 0;
	for (int i = 0; i < suppliersQty; i++)
		for (int j = 0; j < factoriesQty; j++)
			if (qtyOfFeedstockDeliverBySuppliers.get(i, j) > 0)
				result += (*fromSuppliersToFactoriesCost).get(i, j) * qtyOfFeedstockDeliverBySuppliers.get(i, j);
	return result;
}

double MscnProblem::productDeliverByFactoriesTransportCost(Matrix<double>& qtyOfProductDeliverByFactories) {
	double result = 0;
	for (int i = 0; i < factoriesQty; i++)
		for (int j = 0; j < storeQty; j++)
			if (qtyOfProductDeliverByFactories.get(i, j) > 0)
				result += (*fromFactoriesToStoreCost).get(i, j) * qtyOfProductDeliverByFactories.get(i, j);
	return result;
}

double MscnProblem::productDeliverByStoreTransportCost(Matrix<double>& qtyOfProductDeliverByStore) {
	double result = 0;
	for (int i = 0; i < storeQty; i++)
		for (int j = 0; j < shopsQty; j++)
			if (qtyOfProductDeliverByStore.get(i, j) > 0)
				result += (*fromStoreToShopsCost).get(i, j) * qtyOfProductDeliverByStore.get(i, j);
	return result;
}

double MscnProblem::calculateOnceCosts(Matrix<double>& qtyOfFeedstockDeliverBySuppliers, Matrix<double>& qtyOfProductDeliverByFactories, Matrix<double>& qtyOfProductDeliverByStore) {
	double result = 0;
	for (int i = 0; i < suppliersQty; i++)
		if (qtyOfFeedstockDeliverBySuppliers.atLeastOneBiggerValThan0InRow(i))
			result += oneTimeCostForSupplier[i];
	for (int i = 0; i < factoriesQty; i++)
		if (qtyOfProductDeliverByFactories.atLeastOneBiggerValThan0InRow(i))
			result += oneTimeCostForFactories[i];
	for (int i = 0; i < storeQty; i++)
		if (qtyOfProductDeliverByStore.atLeastOneBiggerValThan0InRow(i))
			result += oneTimeCostForStore[i];
	return result;
}

bool MscnProblem::constraintsSatisfied(double* solution, int arrSize, errorCodes& errorCode) {
	if ((errorCode = validateSolution(solution, arrSize)) != noErrors)
		return false;

	Matrix<double> xd(factoriesQty, suppliersQty);
	Matrix<double> xf(storeQty, factoriesQty);
	Matrix<double> xm(shopsQty, storeQty);

	for (int i = 0; i < suppliersQty; i++)
		for (int j = 0; j < factoriesQty; j++)
			xd.set(solution[getIndexInArrAs2D(i, factoriesQty, j)], i, j);
	for (int i = 0; i < factoriesQty; i++)
		for (int j = 0; j < storeQty; j++)
			xf.set(solution[xd.getSize() + getIndexInArrAs2D(i, storeQty, j)], i, j);
	for (int i = 0; i < storeQty; ++i)
		for (int j = 0; j < shopsQty; ++j)
			xm.set(solution[xd.getSize() + xf.getSize() + getIndexInArrAs2D(i, shopsQty, j)], i, j);

	for (int i = 0; i < suppliersQty; i++)
		if (xd.sumInTheRow(i) > suppliersManufacturingCapacity[i])
			return false;
	for (int i = 0; i < factoriesQty; i++)
		if (xf.sumInTheRow(i) > factoriesManufacturingCapacity[i])
			return false;
	for (int i = 0; i < storeQty; i++)
		if (xm.sumInTheRow(i) > storeManufacturingCapacity[i])
			return false;
	for (int i = 0; i < shopsQty; i++)
		if (xm.sumInColumn(i) > shopsManufacturingCapacity[i])
			return false;
	for (int j = 0; j < factoriesQty; j++)
		if (xd.sumInColumn(j) < xf.sumInTheRow(j))
			return false;
	for (int i = 0; i < storeQty; i++)
		if (xf.sumInColumn(i) < xm.sumInTheRow(i))
			return false;

	return true;
}

std::vector<double> MscnProblem::minValuesRange() {
	std::vector<double> v;
	for (int i = 0; i < suppliersQty; i++)
		for (int j = 0; j < factoriesQty; j++)
			v.push_back(qtyOfFeedstockDeliverBySuppliersMin->get(i, j));
	for (int i = 0; i < factoriesQty; i++)
		for (int j = 0; j < storeQty; j++)
			v.push_back(qtyOfProductDeliverByFactoriesMin->get(i, j));
	for (int i = 0; i < storeQty; i++)
		for (int j = 0; j < shopsQty; j++)
			v.push_back(qtyOfProductDeliverByStoreMin->get(i, j));
	return v;
}

std::vector<double> MscnProblem::maxValuesRange() {
	std::vector<double> v;
	for (int i = 0; i < suppliersQty; i++)
		for (int j = 0; j < factoriesQty; j++)
			v.push_back(qtyOfFeedstockDeliverBySuppliersMax->get(i, j));
	for (int i = 0; i < factoriesQty; i++)
		for (int j = 0; j < storeQty; j++)
			v.push_back(qtyOfProductDeliverByFactoriesMax->get(i, j));
	for (int i = 0; i < storeQty; i++)
		for (int j = 0; j < shopsQty; j++)
			v.push_back(qtyOfProductDeliverByStoreMax->get(i, j));
	return v;
}

MscnProblem::errorCodes MscnProblem::setInventoryQty(int suppliersQty, int factoriesQty, int storeQty, int shopsQty) {
	MscnProblem::errorCodes errCode;
	//work proper because of lazy evaluation 
	if (
		(errCode = setSuppliersQty(suppliersQty)) != noErrors
		|| (errCode = setFactoriesQty(factoriesQty)) != noErrors
		|| (errCode = setStoreQty(storeQty)) != noErrors
		|| (errCode = setShopsQty(shopsQty)) != noErrors
	)
		return errCode;
	return noErrors;
}

void MscnProblem::setMinMax(double* minMaxData, int startIndex) {
	//xd - min,max
	int nextIndex = startIndex;
	//xd - min,max
	for (int i = 0; i < suppliersQty; i++)
		for (int j = 0; j < factoriesQty; j++) {
			qtyOfFeedstockDeliverBySuppliersMin->set(minMaxData[nextIndex], i, j);
			nextIndex++;
			qtyOfFeedstockDeliverBySuppliersMax->set(minMaxData[nextIndex], i, j);
			nextIndex++;
		}
	//xf - min, max
	for (int i = 0; i < factoriesQty; i++)
		for (int j = 0; j < storeQty; j++) {
			qtyOfProductDeliverByFactoriesMin->set(minMaxData[nextIndex], i, j);
			nextIndex++;
			qtyOfProductDeliverByFactoriesMax->set(minMaxData[nextIndex], i, j);
			nextIndex++;
		}
	// xm - min, max
	for (int i = 0; i < storeQty; i++)
		for (int j = 0; j < shopsQty; j++) {
			qtyOfProductDeliverByStoreMin->set(minMaxData[nextIndex], i, j);
			nextIndex++;
			qtyOfProductDeliverByStoreMax->set(minMaxData[nextIndex], i, j);
			nextIndex++;
		}
}

void MscnProblem::setMinMax() {
	setMinMax(getDefaultMinMaxData(), 0);
}

int MscnProblem::getSolutionSize() {
	int solutionSize = suppliersQty * factoriesQty + factoriesQty * storeQty + storeQty * shopsQty;
	return solutionSize;
}

double* MscnProblem::getDefaultMinMaxData() {
	int size = (suppliersQty * factoriesQty + factoriesQty * storeQty + storeQty * shopsQty) * 2;
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
		(errorCode = setRandomFromSuppliersToFactoriesCost(random)) != noErrors
		|| (errorCode = setRandomFromFactoriesToStoreCost(random)) != noErrors
		|| (errorCode = setRandomFromStoreToShopsCost(random)) != noErrors
	)
		return errorCode;
	if (
		(errorCode = setRandomOneTimeCostForSupplier(random)) != noErrors
		|| (errorCode = setRandomOneTimeCostForFactories(random)) != noErrors
		|| (errorCode = setRandomOneTimeCostForStore(random)) != noErrors
	)
		return errorCode;
	if (
		(errorCode = setRandomSd(random)) != noErrors
		|| (errorCode = setRandomFactoriesManufacturingCapacity(random)) != noErrors
		|| (errorCode = setRandomStoreManufacturingCapacity(random)) != noErrors
		|| (errorCode = setRandomShopsManufacturingCapacity(random)) != noErrors
	)
		return errorCode;

	errorCode = setRandomProfitFromOnePieceInShops(random);

	return errorCode;
}

MscnProblem::errorCodes MscnProblem::setRandomFromSuppliersToFactoriesCost(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < suppliersQty; i++)
		for (int j = 0; j < factoriesQty; j++) {
			err = setFromSuppliersToFactoriesCost(random.getRandom(MscGeneratorRanges::FROM_SUPPLIERS_TO_FACTORIES_COST_MIN, MscGeneratorRanges::FROM_SUPPLIERS_TO_FACTORIES_COST_MAX), i, j);
			if (err != noErrors)
				return err;
		}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomFromFactoriesToStoreCost(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < factoriesQty; i++)
		for (int j = 0; j < storeQty; j++) {
			err = setInFromFactoriesToStoreCost(random.getRandom(MscGeneratorRanges::FROM_FACTORIES_TO_STORE_COST_MIN, MscGeneratorRanges::FROM_FACTORIES_TO_STORE_COST_MAX), i, j);
			if (err != noErrors)
				return err;
		}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomFromStoreToShopsCost(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < storeQty; i++)
		for (int j = 0; j < shopsQty; j++) {
			err = setFromStoreToShopsCost(random.getRandom(MscGeneratorRanges::FROM_STORE_TO_SHOPS_COST_MIN, MscGeneratorRanges::FROM_STORE_TO_SHOPS_COST_MAX), i, j);
			if (err != noErrors)
				return err;
		}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomOneTimeCostForSupplier(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < suppliersQty; i++) {
		err = setOneTimeCostForSupplier(random.getRandom(MscGeneratorRanges::ONE_TIME_COST_FOR_SUPPLIER_MIN, MscGeneratorRanges::ONE_TIME_COST_FOR_SUPPLIER_MAX), i);
		if (err != noErrors)
			return err;
	}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomOneTimeCostForFactories(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < factoriesQty; i++) {
		err = setOneTimeCostForFactories(random.getRandom(MscGeneratorRanges::ONE_TIME_COST_FOR_FACTORIES_MIN, MscGeneratorRanges::ONE_TIME_COST_FOR_FACTORIES_MAX), i);
		if (err != noErrors)
			return err;
	}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomOneTimeCostForStore(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < storeQty; i++) {
		err = setOneTimeCostForStore(random.getRandom(MscGeneratorRanges::ONE_TIME_COST_FOR_STORE_MIN, MscGeneratorRanges::ONE_TIME_COST_FOR_STORE_MAX), i);
		if (err != noErrors)
			return err;
	}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomSd(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < suppliersQty; i++) {
		err = setSuppliersManufacturingCapacity(random.getRandom(MscGeneratorRanges::SUPPLIERS_MANUFACTURING_CAPACITY_MIN, MscGeneratorRanges::SUPPLIERS_MANUFACTURING_CAPACITY_MAX), i);
		if (err != noErrors)
			return err;
	}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomFactoriesManufacturingCapacity(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < factoriesQty; i++) {
		err = setFactoriesManufacturingCapacity(random.getRandom(MscGeneratorRanges::FACTORIES_MANUFACTURING_CAPACITY_MIN, MscGeneratorRanges::FACTORIES_MANUFACTURING_CAPACITY_MAX), i);
		if (err != noErrors)
			return err;
	}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomStoreManufacturingCapacity(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < storeQty; i++) {
		err = setStoreManufacturingCapacity(random.getRandom(MscGeneratorRanges::STORE_MANUFACTURING_CAPACITY_MIN, MscGeneratorRanges::STORE_MANUFACTURING_CAPACITY_MAX), i);
		if (err != noErrors)
			return err;
	}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomShopsManufacturingCapacity(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < shopsQty; i++) {
		err = setShopsManufacturingCapacity(random.getRandom(MscGeneratorRanges::SHOPS_MANUFACTURING_CAPACITY_MIN, MscGeneratorRanges::SHOPS_MANUFACTURING_CAPACITY_MAX), i);
		if (err != noErrors)
			return err;
	}
	return noErrors;
}

MscnProblem::errorCodes MscnProblem::setRandomProfitFromOnePieceInShops(RandomGenerator<double>& random) {
	errorCodes err;
	for (int i = 0; i < shopsQty; i++) {
		err = setProfitFromOnePieceInShops(random.getRandom(MscGeneratorRanges::PROFIT_FROM_ONE_PIECE_IN_SHOPS_MIN, MscGeneratorRanges::PROFIT_FROM_ONE_PIECE_IN_SHOPS_MAX), i);
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
	const int requiredSize = suppliersQty * factoriesQty + factoriesQty * storeQty + storeQty * shopsQty;
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
