#include <stdio.h>


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


void Gaussian(double A[][3], int n) {
    //int n = size;
    for (int k = 0; k < n; k++) {
        for(int i = k+1; i < n; i++) {
            A[i][k] = A[i][k] / A[k][k];
            for (int j = k+1; j < n; j++) {
                A[i][j] = A[i][j] - A[i][k] * A[k][j];
            }
        }
    }   
    //printArr2d(A,n);
    //return A;
}

int main(){
    double mat[3][3] = { {2.0,-1.0,-2.0},
                            {-4.0,6.0,3.0},
                            {-4.0,-2.0,8.0}};
    printArr2d(mat,3);

    Gaussian(mat,3);

    printArr2d(mat,3);
}