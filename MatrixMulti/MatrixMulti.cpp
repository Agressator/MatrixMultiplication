﻿#include "pch.h"
#include <iostream>
#include <string>
#include <omp.h>

using namespace std;

int** GenerateMatrix(unsigned int rowNum, unsigned int columnNum)
{
	int** matrix = new int*[rowNum];
	for (int i = 0; i < rowNum; i++)
		matrix[i] = new int[columnNum];

	for (int i = 0; i < rowNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			matrix[i][j] = rand() % 10 + 1;
			//cout << matrix[i][j] << " ";
		}
		//cout << endl;
	}
	cout << endl;

	return matrix;
}

void MultiplyWithOutOptimization(int** aMatrix, unsigned int aRowNum, unsigned int aColumnNum, 
								 int** bMatrix, unsigned int bColumnNum)
{	
	int** product = new int* [aRowNum];
	for (int i = 0; i < aRowNum; i++)
		product[i] = new int[bColumnNum];

	for (int i = 0; i < aRowNum; i++)
		for (int j = 0; j < bColumnNum; j++)
			product[i][j] = 0;

	for (int row = 0; row < aRowNum; row++) {
		for (int col = 0; col < bColumnNum; col++) {
			for (int inner = 0; inner < aColumnNum; inner++) {
				product[row][col] += aMatrix[row][inner] * bMatrix[inner][col];
			}
			//cout << product[row][col] << "  ";
		}
		//cout << "\n";
	}
}

void Multiply(int** aMatrix, unsigned int aRowNum, unsigned int aColumnNum,
			  int** bMatrix, unsigned int bColumnNum)
{
	int* product = new int[aRowNum * bColumnNum];
	int* column = new int[aColumnNum];
#pragma omp parallel for
	for (int j = 0; j < bColumnNum; j++)
	{
		for (int k = 0; k < aColumnNum; k++)
			column[k] = bMatrix[k][j];
		for (int i = 0; i < aRowNum; i++)
		{
			int* row = aMatrix[i];
			double summand = 0;
			for (int k = 0; k < aColumnNum; k++)
				summand += row[k] * column[k];
			product[i * aRowNum + j] = summand;
			//cout << product[i * aRowNum + j] << "  ";
		}
		//cout << "\n";
	}
}

int main()
{
	const int aRowNum = 500;
	const int aColumnNum = 500;
	const int bColumnNum = 500;
	int** aMatrix = GenerateMatrix(aRowNum, aColumnNum);
	int** bMatrix = GenerateMatrix(aColumnNum, bColumnNum);
	double start, end;

	start = omp_get_wtime();
	MultiplyWithOutOptimization(aMatrix, aRowNum, aColumnNum, bMatrix, bColumnNum);
	end = omp_get_wtime();
	cout << "Work took " << end - start << "sec. time.\n";

	start = omp_get_wtime();
	Multiply(aMatrix, aRowNum, aColumnNum, bMatrix, bColumnNum);
	end = omp_get_wtime();
	cout << "Work with optimizations took " << end - start << "sec. time.\n";

	getchar();
}
