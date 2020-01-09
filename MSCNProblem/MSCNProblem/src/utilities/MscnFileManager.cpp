#include <fstream>
#include <vector>
#include <string>
#include "../../include/MscnProblem.h"
#include "../../include/utilities/MscnFileManager.h"

MscnFileManager::MscnFileManager() {
	isOpen = false;
	file = new std::fstream;
}

MscnFileManager::~MscnFileManager() {
	delete file;
}

bool MscnFileManager::openFile(const std::string& filetPath, std::ios::open_mode openMode) {
	file->open(filetPath, openMode);
	if (openMode == READ_ACCESS)
		if (isOpen)
			return false;
	if (file != nullptr && file->good() == true)
		isOpen = true;
	return isOpen;
}

bool MscnFileManager::readProblemFromFile(double** data, int& problemArrSize) {
	if (!isOpen)
		return false;
	int* dfms = getDFMSFromFile();
	if (dfms == nullptr)
		return false;
	int dQty = dfms[0];
	int fQty = dfms[1];
	int mQty = dfms[2];
	int sQty = dfms[3];
	int arrSize = 4 + dQty + fQty + mQty + sQty + dQty * fQty + fQty * mQty + mQty * sQty + dQty + fQty + mQty + sQty +
		2 * dQty * fQty + 2 * fQty * mQty + 2 * mQty * sQty;
	problemArrSize = arrSize;

	bool correct = readDataFromFile(data, arrSize);
	if (!correct)
		return false;
	double* result = new double[arrSize];
	std::copy(dfms, dfms + 4, result);
	std::copy(*data, *data + arrSize - 4, result + 4);
	*data = result;
}

bool MscnFileManager::readSolutionFromFile(double** data, int& solutionArrSize) {
	if (!isOpen)
		return false;
	int* dfms = getDFMSFromFile();
	if (dfms == nullptr)
		return false;
	int arrSize = dfms[0] * dfms[1] + dfms[1] * dfms[2] + dfms[2] * dfms[3];
	solutionArrSize = arrSize;

	bool correct = readDataFromFile(data, arrSize);
	if (!correct)
		return false;
	double* result = new double[arrSize];
	std::copy(dfms, dfms + 4, result);
	std::copy(*data, *data + arrSize - 4, result + 4);
	*data = result;
}

bool MscnFileManager::saveProblemToFile(MscnProblem& problem) {
	if (!isOpen)
		return false;
	int D = problem.getSuppliersQty();;
	int F = problem.getFactoriesQty();;
	int M = problem.getStoreQty();;
	int S = problem.getShopsQty();;

	*file << "D " << D << "\n";
	*file << "F " << F << "\n";
	*file << "M " << M << "\n";
	*file << "S " << S;

	*file << "\nsd\n";
	for (int i = 0; i < D; i++)
		*file << problem.getSuppliersManufacturingCapacity()[i] << " ";
	*file << "\nsf\n";
	for (int i = 0; i < F; i++)
		*file << problem.getFactoriesManufacturingCapacity()[i] << " ";
	*file << "\nsm\n";
	for (int i = 0; i < M; i++)
		*file << problem.getStoreManufacturingCapacity()[i] << " ";
	*file << "\nss\n";
	for (int i = 0; i < S; i++)
		*file << problem.getShopsManufacturingCapacity()[i] << " ";
	*file << "\ncd\n";
	for (int i = 0; i < D; i++) {
		for (int j = 0; j < F; j++)
			*file << problem.getFromSuppliersToFactoriesCost()->get(i, j) << " ";
		*file << "\n";
	}
	*file << "\ncf\n";
	for (int i = 0; i < F; i++) {
		for (int j = 0; j < M; j++)
			*file << problem.getFromFactoriesToStoreCost()->get(i, j) << " ";
		*file << "\n";
	}
	*file << "\ncm\n";
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < S; j++)
			*file << problem.getFromStoreToShopsCost()->get(i, j) << " ";
		*file << "\n";
	}
	*file << "\nud\n";
	for (int i = 0; i < D; i++)
		*file << problem.getOneTimeCostForSupplier()[i] << " ";
	*file << "\nuf\n";
	for (int i = 0; i < F; i++)
		*file << problem.getOneTimeCostForFactories()[i] << " ";
	*file << "\num\n";
	for (int i = 0; i < M; i++)
		*file << problem.getOneTimeCostForStore()[i] << " ";
	*file << "\np\n";
	for (int i = 0; i < S; i++)
		*file << problem.getProfitFromOnePieceInShops()[i] << " ";
	*file << "\nxdminmax\n";
	for (int i = 0; i < D; i++) {
		for (int j = 0; j < F; j++) {
			*file << problem.getQtyOfFeedstockDeliverBySuppliersMin()->get(i, j) << " ";
			*file << problem.getQtyOfFeedstockDeliverBySuppliersMax()->get(i, j) << " ";
		}
		*file << "\n";
	}
	*file << "\nxfminmax\n";
	for (int i = 0; i < F; i++) {
		for (int j = 0; j < M; j++) {
			*file << problem.getQtyOfProductDeliverByFactoriesMin()->get(i, j) << " ";
			*file << problem.getQtyOfProductDeliverByFactoriesMax()->get(i, j) << " ";
		}
		*file << "\n";
	}
	*file << "\nxmminmax\n";
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < S; j++) {
			*file << problem.getQtyOfProductDeliverByStoreMin()->get(i, j) << " ";
			*file << problem.getQtyOfProductDeliverByStoreMax()->get(i, j) << " ";
		}
		*file << "\n";
	}
}

bool MscnFileManager::saveSolutionToFile(MscnProblem& problem) {
	if (!isOpen)
		return false;
	int D = problem.getSuppliersQty();;
	int F = problem.getFactoriesQty();;
	int M = problem.getStoreQty();;
	int S = problem.getShopsQty();;

	*file << "D " << D << "\n";
	*file << "F " << F << "\n";
	*file << "M " << M << "\n";
	*file << "S " << S;

	*file << "\nxd\n";
	for (int i = 0; i < D; i++) {
		for (int j = 0; j < F; j++)
			*file << problem.getQtyOfFeedstockDeliverBySuppliers()->get(i, j) << " ";
		*file << "\n";
	}
	*file << "\nxf\n";
	for (int i = 0; i < F; i++) {
		for (int j = 0; j < M; j++)
			*file << problem.getQtyOfProductDeliverByFactories()->get(i, j) << " ";
		*file << "\n";
	}
	*file << "\nxm\n";
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < S; j++)
			*file << problem.getQtyOfProductDeliverByStore()->get(i, j) << " ";
		*file << "\n";
	}
}

int* MscnFileManager::getDFMSFromFile() {
	std::string val;
	int counter = 0; //for D,F,M,S values
	int* dfms = new int[4];
	while (*file >> val && counter < 4) {
		double x;
		try {
			x = std::stod(val);
		}
		catch (...) {
			continue;
		}
		dfms[counter] = x;
		counter++;
	}
	if (counter != 4)
		return nullptr;
	return dfms;
}

bool MscnFileManager::readDataFromFile(double** data, int dataArrSize) {
	if (!isOpen)
		return false;
	std::string val;

	int counter = 4;
	double* values = new double[dataArrSize];
	counter = 0;
	while (*file >> val) {
		double x;
		try {
			x = std::stod(val);
		}
		catch (...) {
			continue;
		}
		values[counter] = x;
		counter++;
	}
	*data = values;
	return (counter == dataArrSize - 4);
}

bool MscnFileManager::closeFile() {
	if (isOpen) {
		file->close();
		isOpen = false;
		return true;
	}
	return false;
}
