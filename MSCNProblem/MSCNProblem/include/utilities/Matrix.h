#pragma once
#include "SmartPointer.h"
#include <iostream>

//for efficient 2d matrix in one array
template <class T>
class Matrix {
private:
	static const int DEFAULT_WIDTH = 2;
	static const int DEFAULT_HEIGHT = 2;

	MySmartPointer<T>* matrix;
	int rows;
	int columns;
	int size;
public:
	Matrix();
	Matrix(const int columns, const int rows);
	~Matrix();
public:
	bool set(T value, int row, int column);
	T get(int row, int column);
	int getWidth() const;
	int getHeight() const;
	int getSize() const;
	bool atLeastOneBiggerValThan0InRow(int rowIndex);
	T sumInTheRow(int rowIndex);
	T sumInColumn(int columnIndex);
	void print();
private:
	void makeMatrix(int columns, int rows);
	int getIndexInArr(int row, int column) const;
	bool indexIsCorrect(int index) const;
};

template <class T>
Matrix<T>::Matrix() {
	makeMatrix(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

template <class T>
Matrix<T>::Matrix(const int columns, const int rows) {
	if (columns < 0 || rows < 0)
		makeMatrix(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	else makeMatrix(columns, rows);
}

template <class T>
Matrix<T>::~Matrix() {
	delete matrix;
}

template <class T>
void Matrix<T>::makeMatrix(int columns, int rows) {
	this->rows = rows;
	this->columns = columns;
	this->size = columns * rows;
	if (this->matrix != nullptr)
		delete matrix;
	this->matrix = new MySmartPointer<T>(new T[size], true);
}

template <class T>
bool Matrix<T>::set(T value, const int row, const int column) {
	int index = getIndexInArr(row, column);
	if (indexIsCorrect(index)) {
		matrix->operator[](index) = value;
		return true;
	}
	return false;
}

template <class T>
T Matrix<T>::get(int row, int column) {
	return matrix->operator[](getIndexInArr(row, column));
}

template <class T>
int Matrix<T>::getWidth() const { return columns; }

template <class T>
int Matrix<T>::getHeight() const { return rows; }

template <class T>
int Matrix<T>::getSize() const { return size; }

template <class T>
int Matrix<T>::getIndexInArr(const int row, const int column) const {
	return row * columns + column;
}

template <class T>
bool Matrix<T>::indexIsCorrect(const int index) const {
	if (index >= size)
		return false;
	return true;
}

template <class T>
bool Matrix<T>::atLeastOneBiggerValThan0InRow(const int rowIndex) {
	if (rowIndex >= rows)
		return false;
	for (int i = 0; i < columns; i++)
		if (matrix->at(getIndexInArr(rowIndex, i)) > 0)
			return true;
	return false;
}

template <class T>
T Matrix<T>::sumInTheRow(const int rowIndex) {
	T result = 0;
	for (int i = 0; i < columns; i++)
		result += matrix->at(getIndexInArr(rowIndex, i));
	return result;
}

template <class T>
T Matrix<T>::sumInColumn(int columnIndex) {
	T result = 0;
	for (int i = 0; i < rows; i++)
		result += matrix->at(getIndexInArr(i, columnIndex));
	return result;
}

template <class T>
void Matrix<T>::print() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++)
			std::cout << matrix->at(getIndexInArr(i, j)) << ", ";
		std::cout << "\n";
	}
}
