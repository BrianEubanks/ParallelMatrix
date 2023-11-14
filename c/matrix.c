#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define EPSILON 0.0000001

struct mult_i_args{
    int i;
    int n;
    double** A;
    double** B;
    double** C;
};

struct mult_j_args{
    int i;
    int j;
    int n;
    double** A;
    double** B;
    double** C;
};

int *mult_j_fn(void *ptr){
    int i;
    int j;
    int n;
    int k;
    double** A;
    double** B;
    double** C;


    i = ((struct mult_j_args*) ptr)->i;
    j = ((struct mult_j_args*) ptr)->j;
    n = ((struct mult_j_args*) ptr)->n;
    A = ((struct mult_j_args*) ptr)->A;
    B = ((struct mult_j_args*) ptr)->B;
    C = ((struct mult_j_args*) ptr)->C;


    for (int k = 0; k < n; k++){
        C[i][j]+=A[i][k]*B[k][j];
    }

    return 0;
}

int *mult_i2_fn(void *ptr){
    int i;
    int j;
    int n;
    double** A;
    double** B;
    double** C;


    i = ((struct mult_i_args*) ptr)->i;
    n = ((struct mult_i_args*) ptr)->n;
    A = ((struct mult_i_args*) ptr)->A;
    B = ((struct mult_i_args*) ptr)->B;
    C = ((struct mult_i_args*) ptr)->C;


    for (j = 0; j < n; j++){
        for (int k = 0; k < n; k++){
            C[i][j]+=A[i][k]*B[k][j];
        }
    }
    return 0;
}

int *mult_i_fn(void *ptr){
    int i;
    int j;
    int n;
    double** A;
    double** B;
    double** C;
    
    pthread_t threads[n];
    struct mult_j_args j_data[n];

    i = ((struct mult_i_args*) ptr)->i;
    n = ((struct mult_i_args*) ptr)->n;
    A = ((struct mult_i_args*) ptr)->A;
    B = ((struct mult_i_args*) ptr)->B;
    C = ((struct mult_i_args*) ptr)->C;


    for (j = 0; j < n; j++){
        j_data[i].i = i;
        j_data[i].j = j;
        j_data[i].n = n;
        j_data[i].A = A;
        j_data[i].B = B;
        j_data[i].C = C;
        pthread_create(&threads[i],NULL,(void*)mult_j_fn,(void*)&j_data[i]);
    }
    for(int i = 0; i < n; i++){
        pthread_join(threads[i],NULL);
    }
    
    
    return 0;
}

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


void matrixMultPar(double**a, double** b, double** c, int n){
    
    pthread_t threads[n];
    struct mult_i_args i_data[n];
    
    for(int i = 0; i < n; i++){
        i_data[i].i = i;
        i_data[i].n = n;
        i_data[i].A = a;
        i_data[i].B = b;
        i_data[i].C = c;
        pthread_create(&threads[i],NULL,(void*)mult_i2_fn,(void*)&i_data[i]);
    }
    for(int i = 0; i < n; i++){
        pthread_join(threads[i],NULL);
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

int equalsMatrix(double** A, double** B, int n){
    int i,j;

    for (i = 0; i < n; i++){
        for (j = 0; j < n; j++){
            if (A[i][j] - B[i][j] > EPSILON){
                printf("%d,%d\n",i,j);
                printf("%f != %f\n",A[i][j],B[i][j]);
                return 0;
            }
        }
    }
    return 1;
}

