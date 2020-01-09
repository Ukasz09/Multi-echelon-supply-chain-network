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
	static const int DEFAULT_SUPPLIERS_QTY = 6;
	static const int DEFAULT_FACTORIES_QTY = 7;
	static const int DEFAULT_STORE_QTY = 8;
	static const int DEFAULT_SHOPS_QTY = 9;

	int suppliersQty, factoriesQty, storeQty, shopsQty;
	std::vector<double> suppliersManufacturingCapacity;
	std::vector<double> factoriesManufacturingCapacity;
	std::vector<double> storeManufacturingCapacity;
	std::vector<double> shopsManufacturingCapacity;
	std::vector<double> oneTimeCostForSupplier;
	std::vector<double> oneTimeCostForFactories;
	std::vector<double> oneTimeCostForStore;
	std::vector<double> profitFromOnePieceInShops;
	Matrix<double>* fromSuppliersToFactoriesCost;
	Matrix<double>* fromFactoriesToStoreCost;
	Matrix<double>* fromStoreToShopsCost;

	Matrix<double>* qtyOfFeedstockDeliverBySuppliers;
	Matrix<double>* qtyOfProductDeliverByFactories;
	Matrix<double>* qtyOfProductDeliverByStore;

	Matrix<double>* qtyOfFeedstockDeliverBySuppliersMin;
	Matrix<double>* qtyOfProductDeliverByFactoriesMin;
	Matrix<double>* qtyOfProductDeliverByStoreMin;

	Matrix<double>* qtyOfFeedstockDeliverBySuppliersMax;
	Matrix<double>* qtyOfProductDeliverByFactoriesMax;
	Matrix<double>* qtyOfProductDeliverByStoreMax;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	MscnProblem();
	MscnProblem(double* data, int arrSize, errorCodes& err);
	~MscnProblem();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	errorCodes setSuppliersQty(int qty);
	errorCodes setFactoriesQty(int qty);
	errorCodes setStoreQty(int qty);
	errorCodes setShopsQty(int qty);

	errorCodes setFromSuppliersToFactoriesCost(double value, int i, int j);
	errorCodes setInFromFactoriesToStoreCost(double value, int i, int j);
	errorCodes setFromStoreToShopsCost(double value, int i, int j);

	errorCodes setSuppliersManufacturingCapacity(double value, int i);
	errorCodes setFactoriesManufacturingCapacity(double value, int i);
	errorCodes setStoreManufacturingCapacity(double value, int i);
	errorCodes setShopsManufacturingCapacity(double value, int i);

	errorCodes setProfitFromOnePieceInShops(double value, int i);

	errorCodes setOneTimeCostForSupplier(double value, int i);
	errorCodes setOneTimeCostForFactories(double value, int i);
	errorCodes setOneTimeCostForStore(double value, int i);


	int getSuppliersQty() const;
	int getFactoriesQty() const;
	int getStoreQty() const;
	int getShopsQty() const;
	std::vector<double> getSuppliersManufacturingCapacity() const;
	std::vector<double> getFactoriesManufacturingCapacity() const;
	std::vector<double> getStoreManufacturingCapacity() const;
	std::vector<double> getShopsManufacturingCapacity() const;
	std::vector<double> getOneTimeCostForSupplier() const;
	std::vector<double> getOneTimeCostForFactories() const;
	std::vector<double> getOneTimeCostForStore() const;
	std::vector<double> getProfitFromOnePieceInShops() const;
	Matrix<double>* getFromSuppliersToFactoriesCost() const;
	Matrix<double>* getFromFactoriesToStoreCost() const;
	Matrix<double>* getFromStoreToShopsCost() const;
	Matrix<double>* getQtyOfFeedstockDeliverBySuppliers() const;
	Matrix<double>* getQtyOfProductDeliverByFactories() const;
	Matrix<double>* getQtyOfProductDeliverByStore() const;
	Matrix<double>* getQtyOfFeedstockDeliverBySuppliersMin() const;
	Matrix<double>* getQtyOfProductDeliverByFactoriesMin() const;
	Matrix<double>* getQtyOfProductDeliverByStoreMin() const;
	Matrix<double>* getQtyOfFeedstockDeliverBySuppliersMax() const;
	double getMaxDeliverQty(int offset);
	Matrix<double>* getQtyOfProductDeliverByFactoriesMax() const;
	Matrix<double>* getQtyOfProductDeliverByStoreMax() const;
	double getProfit(double* solutionArr, int solutionArrSize, errorCodes& errorCode);
	bool constraintsSatisfied(double* solution, int arrSize, errorCodes& errorCode);
	std::vector<double> minValuesRange();
	std::vector<double> maxValuesRange();
	errorCodes generateInstance(unsigned int instanceSeed);
	errorCodes setInventoryQty(int suppliersQty, int factoriesQty, int storeQty, int shopsQty);
	void setMinMax(double* minMaxData, int startIndex);
	void setMinMax();
	int getSolutionSize();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	double* getDefaultMinMaxData();
	errorCodes randomProblemProperty(unsigned int instanceSeed);
	errorCodes setRandomFromSuppliersToFactoriesCost(RandomGenerator<double>& random);
	errorCodes setRandomFromFactoriesToStoreCost(RandomGenerator<double>& random);
	errorCodes setRandomFromStoreToShopsCost(RandomGenerator<double>& random);
	errorCodes setRandomOneTimeCostForSupplier(RandomGenerator<double>& random);
	errorCodes setRandomOneTimeCostForFactories(RandomGenerator<double>& random);
	errorCodes setRandomOneTimeCostForStore(RandomGenerator<double>& random);
	errorCodes setRandomSd(RandomGenerator<double>& random);
	errorCodes setRandomFactoriesManufacturingCapacity(RandomGenerator<double>& random);
	errorCodes setRandomStoreManufacturingCapacity(RandomGenerator<double>& random);
	errorCodes setRandomShopsManufacturingCapacity(RandomGenerator<double>& random);
	errorCodes setRandomProfitFromOnePieceInShops(RandomGenerator<double>& random);

	void setDefaultQty();
	bool validateArrSize(int arrSize);
	errorCodes setInventory(double* dfmsData);
	void makeMatrixes();
	void resizeAllVectors();
	errorCodes setValuesFromData(double* data, int arrSize);
	errorCodes setInMatrix(Matrix<double>& matrix, double value, int i, int j);
	errorCodes setInVector(std::vector<double>& vector, const double value, const int i);
	errorCodes validateSolution(double* solution, int arrSize);
	double feedstockDeliverBySuppliersTransportCost(Matrix<double>& qtyOfFeedstockDeliverBySuppliers);
	double productDeliverByFactoriesTransportCost(Matrix<double>& qtyOfProductDeliverByFactories);
	double productDeliverByStoreTransportCost(Matrix<double>& qtyOfProductDeliverByStore);
	double calculateOnceCosts(Matrix<double>& qtyOfFeedstockDeliverBySuppliers, Matrix<double>& qtyOfProductDeliverByFactories,
	                   Matrix<double>& qtyOfProductDeliverByStore);
	static int getIndexInArrAs2D(int row, int matrixWidth, int column);
	double calculateAllSalesProfit(Matrix<double>& qtyOfProductDeliverByStore);
	double calculateAllTransportCosts(Matrix<double>& qtyOfFeedstockDeliverBySuppliers, Matrix<double>& qtyOfProductDeliverByFactories,
	                   Matrix<double>& qtyOfProductDeliverByStore);
	double getProfit(Matrix<double>& qtyOfFeedstockDeliverBySuppliers, Matrix<double>& qtyOfProductDeliverByFactories,
	                 Matrix<double>& qtyOfProductDeliverByStore);
};
