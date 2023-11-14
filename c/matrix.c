#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void printArr2d(double G[][3], int size) {
    //int n = size;
    //System.out.println(str + ": ");
    for(int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%f,",G[i][j]);
        }
        printf("\n ");
    }
}

double randDouble() {
    return 0.1 + (double) rand() / (double)RAND_MAX;
}

void initMatrix(double** arr, int row, int col){
    int i,j;
    time_t t;
    
    srand((unsigned) time(&t));

    for (i = 0; i < row; i++){
        for (j = 0; j < col; j++){
            arr[i][j] = randDouble();
        }
    }
}

void initMatrixTest1(double** matrix, int row, int col){
    matrix[0][0]= 2.0 ;
    matrix[0][1]= -1.0;
    matrix[0][2]= -2.0;
    matrix[1][0]= -4.0;
    matrix[1][1]= 6.0;
    matrix[1][2]= 3.0;
    matrix[2][0]= -4.0;
    matrix[2][1]= -2.0;
    matrix[2][2]= 8.0;
}

void initMatrixTest2(double** matrix, int row, int col){
    matrix[0][0]= 1.0 ;
    matrix[0][1]= -2.0;
    matrix[0][2]= -2.0;
    matrix[0][3]= -3.0;
    matrix[1][0]= 3.0;
    matrix[1][1]= -9.0;
    matrix[1][2]= 0.0;
    matrix[1][3]= -9.0;
    matrix[2][0]= -1.0;
    matrix[2][1]= 2.0;
    matrix[2][2]= 4.0;
    matrix[2][3]= 7.0;
    matrix[3][0]= -3.0;
    matrix[3][1]= -6.0;
    matrix[3][2]= 26.0;
    matrix[3][3]= 2.0;
}

void matrixMult(double**a, double** b, double** c, int n){
    for(int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            for (int k = 0; k < n; k++){
                c[i][j]+=a[i][k]*b[k][j];
            }
        }
    }
}

double** getMatrix(int row, int col){
    int i;

    double** arr = (double**)malloc(row * sizeof(double*));
    for (i = 0; i < row; i++)
        arr[i] = (double*)malloc(col * sizeof(double));

    return arr;
}

void freeMatrix(double** arr, int row) {
    for (int i = 0; i < row; i++)
        free(arr[i]);
 
    free(arr);
}

void printMatrix(double** arr, int row, int col){
    int i,j;

    for (i = 0; i < row; i++){
        for (j = 0; j < col; j++){
            printf("%f ", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
