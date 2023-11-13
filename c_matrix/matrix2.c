
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
 

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

void Gaussian2(double** A, int row) {
    int i;
    int j;
    int k;
    int n = row;
    for (int k = 0; k < n; k++) {
        for(int i = k+1; i < n; i++) {
            A[i][k] = -A[i][k] / A[k][k];
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
        /*
        for (int j = 0; j < n; j++) {
            //A[j][i] = A[j][i] - A[j][k] * A[k][i];
            A[i][i] = A[j][i] / A[i][i];
        }
       */
    }
}

void GaussianCompLU(double** A, double** L, double** U, int row) {
    int i;
    int j;
    int k;
    int n = row;
    
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
        /*
        for (int j = i+1; j < n; j++) {
            //A[j][i] = A[j][i] - A[j][k] * A[k][i];
            A[i][i] = A[j][i] / A[i][i];
        }*/
       
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

void GaussianCompLU2(double** a, double** l, double** u, int size) {
    
    //for each column...
    for (int i = 0; i < size; i++)
    {
        //for each row....
        for (int j = 0; j < size; j++)
        {
            //if j is smaller than i, set l[j][i] to 0
            if (j < i)
            {
                l[j][i] = 0;
                continue;
            }
            //otherwise, do some math to get the right value
            l[j][i] = a[j][i];
            for (int k = 0; k < i; k++)
            {
                //deduct from the current l cell the value of these 2 values multiplied
                l[j][i] = l[j][i] - l[j][k] * u[k][i];
            }
        }
        //for each row...
        for (int j = 0; j < size; j++)
        {
            //if j is smaller than i, set u's current index to 0
            if (j < i)
            {
                u[i][j] = 0;
                continue;
            }
            //if they're equal, set u's current index to 1
            if (j == i)
            {
                u[i][j] = 1;
                continue;
            }
            //otherwise, do some math to get the right value
            u[i][j] = a[i][j] / l[i][i];
            for (int k = 0; k < i; k++)
            {
                u[i][j] = u[i][j] - ((l[i][k] * u[k][j]) / l[i][i]);
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
                printf("det(L) close to 0!\n Can't divide by 0...\n");
                exit(EXIT_FAILURE);
            }
            U[j][i] = (A[j][i] - sum);/// L[j][j];
        }
        
        for (i = j; i < n; i++) {
            if (L[j][j] == 0) {
                printf("det(L) close to 0!\n Can't divide by 0...\n");
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
                printf("det(L) close to 0!\n Can't divide by 0...\n");
                exit(EXIT_FAILURE);
            }
            U[j][i] = (A[j][i] - sum) / L[j][j];
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

    int row = 4;
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

    
*/
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


    initMatrix(matrix,row,col);

    printMatrix(matrix,row,col);
    //printf("%f\n",matrix[1][2]);
    
    printMatrix(L,row,col);
    printMatrix(U,row,col);
    


    clock_t begin = clock();

    //GaussianCompLU2(matrix,L,U,row);
    //GaussianComp(matrix,row);
    crout(matrix,L,U,row);

    clock_t end = clock();

    printMatrix(matrix,row,col);
    printf("\n");
    
    printMatrix(L,row,col);
    printf("\n");

    printMatrix(U,row,col);
    
    matrixMult(L,U,C,row);
    printf("\n");
    printMatrix(C,row,col);

 
    freeMatrix(matrix,row);
    freeMatrix(L,row);
    freeMatrix(U,row);


    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("%f\n",time_spent);


    //printf("%f\n",randDouble());


 
    return 0;
}
