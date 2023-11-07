package matrix;


public class MatrixMain {
	
	public Double[][] A;
	public Double[][] L;
	public double det;
	public int n;
	
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
	
	public void Gaussian(Double[][] A) {
		for (int k = 0; k < n; k++) {
			for(int i = k+1; i < n; i++) {
				A[i][k] = A[i][k] / A[k][k];
				for (int j = k+1; j < n; j++) {
					A[i][j] = A[i][j] - A[i][k]*A[k][j];
				}
			}
		}
	}
	
	public void Invert(Double[][] A) {
		Gaussian(A);
		
		//Invert L
		
		for (int k = 0; k < n; k++) {
			L[k][k] = 1/A[k][k];
			for(int i = k; i < k; i++) {
				for (int j = k; j < i; j++) {
					L[i][k] = -A[i][j] * (L[j][k] /A[k][k]);
				}
			}
		}
		/*
		//Invert U
		for (int k = 0; k < n; k++) {
			A[k][k] = 1/A[k][k];
			for(int i = k+1; i < k; i++) {
				for (int j = k; j < i; j++) {
					A[k][i] = -A[j][i] * (A[k][j] / A[k][k]);
				}
			}
		}
		/*
		// Invert L
		for (int k = n-1; k >= 0 ; k--) {

			for(int i = k+1; i < n; i++) {
				double temp = A[i][k];
				A[i][k] = 0.0;
				for (int j = 0; j < n; j++) {
					A[j][k] = A[j][k] - (A[j][i] * temp);
				}
			}
		}
		*/
		
	}
	
	public void Determinant(Double[][] A) {
		Gaussian(A);
		det = 1;
		for(int i = 0; i < n; i++) {
			det*=A[i][i];
		}
	}
	
	public MatrixMain() {
		System.out.println("MatrixMain");
		A = new Double[][]{ {2.0,-1.0,-2.0},
						{-4.0,6.0,3.0},
						{-4.0,-2.0,8.0}};
		L = new Double[][]{ {0.0,0.0,0.0},
							{0.0,0.0,0.0},
							{0.0,0.0,0.0}};
		n = A[0].length;
		det = 0;
		printArr2d(A,"A");
		
		Invert(A);
		//Determinant(A);
		printArr2d(A,"A");
		System.out.println("det: "+det);
		System.out.println(A[0][1]);
		System.out.println(A[1][0]);
		printArr2d(L,"L");
		
	}
	
	public static void main(String args[]) {
		MatrixMain m = new MatrixMain();
	}

}

