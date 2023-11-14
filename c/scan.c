#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


struct scan_args{
    int i;
    int d;
    double* A;
    double* V;
};

int *scan_fn(void *ptr){
    int i;
    int d;
    double* A;
    double* V;
    
    i = ((struct scan_args*) ptr)->i;
    d = ((struct scan_args*) ptr)->d;
    A = ((struct scan_args*) ptr)->A;
    V = ((struct scan_args*) ptr)->V;
    

    if (i - d >= 0) {
        V[i] = A[i-d];
    }
    printf("P1Thread:%d\n",i);
    
    return 0;
}

int *scan2_fn(void *ptr){
    int i;
    int d;
    double* A;
    double* V;
    
    printf("P2Thread:%d\n",i);
    
    i = ((struct scan_args*) ptr)->i;
    d = ((struct scan_args*) ptr)->d;
    A = ((struct scan_args*) ptr)->A;
    V = ((struct scan_args*) ptr)->V;
    

    if (i - d >= 0) {
        A[i] += V[i];
    }
    

    
    return 0;
}


void printArr(double* A, int n){
    int j;
    


    for (j = 0; j < n; j++){
        printf("%f ", A[j]);
    }
    
    printf("\n");
}



void prefixSum(double* A, int n){
    int d;
    
    double V[n];
    
    pthread_t threads[n];
    struct scan_args scan_data[n];


    for (d = 1; d < n; d*=2){
        printf("d.%d\n",d);

        for (int i = 0; i < n; i++){
            scan_data[i].A = A;
            scan_data[i].V = V;
            scan_data[i].i = i;
            scan_data[i].d = d;
            pthread_create(&threads[i],NULL,(void*)scan_fn,(void*)&scan_data[i]);
        }
        for (int i = 0; i < n; i++){
            pthread_join(threads[i],NULL);
        }
        printf("V\n");
        printArr(V,n);
        
        for (int i = 0; i < n; i++){
            scan_data[i].A = A;
            scan_data[i].V = V;
            scan_data[i].i = i;
            scan_data[i].d = d;
            pthread_create(&threads[i],NULL,(void*)scan2_fn,(void*)&scan_data[i]);
        }
        for (int i = 0; i < n; i++){
            pthread_join(threads[i],NULL);
        }
        printf("A\n");
        printArr(A,n);
    }
    printf("\n");
}



int main(){
    double A[] = {1,2,3,4,5,6,7,8};
    printArr(A,8);
    prefixSum(A,8);
    printArr(A,8);

    
}
