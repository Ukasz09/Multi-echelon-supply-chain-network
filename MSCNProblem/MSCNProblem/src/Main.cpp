#include  <iostream>
#include "../include/MscnProblem.h"
#include "../include/utilities/RandomGenerator.h"
#include "../include/utilities/MscnFileManager.h"
#include "../include/solutionSearch/randomSearch/RandomSearch.h"
#include "../include/solutionSearch/diffEvolution/DiffEvolution.h"

MscnProblem* getTestMscProblemInstance(int dQty, int fQty, int mQty, int sQty, unsigned int randomSeed) {
	MscnProblem* mscnProblem = new MscnProblem();
	mscnProblem->setDFMSqty(dQty, fQty, mQty, sQty);
	mscnProblem->setMinMax();
	mscnProblem->generateInstance(randomSeed);
	return mscnProblem;
}

bool saveSolutionToFile(MscnProblem& mscnProblem, std::string& filePath) {
	MscnFileManager problemFileHelper;
	problemFileHelper.openFile(filePath, MscnFileManager::WRITE_ACCESS);
	const bool openCorrect = problemFileHelper.saveSolutionToFile(mscnProblem);
	problemFileHelper.closeFile();
	return openCorrect;
}

bool saveProblemToFile(MscnProblem& mscnProblem, std::string& filePath) {
	MscnFileManager problemFileHelper;
	problemFileHelper.openFile(filePath, MscnFileManager::WRITE_ACCESS);
	const bool openCorrect = problemFileHelper.saveProblemToFile(mscnProblem);
	problemFileHelper.closeFile();
	return openCorrect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void eportalManualTest() {
	MscnProblem problem;
	const int testDCount = 1;
	const int testFCount = 1;
	const int testMCount = 1;
	const int testSCount = 1;
	problem.setDQty(testDCount);
	problem.setFQty(testFCount);
	problem.setMQty(testMCount);
	problem.setSQty(testSCount);

	problem.setInSd(100, 0);
	problem.setInSf(50, 0);
	problem.setInSm(10.5, 0);
	problem.setInSs(100, 0);

	problem.setInCd(3, 0, 0);
	problem.setInCf(4, 0, 0);
	problem.setInCm(1, 0, 0);

	problem.setInUd(10, 0);
	problem.setInUf(10, 0);
	problem.setInUm(10, 0);
	problem.setInPs(100, 0);
	double* arr = new double[3]{1, 1, 1};
	const MySmartPointer<double> solution(arr, true);

	MscnProblem::errorCodes errors;
	std::cout << "RESULT: " << problem.getQuality(solution.get(), 3, errors) << "\n";
	std::cout << "SATISFIED: " << problem.constraintsSatisfied(solution.get(), 3, errors) << '\n';
}

void eportalTestReadingFromFile() {
	MscnProblem::errorCodes errors;
	MscnFileManager problemFileReader;
	problemFileReader.openFile("eportalProblem.txt", MscnFileManager::READ_ACCESS);
	double* problemData = nullptr;
	int problemArrSize;
	problemFileReader.readProblemFromFile(&problemData, problemArrSize);
	problemFileReader.closeFile();

	MscnProblem problem(problemData, problemArrSize, errors);

	MscnFileManager solutionFileReader;
	solutionFileReader.openFile("eportalRozwiazanie.txt", MscnFileManager::READ_ACCESS);
	double* solutionData = nullptr;
	int solutionArrSize;
	solutionFileReader.readSolutionFromFile(&solutionData, solutionArrSize);
	solutionFileReader.closeFile();

	std::cout << "RESULT: " << problem.getQuality(solutionData, solutionArrSize, errors) << "\n";
	std::cout << "SATISFIED: " << problem.constraintsSatisfied(solutionData, solutionArrSize, errors) << '\n';

	std::cout << "MIN: \n";
	std::vector<double> min = problem.minValuesRange();
	for (int i = 0; i < min.size(); i++)
		std::cout << min[i] << ",";

	std::cout << "\n\nMAX: \n";
	std::vector<double> max = problem.maxValuesRange();
	for (int i = 0; i < min.size(); i++)
		std::cout << max[i] << ",";

	delete[] solutionData;
	delete[] problemData;
}

void test1Manual() {
	MscnProblem problem;
	const int testDCount = 3;
	const int testFCount = 2;
	const int testMCount = 3;
	const int testSCount = 4;
	problem.setDQty(testDCount);
	problem.setFQty(testFCount);
	problem.setMQty(testMCount);
	problem.setSQty(testSCount);

	problem.setInSd(10, 0);
	problem.setInSd(23, 1);
	problem.setInSd(12, 2);

	problem.setInSf(9, 0);
	problem.setInSf(14, 1);

	problem.setInSm(10, 0);
	problem.setInSm(13, 1);
	problem.setInSm(21, 2);

	problem.setInSs(17, 0);
	problem.setInSs(21, 1);
	problem.setInSs(31, 2);
	problem.setInSs(24, 3);

	int q = 0;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 2; j++) {
			problem.setInCd(q, i, j);
			q++;
		}

	int qq = 0;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 3; j++) {
			problem.setInCf(qq, i, j);
			qq++;
		}

	int k = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			problem.setInCm(k, i, j);
			k++;
		}
	}

	problem.setInUd(10, 0);
	problem.setInUd(20, 1);
	problem.setInUd(30, 2);

	problem.setInUf(30, 0);
	problem.setInUf(40, 1);

	problem.setInUm(60, 0);
	problem.setInUm(70, 1);
	problem.setInUm(80, 2);

	problem.setInPs(100, 0);
	problem.setInPs(110, 1);
	problem.setInPs(120, 2);
	problem.setInPs(130, 3);
	MscnProblem::errorCodes err;

	double* arr = new double[24]{
		0, 1, 2, 3, 8, 7,
		9, 11, 22, 13, 14, 25,
		4, 1, 2, 3, 4, 5,
		6, 7, 8, 9, 10, 11,
	};

	MySmartPointer<double> solution(arr, true);
	std::cout << "RESULT: " << problem.getQuality(solution.get(), 24, err) << '\n';
	std::cout << "SATISFIED: " << problem.constraintsSatisfied(solution.get(), 24, err) << '\n';
}

void test1ReadingFromFile() {
	MscnProblem::errorCodes errors;
	MscnFileManager problemFileReader;
	problemFileReader.openFile("test1Problem.txt", MscnFileManager::READ_ACCESS);
	double* problemData = nullptr;
	int problemArrSize;
	problemFileReader.readProblemFromFile(&problemData, problemArrSize);
	problemFileReader.closeFile();

	MscnProblem problem(problemData, problemArrSize, errors);

	MscnFileManager solutionFileReader;
	solutionFileReader.openFile("test1Rozwiazanie.txt", MscnFileManager::READ_ACCESS);
	double* solutionData = nullptr;
	int solutionArrSize;
	solutionFileReader.readSolutionFromFile(&solutionData, solutionArrSize);
	solutionFileReader.closeFile();

	std::cout << "\nRESULT: " << problem.getQuality(solutionData, solutionArrSize, errors) << "\n";
	std::cout << "SATISFIED: " << problem.constraintsSatisfied(solutionData, solutionArrSize, errors) << '\n';

	delete[] solutionData;
}

void test3ReadingAndWriting() {
	MscnProblem::errorCodes errors;
	MscnFileManager problemFileReader;
	problemFileReader.openFile("test3Problem.txt", MscnFileManager::READ_ACCESS);
	double* problemData = nullptr;
	int problemArrSize;
	problemFileReader.readProblemFromFile(&problemData, problemArrSize);
	problemFileReader.closeFile();

	MscnProblem problem(problemData, problemArrSize, errors);

	MscnFileManager solutionFileHelper;
	solutionFileHelper.openFile("test3Rozwiazanie.txt", MscnFileManager::READ_ACCESS);
	double* solutionData = nullptr;
	int solutionArrSize;
	solutionFileHelper.readSolutionFromFile(&solutionData, solutionArrSize);
	solutionFileHelper.closeFile();

	std::cout << "RESULT: " << problem.getQuality(solutionData, solutionArrSize, errors) << "\n";
	std::cout << "SATISFIED: " << problem.constraintsSatisfied(solutionData, solutionArrSize, errors) << '\n';

	solutionFileHelper.openFile("test3Problem_save.txt", MscnFileManager::WRITE_ACCESS);
	std::cout << "SAVED_PROBLEM: " << solutionFileHelper.saveProblemToFile(problem) << "\n";
	solutionFileHelper.closeFile();

	solutionFileHelper.openFile("test3Rozwiazanie_save.txt", MscnFileManager::WRITE_ACCESS);
	std::cout << "SAVED_SOLUTION: " << solutionFileHelper.saveSolutionToFile(problem);
	solutionFileHelper.closeFile();

	delete[] solutionData;
}

//list 10 and list 11
void randomTest(int seed) {
	RandomGenerator<int> random(seed);
	std::cout << "\n\nRandom int (10-20): ";
	for (int i = 0; i < 20; i++)
		std::cout << random.getRandom(10, 20) << ", ";

	std::cout << "\n\nRandom int (0-3): ";
	for (int i = 0; i < 20; i++)
		std::cout << random.getRandom(0, 3) << ", ";

	std::cout << "\n\nRandom float (0-2): ";
	RandomGenerator<float> random2(seed);
	for (int i = 0; i < 20; i++)
		std::cout << random2.getRandom(0, 2) << ",";


	std::cout << "\n\nRandom float (15-100): ";
	for (int i = 0; i < 20; i++)
		std::cout << random2.getRandom(15, 100) << ",";

	std::cout << "\n\nRandom int (8-10): ";
	for (int i = 0; i < 20; i++)
		std::cout << random.getRandom(8, 10) << ",";

	std::cout << "\n\nRandom ((-5) - 10): ";
	for (int i = 0; i < 20; i++)
		std::cout << random.getRandom(-5, 10) << ",";
}

void problemGeneratorTest(unsigned int seed) {
	MscnProblem* mscnProblem = getTestMscProblemInstance(2, 3, 4, 5, seed);
	std::string saveFilePath = "problemGeneratorTest.txt";
	std::cout << "Saved without errors: " << saveProblemToFile(*mscnProblem, saveFilePath) << "\n";
}

void randomSearchTest(int dQty, int fQty, int mQty, int sQty, unsigned int seed, int stopCounterValue) {
	MscnProblem* mscnProblem = getTestMscProblemInstance(dQty, fQty, mQty, sQty, seed);
	RandomSearch randomSearch(mscnProblem, stopCounterValue);
	double* bestSolution = randomSearch.findSolution();
	MscnProblem::errorCodes errCode;
	double profit = mscnProblem->getQuality(bestSolution, mscnProblem->getSolutionSize(), errCode);
	//todo: zmenic by nie potrzebowal solution size 
	std::cout << "Zysk: " << profit << "\n";

	std::string saveFilePath = "RandomSearchSolution.txt";
	std::cout << "Saved without errors: " << saveSolutionToFile(*mscnProblem, saveFilePath) << "\n";
	delete bestSolution;
	delete mscnProblem;
}

void differentialEvolutionTest(int dQty, int fQty, int mQty, int sQty, unsigned int seed, int stopCounterValue) {
	MscnProblem* mscnProblem = getTestMscProblemInstance(dQty, fQty, mQty, sQty, seed);
	DiffEvolution diffEvolution(mscnProblem, stopCounterValue);
	MscnProblem::errorCodes errCode;
	double* bestSolution = diffEvolution.findBestSolution();
	const double profit = mscnProblem->getQuality(bestSolution, mscnProblem->getSolutionSize(), errCode);

	std::cout << "\nResult: " << profit << "\n";
	std::string saveFilePath = "DESearchSolution.txt";
	std::cout << "Saved without errors: " << saveSolutionToFile(*mscnProblem, saveFilePath);

	delete bestSolution;
	delete mscnProblem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
	// //older lists tests
	//  eportalManualTest();
	//  eportalTestReadingFromFile();
	//  test1ReadingFromFile();
	//  test3ReadingAndWriting();
	//  randomTest(1234);
	//
	// // list 10 test
	//  problemGeneratorTest(1234);

	// list 10, 11 tests
	randomSearchTest(2, 3, 4, 5, 1234, 50);
	differentialEvolutionTest(2, 3, 4, 5, 1234, 50);

	std::cin.get();
	std::cin.get();
	return 0;
}
