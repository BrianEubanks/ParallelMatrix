#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "matrix.h"

#define RUN_COUNT   10

#define ROW_SIZE    1024

#define LU_PAR      0

//#define DEBUG_PRINT


struct args{
    int i;
    int k;
    int size;
    double** Arr;
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

    int row = ROW_SIZE;
    int col = row;
    
    double** matrix;
    
    struct timespec start_lu, finish_lu;
    
    double elapsedlu[RUN_COUNT];
    
    for (int run = 0; run < RUN_COUNT; run++){
        
        //elapsedlu[run] = 1;
        matrix = getMatrix(row,col);
        // L = getMatrix(row,col);
        //U = getMatrix(row,col);
        //C = getMatrix(row,col);

        // row = 3
        //initMatrixTest1(matrix,row,col);
        
        // row = 4
        //initMatrixTest2(matrix,row,col);
        
        initMatrix(matrix,row,col);
        
#ifdef DEBUG_PRINT
        printf("\n");
        printMatrix(matrix,row,col);
#endif
        
        //printf("start lu\n");
        clock_gettime(CLOCK_MONOTONIC, &start_lu);
        if(LU_PAR){
            GaussianPar(matrix,row);
        } else {
            Gaussian(matrix,row);
        }
        clock_gettime(CLOCK_MONOTONIC, &finish_lu);
        //printf("end lu\n");
        
#ifdef DEBUG_PRINT
        printf("\n");
        printMatrix(matrix,row,col);
#endif
        
        
        


        elapsedlu[run] = (finish_lu.tv_sec - start_lu.tv_sec);
        elapsedlu[run] += (finish_lu.tv_nsec - start_lu.tv_nsec) / 1000000000.0;
                
        //printf("\n");
        
        freeMatrix(matrix,row);

        
    }
    
    printf("Parallel LU , %d\n", LU_PAR);
    printf("ROW_SIZE, %d\n", ROW_SIZE);
    printf("Run , LU_TIME\n");
    
    for (int r = 0; r < RUN_COUNT; r++){
        printf("%d , %f \n", r, elapsedlu[r]);
    }
    
    printf("Run , LU_TIME\n");

    
    return 0;
}
