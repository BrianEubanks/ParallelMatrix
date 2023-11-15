# ParallelMatrix
Parallel Matrix Algorithms for LU decomposition

matrix - Java Implementations

## C - C Implementations

* Set RUN_COUNT and ROW_SIZE defines

* MULT_PAR - Run Parallel Multiplication. True or False
* LU_PAR - Run Parallel LU Decomposition. True or False


```
#define RUN_COUNT   100

#define ROW_SIZE    4

#define MULT_PAR    1
#define LU_PAR      1

```

### Crout Composite - 2 Threads. 1 for L and 1 for U

OSX
```
gcc composite.c matrix.c
```

Linux
```
gcc -pthread composite.c matrix.c
```

```
./a.out
```


### Gaussian Generic - N Threads

OSX
```
gcc gauss.c matrix.c
```

Linux
```
gcc -pthread gauss.c matrix.c
```

```
./a.out
```

