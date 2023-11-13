
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <Accelerate/Accelerate.h>

 

double randDouble() 
{
    return 0.1 + (double) rand() / (double)RAND_MAX;
}

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


void demo(){
    int r = 3, c = 4, i, j, count;
    double** matrix;
 
    // Note that arr[i][j] is same as *(*(arr+i)+j)

    matrix = getMatrix(r,c);

    count = 0;
    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++)
            matrix[i][j] = ++count; // OR *(*(arr+i)+j) = ++count
 
    printMatrix(matrix,r,c);

    printf("%f\n",matrix[1][2]);
 
    freeMatrix(matrix,r);
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

    int row = 1000;
    int col = row;
    double** matrix;
    time_t t;

    srand((unsigned) time(&t));
    matrix = getMatrix(row,col);

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
*/


    initMatrix(matrix,row,col);

    //printMatrix(matrix,row,col);
    //printf("%f\n",matrix[1][2]);


    clock_t begin = clock();

    Gaussian(matrix,row);

    clock_t end = clock();

    //printMatrix(matrix,row,col);
 
    freeMatrix(matrix,row);


    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("%f\n",time_spent);


    //printf("%f\n",randDouble());


 
    return 0;
}
