#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "matrix.h"


struct args{
    int i;
    int k;
    int size;
    double** Arr;
};

struct croutargs{
    int i;
    int k;
    int size;
    double** A;
    double** L;
    double** U;
};

int *saxpy_fn(void *ptr){
    int i;
    int k;
    double** Arr;
    int n;

    i = ((struct args*) ptr)->i;
    k = ((struct args*) ptr)->k;
    n = ((struct args*) ptr)->size;
    Arr = ((struct args*) ptr)->Arr;

    for (int j = k+1; j < n; j++) {
        Arr[i][j] = Arr[i][j] - Arr[i][k] * Arr[k][j];
    }
    return 0;
}

int *crout_L_fn(void *ptr){
    int i;
    int j;
    int k;
    double** A;
    double** L;
    double** U;
    int n;
    int sum;
    
    i = ((struct args*) ptr)->i;
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

    i = ((struct args*) ptr)->i;
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

void Gaussian(double** A, int row) {
    int i;
    int j;
    int k;
    int n = row;
    for (int k = 0; k < n; k++) {
        for(int i = k+1; i < n; i++) {
            A[i][k] = A[i][k] / A[k][k];
            for (int j = k+1; j < n; j++) {
                A[i][j] = A[i][j] - A[i][k] * A[k][j];
            }
        }
    }
}

void GaussianComp(double** A, int row) {
    int i;
    int j;
    int k;
    int n = row;
    
    for (int i = 0; i < n; i++) {
        
        for(int k = 0; k < i; k++) {
            for (int j = i; j < n; j++) {
                A[i][j] = A[i][j] - A[i][k] * A[k][j];
            }
        }
        
        for(int k = 0; k < i; k++) {
            for (int j = 0; j < n; j++) {
                A[j][i] = A[j][i] - A[j][k] * A[k][i];
            }
        }
        
        for (int j = 0; j < n; j++) {
            A[j][i] = A[j][i] - A[j][k] * A[k][i];
            //A[i][i] = A[j][i] / A[i][i];
        }
       
    }
}

void GaussianPar(double** A, int row) {
    int i;
    int j;
    int k;
    int n = row;

    pthread_t threads[n];

    struct args saxpy_data[n];

    for (int k = 0; k < n; k++) {
        for(int i = k+1; i < n; i++) {
            A[i][k] = A[i][k] / A[k][k];
            saxpy_data[i].i=i;
            saxpy_data[i].k=k;
            saxpy_data[i].size=row;
            saxpy_data[i].Arr=A;

            pthread_create(&threads[i],NULL,(void*)saxpy_fn,(void*)&saxpy_data[i]);

        }
        for(int i = k+1; i < n; i++) {

            pthread_join(threads[i],NULL);

        }
    }
}


int main() {

    int row = 1000;
    int col = row;
    
    double** matrix;
    double** C;
    double** L;
    double** U;
    
    clock_t begin;
    clock_t end;
    clock_t begin_mult;
    clock_t end_mult;
    clock_t begin_tot;
    clock_t end_tot;
    struct timespec start_mult, finish_mult;
    struct timespec start_lu, finish_lu;
    double elapsed;
    double elapsedlu;

    

    matrix = getMatrix(row,col);
    L = getMatrix(row,col);
    U = getMatrix(row,col);
    C = getMatrix(row,col);
    


    // row = 3
    //initMatrixTest1(matrix,row,col);

    // row = 4
    //initMatrixTest2(matrix,row,col);
    
    initMatrix(matrix,row,col);
    
    //printMatrix(matrix,row,col);

    printf("start\n");
    clock_gettime(CLOCK_MONOTONIC, &start_lu);
    begin_tot = clock();
    begin = clock();

    //Gaussian(matrix,row);
    GaussianPar(matrix,row);

    end = clock();
    clock_gettime(CLOCK_MONOTONIC, &finish_lu);

    printf("end\n");

    //printMatrix(matrix,row,col);
    //printMatrix(L,row,col);
    //printMatrix(U,row,col);
    //printMatrix(C,row,col);
    
    
    printf("start_mult\n");
    begin_mult = clock();
    clock_gettime(CLOCK_MONOTONIC, &start_mult);
    matrixMult(L,U,C,row);
    //matrixMultPar(L,U,C,row);
    end_mult = clock();
    end_tot = clock();
    clock_gettime(CLOCK_MONOTONIC, &finish_mult);
    
    printf("end_mult\n");

    
    printf("\n");
    //printMatrix(matrix,row,col);
    
    printf("Equal: %d\n",equalsMatrix(matrix,C,row));

 
    freeMatrix(matrix,row);
    freeMatrix(L,row);
    freeMatrix(U,row);
    freeMatrix(C,row);


    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    double time_spent_mult = (double)(end_mult - begin_mult) / CLOCKS_PER_SEC;
    double time_spent_tot = (double)(end_tot - begin_tot) / CLOCKS_PER_SEC;
    
    elapsed = (finish_mult.tv_sec - start_mult.tv_sec);
    elapsed += (finish_mult.tv_nsec - start_mult.tv_nsec) / 1000000000.0;
    
    elapsedlu = (finish_lu.tv_sec - start_lu.tv_sec);
    elapsedlu += (finish_lu.tv_nsec - start_lu.tv_nsec) / 1000000000.0;


    //printf("LU: %f\n",time_spent);
    //printf("Mult: %f\n",time_spent_mult);
    //printf("Tot: %f\n",time_spent_tot);
    printf("elapLU: %f\n",elapsedlu);

    printf("elapMult: %f\n",elapsed);

    
    return 0;
}
