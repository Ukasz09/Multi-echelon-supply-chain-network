#pragma once
#include "Matrix.h"
#include "../MscnProblem.h"

class MscnFileManager {
public:
	static const std::ios::open_mode WRITE_ACCESS = std::ios::out;
	static const std::ios::open_mode READ_ACCESS = std::ios::in;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	std::fstream* file;
	bool isOpen;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	MscnFileManager();
	~MscnFileManager();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	bool openFile(const std::string& filetPath, std::ios::open_mode operation);
	bool readProblemFromFile(double** data, int& problemArrSize);
	bool readSolutionFromFile(double** data, int& solutionArrSize);
	bool saveProblemToFile(MscnProblem& problem);
	bool saveSolutionToFile(MscnProblem& problem);
	bool closeFile();
private:
	bool readDataFromFile(double** data, int dataArrSize);
	int* getDFMSFromFile();
};
