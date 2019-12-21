import java.util.*;

public class NeuralNet {
  
    public static void main(String[] args) {
    	
    	//input to the input layer
        double[][] ipArr = null;
        //expected output from the output layer
        double[][] opArr = null;
        
    	int choice = 0;
    	System.out.println("1. Dyspnea  [IP features will be Pollution, Smoker, Cancer, Xray]");
    	System.out.println("2. Xray     [IP features will be Pollution, Smoker, Cancer, Dyspnea]");
    	System.out.println("Choose which feature will be used as output to be predicted...");
 
    	Scanner sc = new Scanner(System.in);
    	choice = Integer.parseInt(sc.next());
    	
    	switch(choice) {
    		case 1: ipArr = new double[][] {{0.1, 0.3, 0.1, 0.1}, {0.1, 0.7, 0.5, 0.4}, {0.9, 0.3, 0.5, 0.4}, {0.9, 0.7, 0.9, 0.9}};
    				opArr = new double[][] {{0}, {0}, {0.5}, {1}};
    				break;
    	    
    		case 2: ipArr = new double[][] {{0.1, 0.3, 0.1, 0.1}, {0.1, 0.7, 0.5, 0.4}, {0.9, 0.3, 0.5, 0.4}, {0, 0, 0.5, 1}};
    	     		opArr = new double[][] {{0.9}, {0.7}, {0.9}, {0.9}};
    	     		break;
    	    
    		default: System.out.println("Only options 0 and 1 please...");
    	    		 break;
    	}
    	
        sc.close();
        //no. of outputs
        int op = 4;
        //no. of nodes in hidden layer
        int hiddenNodes = 500;
        
        
        //the rows to be ip of nn are columns, so a transpose of the matrix is required
        ipArr = Transpose(ipArr);
       
        //same case for the ops
        opArr = Transpose(opArr);

        //randomising the weights and bias between layer 1 (ip) and layer 2
        double[][] W1 = random(hiddenNodes, 4);
        double[][] b1 = new double[hiddenNodes][op];
        
        //randomising the weights and bias between layer 2 and layer 3 (op)
        double[][] W2 = random(1, hiddenNodes);
        double[][] b2 = new double[1][op];

        //training for 5000 epochs
        for (int i = 0; i < 5000; i++) {
            
        	//feedforward prop between layer 1 and layer 2 
            double[][] Z1 = sumMat(dotVec(W1, ipArr), b1);
            double[][] A1 = sigmoidAct(Z1);

            //feedforward prop between layer 2 and layer 3 
            double[][] Z2 = sumMat(dotVec(W2, A1), b2);
            double[][] A2 = sigmoidAct(Z2);

            //calculating error between expected and predicted values
            double cost = crossEntropyLoss(op, opArr, A2);
         
            //back prop between layer 3 and layer 2
            //adjusts weights between 2 and 3
            double[][] dZ2 = diffMat(A2, opArr);
            double[][] dW2 = divisionMat(dotVec(dZ2, Transpose(A1)), op);
            double[][] db2 = divisionMat(dZ2, op);

            //back prop between layer 2 and layer 1
            //adjusts weights between 1 and 2
            double[][] dZ1 = productMat(dotVec(Transpose(W2), dZ2), diffMat(1.0, power(A1, 2)));
            double[][] dW1 = divisionMat(dotVec(dZ1, Transpose(ipArr)), op);
            double[][] db1 = divisionMat(dZ1, op);

            //gradient descent for layer 1-2 weights and biases
            W1 = diffMat(W1, productMat(0.01, dW1));
            b1 = diffMat(b1, productMat(0.01, db1));

            //gradient descent for layer 2-3 weights and biases
            W2 = diffMat(W2, productMat(0.01, dW2));
            b2 = diffMat(b2, productMat(0.01, db2));

            //prints results every 400 epochs
            if (i % 500 == 0) {
            	System.out.println("");
            	System.out.println("######################################");
            	System.out.println("error = " + cost);
            	System.out.println("Predicted OP = " + Arrays.deepToString(A2));
            }
        }
        
    }
    
    
    //list of all functions involved including activations, losses, arithmetic functions and 
    //random weight and bias setting functions
    /*
	 * 1. transpose
	 * 2. sumMat
	 * 3. diffMat (matrix wise)
	 * 4. diffMat (element wise)
	 * 5. dotVec (dot product)
	 * 6. productMat (element wise)
	 * 7. productMat (matrix wise)
	 * 8. power (element wise)
	 * 9. shape 
	 * 10. sigmoidAct
	 * 11. reluAct
	 * 12. divisionMat
	 * 13. crossEntropyLoss
	 * 14. setaThread
	 * 15. getaThread
	 * 16. setUniform
	 * 17. setUniform
	 * 18. setUniform
	 * 19. setUniform
	 * 20. setUniform
	 * 21. random
	 */
	
	
	
    private static Random rando;
    private static long aThread;

    static {
        aThread = System.currentTimeMillis();
        rando = new Random(aThread);
    }
    
    //matrix transpose before being fed into nn
    public static double[][] Transpose(double[][] x) {
        int len = x.length;
        int bre = x[0].length;
        double[][] y = new double[bre][len];
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < bre; j++) {
                y[j][i] = x[i][j];
            }
        }
        return y;
    }


    //matrix addition
    public static double[][] sumMat(double[][] x, double[][] y) {
        int len = x.length;
        int bre = x[0].length;
        double[][] z = new double[len][bre];
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < bre; j++) {
                z[i][j] = x[i][j] + y[i][j];
            }
        }
        return z;
    }


    //matrix subtraction
    public static double[][] diffMat(double[][] x, double[][] y) {
        int len = x.length;
        int bre = x[0].length;
        double[][] z = new double[len][bre];
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < bre; j++) {
                z[i][j] = x[i][j] - y[i][j];
            }
        }
        return z;
    }


    //element wise subtraction
    public static double[][] diffMat(double x, double[][] y) {
        int len = y.length;
        int bre = y[0].length;
        double[][] z = new double[len][bre];
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < bre; j++) {
                z[i][j] = x - y[i][j];
            }
        }
        return z;
    }


    //dot product
    public static double[][] dotVec(double[][] x, double[][] y) {
        int m1 = x.length;
        int n1 = x[0].length;
        int m2 = y.length;
        int n2 = y[0].length;
        if (n1 != m2) {
            throw new RuntimeException("Illegal matrix dimensions.");
        }
        double[][] z = new double[m1][n2];
        for (int i = 0; i < m1; i++) {
            for (int j = 0; j < n2; j++) {
                for (int k = 0; k < n1; k++) {
                    z[i][j] += x[i][k] * y[k][j];
                }
            }
        }
        return z;
    }


    //element wise multiplication
    public static double[][] productMat(double[][] a, double[][] b) {
        int m = b.length;
        int n = b[0].length;

        if (a.length != m || a[0].length != n) {
            throw new RuntimeException("Illegal matrix dimensions.");
        }
        double[][] c = new double[m][n];
        for (int j = 0; j < m; j++) {
            for (int i = 0; i < n; i++) {
                c[j][i] = b[j][i] * a[j][i];
            }
        }
        return c;
    }


    //matrix multiplication
    public static double[][] productMat(double a, double[][] b) {
        int m = b.length;
        int n = b[0].length;

        double[][] c = new double[m][n];
        for (int j = 0; j < m; j++) {
            for (int i = 0; i < n; i++) {
                c[j][i] = b[j][i] * a;
            }
        }
        return c;
    }


    //math.power applied element wise
    public static double[][] power(double[][] a, int b) {
        int m = a.length;
        int n = a[0].length;

        double[][] c = new double[m][n];
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                c[i][j] = Math.pow(a[i][j], b);
            }
        }
        return c;
    }


    //return shape of matrix
    public static String shape(double[][] mat) {
        int len = mat.length;
        int bre = mat[0].length;
        String Vshape = "(" + len + "," + bre + ")";
        return Vshape;
    }


    //using sigmoid activation function
    public static double[][] sigmoidAct(double[][] mat) {
        int len = mat.length;
        int bre = mat[0].length;
        double[][] z = new double[len][bre];

        for (int i = 0; i < len; i++) {
            for (int j = 0; j < bre; j++) {
                z[i][j] = (1.0 / (1 + Math.exp(-mat[i][j])));
            }
        }
        return z;
    }
    
    //using relu activation function
    public static double[][] reluAct(double[][] mat) {
        int len = mat.length;
        int bre = mat[0].length;
        double[][] op = new double[len][bre];

        for (int i = 0; i < len; i++) {
            for (int j = 0; j < bre; j++) {
            	if(mat[i][j] >= 0) {
            		op[i][j] = mat[i][j];
            	}
            	else
            		op[i][j] = 0;      
            }
        }
        return op;
    }

    //element wise division
    public static double[][] divisionMat(double[][] a, int b) {
        int m = a.length;
        int n = a[0].length;

        double[][] z = new double[m][n];

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                z[i][j] = (a[i][j] / b);
            }
        }
        return z;
    }
    
    //using cross entropy loss function
    public static double crossEntropyLoss(int batchSize, double[][] I, double[][] J) {
        int m = J.length;
        int n = J[0].length;
        double[][] k = new double[m][n];
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                k[i][j] = (I[i][j] * Math.log(J[i][j])) + ((1 - I[i][j]) * Math.log(1 - J[i][j]));
            }
        }
        double sum = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                sum += k[i][j];
            }
        }
        return -sum / batchSize;
    }
    
    //using softmax activation function
    public static double[][] softmaxAct(double[][] ip) {
        double[][] op = new double[ip.length][ip[0].length];
        double sum = 0.;
        for (int i = 0; i < ip.length; i++) {
            for (int j = 0; j < ip[0].length; j++) {
                sum += Math.exp(ip[i][j]);
            }
        }
        for (int i = 0; i < ip.length; i++) {
            for (int j = 0; j < ip[0].length; j++) {
                op[i][j] = Math.exp(ip[i][j]) / sum;
            }
        }
        return op;
    }

    
    //sequence of random nos
    public static void setaThread(long s) {
        aThread = s;
        rando = new Random(aThread);
    }


    //returns the set nos
    public static long getaThread() {
        return aThread;
    }


    //set random weights/biases
    public static double setUniform() {
        return rando.nextDouble();
    }

    
    //random int between 0,n
    public static int setUniform(int n) {
        if (n <= 0) {
            throw new IllegalArgumentException("argument must be positive: " + n);
        }
        return rando.nextInt(n);
    }


    //random long int between 0,n
    public static long setUniform(long n) {
        if (n <= 0L) {
            throw new IllegalArgumentException("argument must be positive: " + n);
        }

        long r = rando.nextLong();
        long m = n - 1;

        // power of two
        if ((n & m) == 0L) {
            return r & m;
        }

        // reject over-represented candidates
        long u = r >>> 1;
        while (u + m - (r = u % n) < 0L) {
            u = rando.nextLong() >>> 1;
        }
        return r;
    }
    //return random int between a,b
    public static int setUniform(int x, int y) {
        if ((y <= x) || ((long) y - x >= Integer.MAX_VALUE)) {
            throw new IllegalArgumentException("invalid range: [" + x + ", " + y + ")");
        }
        return x + setUniform(y - x);
    }
    //return random real between a,b
    public static double setUniform(double x, double y) {
        if (!(x < y)) {
            throw new IllegalArgumentException("invalid range: [" + x + ", " + y + ")");
        }
        return x + setUniform() * (y - x);
    }
    //return matrix with values between 0,1
    public static double[][] random(int m, int n) {
        double[][] a = new double[m][n];
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = setUniform(0.0, 1.0);
            }
        }
        return a;
    }
}