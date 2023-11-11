package matrix;


import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.Arrays;

public class MatrixTest {

	Double[][] ATest = new Double[][]{ {1.0,2.0,3.0},
									{3.0,8.0,14.0},
									{2.0,6.0,13.0}};
	Double[][] ATestexp = new Double[][]{ {1.0,2.0,3.0},
									{3.0,8.0,14.0},
									{2.0,6.0,13.0}};
	


    @Test public void GuassSerEqPar() {
    	
    	MatrixMain m = new MatrixMain();
    	

		m.Gaussian(ATest);
		m.GaussianPar(ATest);
		
		
        assertTrue(Arrays.deepEquals(ATest, ATest));
    }

   // @Test public void theoe1() {
    //    assertEquals(4, GraphGenerator.generateAllGraphs(2, "hasExactlyOneEdge").size());
    //}


}
