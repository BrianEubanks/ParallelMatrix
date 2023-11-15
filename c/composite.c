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

int *gausscomp_L_fn(void *ptr){
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

int *gausscomp_U_fn(void *ptr){
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
    return 0;
    
}

int *crout_L_fn(void *ptr){
    int i;
    int j;
    int k;
    double** A;
    double** l;
    double** u;
    int n;
    int sum;
    
    i = ((struct croutargs*) ptr)->i;
    //j = ((struct croutargs*) ptr)->j;
    n = ((struct croutargs*) ptr)->size;
    A = ((struct croutargs*) ptr)->A;
    l = ((struct croutargs*) ptr)->L;
    u = ((struct croutargs*) ptr)->U;

    for (int j = 0; j < n; j++) {
        
        if (j == i){
            l[i][i] = 1.0;
        }
        
        if (j > i) {
            l[j][i] = A[j][i];
            for(int k = 0; k < i; k++)
                l[j][i] = l[j][i] - u[k][i] * l[j][k];
            l[j][i] = l[j][i] / u[i][i];
        }
    }
    
    return 0;
}

int *crout_U_fn(void *ptr){
    int i;
    int j;
    int k;
    double** A;
    double** l;
    double** u;
    int n;
    int sum;

    i = ((struct croutargs*) ptr)->i;
    //j = ((struct croutargs*) ptr)->j;
    n = ((struct croutargs*) ptr)->size;
    A = ((struct croutargs*) ptr)->A;
    l = ((struct croutargs*) ptr)->L;
    u = ((struct croutargs*) ptr)->U;

    
    
    for (int j = 0; j < n; j++) {
        
        if (j >= i) {
            u[i][j] = A[i][j];
            for(int k = 0; k < i; k++)
                u[i][j] = u[i][j] - u[k][j] * l[i][k];
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


void GaussianCompLUPar(double** a, double** l, double** u, int size) {

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
        pthread_create(&t_crout_L,NULL,(void*)gausscomp_L_fn,(void*)&crout_data);
        
        //U
        pthread_create(&t_crout_U,NULL,(void*)gausscomp_U_fn,(void*)&crout_data);
  
        pthread_join(t_crout_L,NULL);
        pthread_join(t_crout_U,NULL);
        
    }
}


void CroutCompLU(double** A, double** l, double** u, int row) {
    int i;
    int j;
    int k;
    int n = row;
    
    
    printf("CroutSequential\n");
    
    for (int i = 0; i < n; i++) {
        
        for (int j = 0; j < n; j++) {
            
            if (j >= i) {
                u[i][j] = A[i][j];
                for(int k = 0; k < i; k++)
                    u[i][j] = u[i][j] - u[k][j] * l[i][k];
            }
        }
 
        for (int j = 0; j < n; j++) {
            
            if (j == i){
                l[i][i] = 1.0;
            }
            
            if (j > i) {
                l[j][i] = A[j][i];
                for(int k = 0; k < i; k++)
                    l[j][i] = l[j][i] - u[k][i] * l[j][k];
                l[j][i] = l[j][i] / u[i][i];
            }
        }
    }
}

void CroutCompLUPar(double** A, double** l, double** u, int row) {
    int i;
    int j;
    int k;
    int n = row;
    
    struct croutargs crout_data;
    
    crout_data.size = n;
    crout_data.A = A;
    crout_data.L = l;
    crout_data.U = u;
    
    pthread_t t_crout_L;
    pthread_t t_crout_U;
    
    printf("CroutParallel\n");
    
            for (int i = 0; i < n; i++) {
                u[i][i] = 1.0;
            }
    
    for (int i = 0; i < n; i++) {
        
        crout_data.i = i;
        
        
        //U
        pthread_create(&t_crout_U,NULL,(void*)crout_U_fn,(void*)&crout_data);
        
        //L
        pthread_create(&t_crout_L,NULL,(void*)crout_L_fn,(void*)&crout_data);
        
        pthread_join(t_crout_L,NULL);
        pthread_join(t_crout_U,NULL);

        
    }
}


int main() {

    int row = 64;
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
    
    printf("\n");
    printMatrix(matrix,row,col);

    printf("start\n");
    clock_gettime(CLOCK_MONOTONIC, &start_lu);
    begin_tot = clock();
    begin = clock();


    //GaussianCompLU(matrix,L,U,row);
    //GaussianCompLUPar(matrix,L,U,row);
    //CroutCompLU(matrix,L,U,row);
    CroutCompLUPar(matrix,L,U,row);

    end = clock();
    clock_gettime(CLOCK_MONOTONIC, &finish_lu);

    printf("end\n");

    printMatrix(matrix,row,col);
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
    printMatrix(C,row,col);
    
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
