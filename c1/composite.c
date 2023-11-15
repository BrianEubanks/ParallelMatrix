#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "matrix.h"


#define RUN_COUNT   100

#define ROW_SIZE    2048

#define MULT_PAR    1
#define LU_PAR      0

#define GS_OR_CRT   0

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
    
    //printf("Sequential\n");
    
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
    
    //printf("Parallel\n");
    
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
    
    
    //printf("CroutSequential\n");
    
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
    
    //printf("CroutParallel\n");
    
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

    int row = ROW_SIZE;
    int col = row;
    
    double** matrix;
    double** C;
    double** L;
    double** U;
    
    struct timespec start_mult, finish_mult;
    struct timespec start_lu, finish_lu;
    
    double elapsed[RUN_COUNT];
    double elapsedlu[RUN_COUNT];
    double moe[RUN_COUNT];
    int errorcount = 0;

    




    
    for (int run = 0; run < RUN_COUNT; run++) {
        //printf("Run: %d\n",run);

        
        matrix = getMatrix(row,col);
        L = getMatrix(row,col);
        U = getMatrix(row,col);
        C = getMatrix(row,col);
        
        // row = 3
        //initMatrixTest1(matrix,row,col);
        
        // row = 4
        //initMatrixTest2(matrix,row,col);
        
        initMatrix(matrix,row,col);
        
#ifdef DEBUG_PRINT
        printf("\n");
        printMatrix(matrix,row,col);
#endif
        
        //printf("  start lu\n");
        clock_gettime(CLOCK_MONOTONIC, &start_lu);
        if(GS_OR_CRT){
            if(LU_PAR){
                GaussianCompLUPar(matrix,L,U,row);
            } else {
                GaussianCompLU(matrix,L,U,row);
            }
        } else {
            if (LU_PAR){
                CroutCompLUPar(matrix,L,U,row);
            } else {
                CroutCompLU(matrix,L,U,row);
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &finish_lu);
        //printf("  end lu\n");
        
#ifdef DEBUG_PRINT
        printMatrix(matrix,row,col);
        //printMatrix(L,row,col);
        //printMatrix(U,row,col);
        //printMatrix(C,row,col);
#endif
        
        
        //printf("  start_mult\n");
        clock_gettime(CLOCK_MONOTONIC, &start_mult);
        if(MULT_PAR){
            matrixMultPar(L,U,C,row);
        } else {
            matrixMult(L,U,C,row);
        }
        clock_gettime(CLOCK_MONOTONIC, &finish_mult);
        //printf("  end_mult\n");
        
#ifdef DEBUG_PRINT
        printf("\n");
        printMatrix(C,row,col);
#endif
        
        moe[run] = equalsMatrix(matrix,C,row);
        if (moe[run] != 0){
            errorcount++;
        }
        
        
        elapsed[run] = (finish_mult.tv_sec - start_mult.tv_sec);
        elapsed[run] += (finish_mult.tv_nsec - start_mult.tv_nsec) / 1000000000.0;
        
        elapsedlu[run] = (finish_lu.tv_sec - start_lu.tv_sec);
        elapsedlu[run] += (finish_lu.tv_nsec - start_lu.tv_nsec) / 1000000000.0;
        
        /*
        printf("  elapLU: %f\n",elapsedlu[run]);
        printf("  elapMult: %f\n",elapsed[run]);
        printf("  MoE: %f\n",moe[run]);
        printf("  ErrCount: %d\n",errorcount);
        */
        
        freeMatrix(matrix,row);
        freeMatrix(L,row);
        freeMatrix(U,row);
        freeMatrix(C,row);
        
    }
    
    printf("\n");
    
    if(GS_OR_CRT){
        printf("Gauss Comp\n");
    } else{
        printf("Crout Comp\n");
    }
    printf("Parallel LU , %d\n", LU_PAR);
    printf("Parallel MULT , %d\n", MULT_PAR);
    printf("ROW_SIZE, %d\n", ROW_SIZE);
    
    printf("Err , RUN_COUNT\n");
    printf("%d , %d\n", errorcount, RUN_COUNT);
    printf("Run , LU_TIME, MULT_TIME, MoE\n");
    for (int r = 0; r < RUN_COUNT; r++){
        printf("%d , %f , %f , %f\n", r, elapsedlu[r], elapsed[r], moe[r]);
    }
    printf("Run , LU_TIME, MULT_TIME, MoE\n");
    printf("Err , RUN_COUNT\n");
    printf("%d , %d\n", errorcount, RUN_COUNT);
    
    

    
    return 0;
}
