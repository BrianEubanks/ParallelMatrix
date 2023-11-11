package matrix;

import java.util.Random;

public class MatrixMain {
	
	public Double[][] A;
	public Double[][] B;
	public Double[][] C;
	public Double[][] D;
	public Double[][] E;
	public Double[][] F;
	public Double[][] L;

	public double det;

	
	public void printArr2d(Double[][] G, String str) {
		System.out.println(str + ": ");
		for(int i = 0; i < G.length; i++) {
			for (int j = 0; j < G.length; j++) {
				System.out.print(G[i][j]);
				System.out.print(",");
			}
			System.out.println(" ");
		}
	}
	
	public Double[][] arr2dCopy(Double[][] G) {
		int size = G[0].length;
		Double[][] matrix = new Double[size][size];
		for(int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				matrix[i][j] = G[i][j];
			}
		}
		return matrix;
	}

	
    public class Worker implements Runnable {
        int i;
        int k;
        int j;
        Double[][] Arr;
 
        Worker(int i, int j, int k, Double[][] Arr) {
            this.i = i;
            this.k = k;
            this.j = j;
            this.Arr = Arr;
        }
 
        @Override
        public void run() {
            //for (int j = k+1; j < n; j++) {
        	//System.out.println("  ThreadStart: "+j);
        	Arr[i][j] = Arr[i][j] - Arr[i][k] * Arr[k][j];
        	//System.out.println("  ThreadDone : "+j);
           // }
        }
    }

	public Double[][] Gaussian(Double[][] A) {
		int n = A[0].length;
		for (int k = 0; k < n; k++) {
			for(int i = k+1; i < n; i++) {
				A[i][k] = A[i][k] / A[k][k];
				for (int j = k+1; j < n; j++) {
					A[i][j] = A[i][j] - A[i][k] * A[k][j];
				}
			}
		}	
		printArr2d(A,"A Gauss");
		return A;
	}
	
	public Double[][] GaussianPar (Double[][] A) {
		int n = A[0].length;
        Thread[] threads = new Thread[n];
		for (int k = 0; k < n; k++) {
			
			for(int i = k+1; i < n; i++) {
				A[i][k] = (1 / A[k][k]) * A[i][k];
			
				//Parallel might need a sync step here
				//for (int j = k+1; j < n; j++) {
				//	A[i][j] = A[i][j] - A[i][k] * A[k][j];
				//}
			
				//for(int j = k+1; j < n; j++) {
					//A[i][k] = -A[i][k] / A[k][k];
				//	saxpy_row(i,n,k,A);
				//}
		        // declaring four threads
				//System.out.println("I: "+i);

		 
		        // Creating four threads, each evaluating its own part
		        for (int j = k+1; j < n; j++) {
		            threads[j] = new Thread(new Worker(i,j,k,A));
		            threads[j].start();
		        }
		 
		        // joining and waiting for all threads to complete
		        for (int j = k+1; j < n; j++) {
		            try {
		                threads[j].join();
		            } catch (InterruptedException e) {
		                e.printStackTrace();
		            }
		        }
			}
		}
		printArr2d(A,"A GaussPar");
		return A;
	}
	
	public void Determinant(Double[][] A) {
		Gaussian(A);
		det = 1;
		int n = A[0].length;
		for(int i = 0; i < n; i++) {
			det*=A[i][i];
		}
	}
	
	public Double[][] Invert(Double[][] A) {
		Gaussian(A);
		int n = A[0].length;
			
		//Invert U
		for (int k = 0; k < n; k++) {
			A[k][k] = 1 / A[k][k];
			for(int i = 0; i < k; i++) {
				for (int j = i; j < k; j++) {
					A[i][k] = -A[k][k] * (A[i][j] * A[j][k]);
				}
			}
		}
		
		//Calculate U-1L-1
		for (int k = n-1; k >= 0; k--) {
			
			//A[k][k] = 1/A[k][k];
			for(int i = k; i < k; i++) {
				double temp = A[i][k];
				A[i][k] = 0.0;
				
				for (int j = 0; j < n; j++) {
					A[j][k] = A[j][k] - (A[j][i] * temp);
				}
			}
		}
		

		// Interchanges
		/*
		for (int k = n-1; k >= 0 ; k--) {
	
			for(int i = k+1; i < n; i++) {
				double temp = A[i][k];
				A[i][k] = 0.0;
				for (int j = 0; j < n; j++) {
					A[j][k] = A[j][k] - (A[j][i] * temp);
				}
			}
		}*/
		
		return A;
	}
	
	
    public Double[][] generateMatrix(int size) {
        Double[][] matrix = new Double[size][size];
        Random rand = new Random(); 
 
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                matrix[i][j] = rand.nextDouble()+0.5;
            }
        }
        return matrix;
    }
	
    
	public MatrixMain() {
		System.out.println("MatrixMain");

		D = new Double[][]{ {1.0,2.0,3.0},
							{3.0,8.0,14.0},
							{2.0,6.0,13.0}};

		Double[][] A = new Double[][]{ {2.0,-1.0,-2.0},
									{-4.0,6.0,3.0},
									{-4.0,-2.0,8.0}};
						
		B = new Double[][]{ {2.0,-1.0,-2.0},
							{-4.0,6.0,3.0},
							{-4.0,-2.0,8.0}};
		
		C = new Double[][]{ {1.0,-2.0,-2.0,-3.0},
							{3.0,-9.0,0.0,-9.0},
							{-1.0,2.0,4.0,7.0},
							{-3.0,-6.0,26.0,2.0}};


		Double[][] F = generateMatrix(100);
		Double[][] F2 = arr2dCopy(F);
		
		//Double[][] F = B;
		//Double[][] F2 = arr2dCopy(B);
		
		long start;
		long stop;
		long seqtime;
		long partime;
		
		System.out.println("input");
		printArr2d(F,"C");
		
		System.out.println("lu");
		start = System.currentTimeMillis();
		printArr2d(Gaussian(F),"C");
		stop = System.currentTimeMillis();
		seqtime = stop-start;
		
		System.out.println("input");
		//printArr2d(F,"C");
		
		System.out.println("lu");
		start = System.currentTimeMillis();
		//printArr2d(GaussianPar(F2),"C");
		GaussianPar(F2);
		stop = System.currentTimeMillis();
		partime = stop-start;
		
		//printArr2d(Invert(A),"Ainv");
		System.out.println("done");
		System.out.println("seq: "+seqtime);
		System.out.println("par: "+partime);

		


		
	}
	
	public static void main(String args[]) {
		MatrixMain m = new MatrixMain();
	}

}

