#include <stdio.h>


void printArr2d(double G[][3], int size);

double randDouble();

void initMatrix(double** arr, int row, int col);

void initMatrixTest1(double** matrix, int row, int col);

void initMatrixTest2(double** matrix, int row, int col);

void matrixMult(double**a, double** b, double** c, int n);

double** getMatrix(int row, int col);

void freeMatrix(double** arr, int row);

void printMatrix(double** arr, int row, int col);
