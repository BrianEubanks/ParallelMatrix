
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
 

double randDouble() 
{
    return (double) 1 + (rand() % 5);
}

struct args{
    int i;
    int k;
    int size;
    double** Arr;
};

struct croutargs{
    int i;
    int j;
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

int *crout_U_fn(void *ptr){
    int i;
    int j;
    int k;
    double** A;
    double** L;
    double** U;
    int n;
    int sum;
    
    //i = ((struct args*) ptr)->i;
    j = ((struct croutargs*) ptr)->j;
    n = ((struct croutargs*) ptr)->size;
    A = ((struct croutargs*) ptr)->A;
    L = ((struct croutargs*) ptr)->L;
    U = ((struct croutargs*) ptr)->U;

    for (i = j; i < n; i++) {
        sum = 0;
        for(k = 0; k < j; k++) {
            sum = sum + L[j][k] * U[k][i];
        }
        if (L[j][j] == 0) {
            printf("j = %d det(L) close to 0!\n Can't divide by 0...\n",j);
            exit(EXIT_FAILURE);
        }
        U[j][i] = (A[j][i] - sum);// / L[j][j];
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

    //i = ((struct args*) ptr)->i;
    j = ((struct croutargs*) ptr)->j;
    n = ((struct croutargs*) ptr)->size;
    A = ((struct croutargs*) ptr)->A;
    L = ((struct croutargs*) ptr)->L;
    U = ((struct croutargs*) ptr)->U;

    for (i = j; i < n; i++) {
        sum = 0;
        for (k = 0; k < j; k++) {
            sum = sum + L[i][k] * U[k][j];
        }
        L[i][j] = A[i][j] - sum;
    }
    return 0;
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

void GaussianPar(double** A, int row) {
    int i;
    int j;
    int k;
    int n = row;

    pthread_t threads[n];

    struct args saxpy_data;

    for (int k = 0; k < n; k++) {
        for(int i = k+1; i < n; i++) {
            A[i][k] = A[i][k] / A[k][k];
            saxpy_data.i=i;
            saxpy_data.k=k;
            saxpy_data.size=row;
            saxpy_data.Arr=A;

            pthread_create(&threads[i],NULL,(void*)saxpy_fn,(void*)&saxpy_data);

        }
        for(int i = k+1; i < n; i++) {

            pthread_join(threads[i],NULL);

        }
    }   
}

void crout(double **A, double **L, double **U, int n) {
    int i, j, k;
    double sum = 0;

    printf("c\n");
    for (i = 0; i < n; i++) {
        U[i][i] = 1;
    }

    for (j = 0; j < n; j++) {
        for (i = j; i < n; i++) {
            sum = 0;
            for (k = 0; k < j; k++) {
                sum = sum + L[i][k] * U[k][j];
            }
            L[i][j] = A[i][j] - sum;
        }
        
        for (i = j; i < n; i++) {
            sum = 0;
            for(k = 0; k < j; k++) {
                sum = sum + L[j][k] * U[k][i];
            }
            
             if (L[j][j] == 0) {
             printf("j = %d det(L) close to 0!\n Can't divide by 0...\n",j);
             exit(EXIT_FAILURE);
             }
             U[j][i] = (A[j][i] - sum);// / L[j][j];
        }

        for (i = j; i < n; i++) {
           // sum = 0;
           // for(k = 0; k < j; k++) {
           //     sum = sum + L[j][k] * U[k][i];
           // }
            if (L[j][j] == 0) {
                printf("j = %d det(L) close to 0!\n Can't divide by 0...\n",j);
                exit(EXIT_FAILURE);
            }
            U[j][i] = U[j][i] / L[j][j];
        }
    }
    
}

void croutPar(double **A, double **L, double **U, int n) {
    int i, j, k;
    double sum = 0;

    printf("c\n");
    for (i = 0; i < n; i++) {
        U[i][i] = 1;
    }
    
    struct croutargs crout_data;
    
    crout_data.size = n;
    crout_data.A = A;
    crout_data.L = L;
    crout_data.U = U;
    
    pthread_t t_crout_L;
    pthread_t t_crout_U;

    for (j = 0; j < n; j++) {
        
        crout_data.j = j;
        
        // L
        pthread_create(&t_crout_L,NULL,(void*)crout_L_fn,(void*)&crout_data);
/*
        for (i = j; i < n; i++) {
            sum = 0;
            for (k = 0; k < j; k++) {
                sum = sum + L[i][k] * U[k][j];
            }
            L[i][j] = A[i][j] - sum;
        }
*/
        // U
        pthread_create(&t_crout_U,NULL,(void*)crout_U_fn,(void*)&crout_data);
        /*
        for (i = j; i < n; i++) {
            sum = 0;
            for(k = 0; k < j; k++) {
                sum = sum + L[j][k] * U[k][i];
            }
            if (L[j][j] == 0) {
                printf("det(L) close to 0!\n Can't divide by 0...\n");
                exit(EXIT_FAILURE);
            }
            U[j][i] = (A[j][i] - sum) / L[j][j];
        }
         */
        pthread_join(t_crout_L,NULL);

        pthread_join(t_crout_U,NULL);
        
        for (i = j; i < n; i++) {
           // sum = 0;
           // for(k = 0; k < j; k++) {
           //     sum = sum + L[j][k] * U[k][i];
           // }
            if (L[j][j] == 0) {
                printf("j = %d det(L) close to 0!\n Can't divide by 0...\n",j);
                exit(EXIT_FAILURE);
            }
            U[j][i] = U[j][i] / L[j][j];
        }

    }
}

void initMatrix(double** arr, int row, int col){
    int i,j;

    for (i = 0; i < row; i++){
        for (j = 0; j < col; j++){
            arr[i][j] = randDouble();
        }
    }
}


int main()
{
    //demo();

    int row = 16;
    int col = row;
    double** matrix;
    double** C;
    double** L;
    double** U;
    time_t t;

    srand((unsigned) time(&t));
    matrix = getMatrix(row,col);
    L = getMatrix(row,col);
    U = getMatrix(row,col);
    C = getMatrix(row,col);

/*
    matrix[0][0]= 2.0 ;
    matrix[0][1]= -1.0;
    matrix[0][2]= -2.0;
    matrix[1][0]= -4.0;
    matrix[1][1]= 6.0;
    matrix[1][2]= 3.0;
    matrix[2][0]= -4.0;
    matrix[2][1]= -2.0;
    matrix[2][2]= 8.0;

    

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
//*/

    initMatrix(matrix,row,col);

    //printMatrix(matrix,row,col);
    //printf("%f\n",matrix[1][2]);
    
    //printMatrix(L,row,col);
    //printMatrix(U,row,col);
    


    clock_t begin = clock();

    //GaussianCompLU2(matrix,L,U,row);
    //GaussianComp(matrix,row);
    croutPar(matrix,L,U,row);

    clock_t end = clock();

    printMatrix(matrix,row,col);
    //printf("\n");
    
    //printMatrix(L,row,col);
    //printf("\n");

    //printMatrix(U,row,col);
    
    matrixMult(L,U,C,row);
    printf("\n");
    printMatrix(C,row,col);

 
    freeMatrix(matrix,row);
    freeMatrix(L,row);
    freeMatrix(U,row);
    freeMatrix(C,row);


    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("%f\n",time_spent);


    //printf("%f\n",randDouble());

    return 0;
}
