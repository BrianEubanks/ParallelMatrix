#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "matrix.h"
 

struct croutargs{
    int i;
    int k;
    int size;
    double** A;
    double** L;
    double** U;
};

int *crout_L_fn(void *ptr){
    int i;
    int j;
    int k;
    double** A;
    double** L;
    double** U;
    int n;
    int sum;
    
    i = ((struct croutargs*) ptr)->i;
    //j = ((struct croutargs*) ptr)->j;
    n = ((struct croutargs*) ptr)->size;
    A = ((struct croutargs*) ptr)->A;
    L = ((struct croutargs*) ptr)->L;
    U = ((struct croutargs*) ptr)->U;

    for (int j = 0; j < n; j++)
    {

        if (j < i) {
            L[j][i] = 0;
            continue;
        }

        L[j][i] = A[j][i];
        for (int k = 0; k < i; k++) {
            L[j][i] = L[j][i] - L[j][k] * U[k][i];
        }
    }
    
    return 0;
}

int *crout_U_fn(void *ptr){
    int i;
    int j;
    int k;
    double** A;
    double** L;
    double** U;
    int n;
    int sum;

    i = ((struct croutargs*) ptr)->i;
    //j = ((struct croutargs*) ptr)->j;
    n = ((struct croutargs*) ptr)->size;
    A = ((struct croutargs*) ptr)->A;
    L = ((struct croutargs*) ptr)->L;
    U = ((struct croutargs*) ptr)->U;

    for (int j = 0; j < n; j++) {
        if (j < i) {
            U[i][j] = 0;
            continue;
        }
        
        if (j == i) {
            U[i][j] = 1;
            continue;
        }
        
        U[i][j] = A[i][j] / L[i][i];
        for (int k = 0; k < i; k++) {
            U[i][j] = U[i][j] - ((L[i][k] * U[k][j]) / L[i][i]);
        }
    }
    return 0;
    
}


void GaussianCompLU(double** A, double** L, double** U, int row) {
    int i;
    int j;
    int k;
    int n = row;
    
    printf("Sequential\n");
    
    for (int i = 0; i < n; i++) {
 
        for (int j = 0; j < n; j++) {
            
            if (j < i){
                continue;
            }
            
            L[j][i] = A[j][i];

            
            for(int k = 0; k < i; k++) {
                L[j][i] = L[j][i] - L[j][k] * U[k][i];
            }
            
        }
        
        for (int j = 0; j < n; j++) {
            
            if (j<i){
                continue;
            }
            if (j==i){
                U[i][j]=1;
            }

            U[i][j] = A[i][j] / L[i][i];
            for(int k = 0; k < i; k++) {
                U[i][j] = U[i][j] - ((L[i][k] * U[k][j])/L[i][i]);
            }
        }
    }
}


void GaussianCompLU2(double** a, double** l, double** u, int size) {

    struct croutargs crout_data;
    
    crout_data.size = size;
    crout_data.A = a;
    crout_data.L = l;
    crout_data.U = u;
    
    pthread_t t_crout_L;
    pthread_t t_crout_U;
    
    printf("Parallel\n");
    
    for (int i = 0; i < size; i++) {
        crout_data.i = i;
        
        //L
        pthread_create(&t_crout_L,NULL,(void*)crout_L_fn,(void*)&crout_data);
        
        //U
        pthread_create(&t_crout_U,NULL,(void*)crout_U_fn,(void*)&crout_data);
  
        pthread_join(t_crout_L,NULL);
        pthread_join(t_crout_U,NULL);
        
    }
}


int main() {

    int row = 4000;
    int col = row;
    
    double** matrix;
    double** C;
    double** L;
    double** U;
    
    clock_t begin;
    clock_t end;

    matrix = getMatrix(row,col);
    L = getMatrix(row,col);
    U = getMatrix(row,col);
    C = getMatrix(row,col);


    // row = 3
    //initMatrixTest1(matrix,row,col);

    // row = 4
    //initMatrixTest2(matrix,row,col);
    
    initMatrix(matrix,row,col);

    printf("start\n");
    begin = clock();

    //GaussianCompLU2(matrix,L,U,row);
    GaussianCompLU(matrix,L,U,row);

    end = clock();
    printf("end\n");

    //printMatrix(matrix,row,col);
    //printMatrix(L,row,col);
    //printMatrix(U,row,col);
    //printMatrix(C,row,col);
    
    //matrixMult(L,U,C,row);
    //printf("\n");
    //printMatrix(C,row,col);

 
    freeMatrix(matrix,row);
    freeMatrix(L,row);
    freeMatrix(U,row);
    freeMatrix(C,row);


    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("%f\n",time_spent);
 
    return 0;
}
