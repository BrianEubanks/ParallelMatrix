# ParallelMatrix
Parallel Matrix Algorithms for LU decomposition

matrix - Java Implementations

C - C Implementations


OSX
```
gcc composite.c matrix.c
```

Linux
```
gcc -pthread composite.c matrix.c
```

```
a.out
```


composite.c
```
    //GaussianCompLU2(matrix,L,U,row); //Parallel
    GaussianCompLU(matrix,L,U,row); //Sequential
```
