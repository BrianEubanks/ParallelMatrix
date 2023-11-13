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

	
    public class RowBlock implements Runnable {
    	int id;
        int i;
        int n;
        int[] rowList;
        Double[][] A;
        int start;
        int end;
 
        RowBlock(int id, int i, Double[][] Arr, int[] rowList, int nThreads, int blocksize) {
        	this.id = id;
            this.i = i;
            this.A = Arr;
            this.rowList = rowList;
            this.n = Arr[0].length;

            start = rowList[id];
 //           if (start < i) {
  //          	start = i;
   //         }

            end = rowList[id+1];
            if (end > n) {
            	end = n;
            }
        }
 
        @Override
        public void run() {
        	//System.out.println("  ThreadStart: "+j);
        	//double leadmult = A[start][i];
        	
        	System.out.println("T.i: "+id +"." + i +" S.E:" + start + "." + end);
        	//System.out.println(end);

			for(int j = start+1; j <= end; j++) {
				A[j][i] = A[j][i] / A[start][i];
				//A[j][i] = -A[j][i] / A[start][i];
			}
			
			for(int j = start+1; j < end; j++) {
				for (int k = i+1; k < end; k++) {
					A[j][k] = A[j][k] - A[j][i] * A[i][k];
					//A[j][k] = A[j][k] + A[j][i] * A[j][k];
				}
			}
        	//System.out.println("  ThreadDone : "+j);
        }
    }
	
    public class Saxpy implements Runnable {
        int i;
        int k;
        int j;
        int n;
        Double[][] Arr;
 
        Saxpy(int i,  int k, Double[][] Arr) {
            this.i = i;
            this.k = k;
            this.Arr = Arr;
            this.n = Arr[0].length;
        }
 
        @Override
        public void run() {
        	//System.out.println("  ThreadStart: "+j);
            for (int j = k+1; j < n; j++) {
            	Arr[i][j] = Arr[i][j] - Arr[i][k] * Arr[k][j];
            }
        	//System.out.println("  ThreadDone : "+j);
        }
    }
    
    public class Div implements Runnable {
        int i;
        int k;
        int j;
        int n;
        Double[][] Arr;
 
        Div(int i,  int k, Double[][] Arr) {
            this.i = i;
            this.k = k;
            this.Arr = Arr;
            this.n = Arr[0].length;
        }
 
        @Override
        public void run() {
        	//System.out.println("  ThreadStart: "+j);
        	A[i][k] = (1 / A[k][k]) * A[i][k];
        	//System.out.println("  ThreadDone : "+j);
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
		//printArr2d(A,"A Gauss");
		return A;
	}
	
	public Double[][] SerialRankTwo(Double[][] A) {
		//Gaussian(A);
		int n = A[0].length;

		for (int k = 0; k < n -1; k+=2) {
			
			for (int i = k+1; i < n-1; i++) {
				A[i][k+1]   = 1/A[k][k] * A[i][k];
				A[i][k+1]   = A[i][k+1] + A[k][k+1] * A[i][k];
				A[i+1][k+1] = 1/A[k+1][k+1] * A[i+1][k+1];
				
				A[k+1][i+1] = A[k+1][i] - A[k][k] * A[k+1][k];
				A[i+1][i+1] = A[i][i] - A[k][i] * A[i][k] - A[k+1][i] * A[i][k+1];
			}
			
			
		}
		return A;
	}
	
	public Double[][] Gaussian2(Double[][] A) {
		int n = A[0].length;
		for (int i = 0; i < n; i++) {
			for(int j = i+1; j < n; j++) {
				A[j][i] = A[j][i] / A[i][i];
				for (int k = i+1; k < n; k++) {
					A[j][k] = A[j][k] - A[j][i] * A[i][k];
				}
			}
		}	
		//printArr2d(A,"A Gauss");
		return A;
	}
	
	
	public Double[][] GaussianPar (Double[][] A) {
		int n = A[0].length;
        Thread[] threads = new Thread[n];
		for (int k = 0; k < n; k++) {
			
			for(int i = k+1; i < n; i++) {
				A[i][k] = (1 / A[k][k]) * A[i][k];
			
	            threads[i] = new Thread(new Saxpy(i,k,A));
	            threads[i].start();
			}
	        for(int i = k+1; i < n; i++) {
	            try {
					threads[i].join();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		//printArr2d(A,"A GaussPar");
		return A;
	}
	
	
	public Double[][] GaussianPar2 (Double[][] A) {
		int n = A[0].length;
		Thread[] threads_div = new Thread[n];
        Thread[] threads = new Thread[n];
		for (int k = 0; k < n; k++) {
			
			for(int i = k+1; i < n; i++) {
				
	            threads_div[i] = new Thread(new Div(i,k,A));
	            threads_div[i].start();
			}
	        for(int i = k+1; i < n; i++) {
	            try {
					threads_div[i].join();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			
			
			for(int i = k+1; i < n; i++) {
				A[i][k] = (1 / A[k][k]) * A[i][k];
			
	            threads[i] = new Thread(new Saxpy(i,k,A));
	            threads[i].start();
			}
	        for(int i = k+1; i < n; i++) {
	            try {
					threads[i].join();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		//printArr2d(A,"A GaussPar");
		return A;
	}

	public Double[][] GaussianPar3 (Double[][] A) {
		int n = A[0].length;
		int nThreads = 2;
        Thread[] threads = new Thread[nThreads];
        int rowLimits[] = {0,2,4};
        int blocksize = 5;
        
		for (int i = 0; i < n; i++) {
			
			for (int thread = 0; thread < nThreads; thread++) {
	            threads[thread] = new Thread(new RowBlock(thread,i,A,rowLimits,nThreads,blocksize));
	            threads[thread].start();
			}
			
			for (int thread = 0; thread < nThreads; thread++) {
	            try {
					threads[thread].join();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		//printArr2d(A,"A GaussPar");
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
	

	
	public Double[][] Invert2(Double[][] A) {
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
		for (int k = n-1; k > 0; k--) {
			
			//A[k][k] = 1/A[k][k];
			for(int i = k; i < k; i++) {
				double temp = A[i][k];
				A[i][k] = 0.0;
				
				for (int j = 0; j < n; j++) {
					A[j][k] = A[j][k] - (A[j][i] * temp);
				}
			}
		}
		return A;
	}
		
	public Double[][] Invert(Double[][] A) {
		Gaussian(A);
		int n = A[0].length;
			
		//Invert U
		for (int k = 0; k < n; k++) {
			A[k][k] = 1 / A[k][k];
			for(int i = k; i < n; i++) {
				
				for (int j = k; j < i; j++) {
					A[i][k] = -A[i][j] * (A[j][k] / A[k][k]);
				}
				
			}
		}

	
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


		//Double[][] F = generateMatrix(1000);
		//Double[][] F2 = arr2dCopy(F);
		//Double[][] F3 = arr2dCopy(F);
		
		Double[][] F = A;
		Double[][] F2 = arr2dCopy(F);
		Double[][] F3 = arr2dCopy(F);
/*
		
		long start;
		long stop;
		long seqtime;
		long partime;
		
		System.out.println("input");
		printArr2d(F,"C");
		
		System.out.println("Sequential-lu");
		start = System.currentTimeMillis();
		//printArr2d(Gaussian(F),"C");
		Gaussian(F);
		stop = System.currentTimeMillis();
		seqtime = stop-start;
		System.out.println("Seq Time: "+seqtime);
		printArr2d(F,"C");
		
		System.out.println("Sequential-lu-IJK");
		start = System.currentTimeMillis();
		//printArr2d(Gaussian(F),"C");
		Gaussian2(F2);
		stop = System.currentTimeMillis();
		seqtime = stop-start;
		System.out.println("Seq Time: "+seqtime);
		printArr2d(F2,"C");
		
		System.out.println("Parallel-lu-2");
		start = System.currentTimeMillis();
		//printArr2d(GaussianPar(F2),"C");
		GaussianPar3(F3);
		stop = System.currentTimeMillis();
		partime = stop-start;
		System.out.println("Par Time2: "+partime);
		printArr2d(F3,"C");
		

		/*
		System.out.println("Parallel-lu");
		start = System.currentTimeMillis();
		//printArr2d(GaussianPar(F2),"C");
		GaussianPar(F2);
		stop = System.currentTimeMillis();
		partime = stop-start;
		System.out.println("Par Time: "+partime);
		//printArr2d(F2,"C");
		
		
		System.out.println("Parallel-lu-2");
		start = System.currentTimeMillis();
		//printArr2d(GaussianPar(F2),"C");
		GaussianPar(F3);
		stop = System.currentTimeMillis();
		partime = stop-start;
		System.out.println("Par Time2: "+partime);
		//printArr2d(F3,"C");
		
	*/
		System.out.println("Sequential-lu");
		//start = System.currentTimeMillis();
		//printArr2d(Gaussian(F),"C");
		Gaussian(F);
		//stop = System.currentTimeMillis();
		//seqtime = stop-start;
		//System.out.println("Seq Time: "+seqtime);
		printArr2d(F,"C");
		
		printArr2d(SerialRankTwo(F2),"SerialRankTwo");
		System.out.println("done");
	


		


		
	}
	
	public static void main(String args[]) {
		MatrixMain m = new MatrixMain();
	}

}

