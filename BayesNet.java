import java.util.*;
import java.io.*;



public class BayesNet {

	/**
	 * @param args
	 */
	
	public class Node {
		public String name;
		public int id;
		public int noOfStates;
		public int parentCount;
		public int childCount;
		public int probCount;
		public String state1;
		public String state2;
		public int[] nodes_parents;
		public int[] nodes_children;
		public double[] nodes_probabilities;
	}
	
	
	public static void main(String[] args) {
		BayesNet run = new BayesNet(args);
	}
	
	public BayesNet(String[] args) {
		// TODO Auto-generated method stub
		String fileBN = "";
				
		//get the path of the file containing the BN data
		if (args.length > 0)
		{
			try {
				fileBN = args[0];
				
			} catch (Exception e) {
				System.err.println("Invalid path");
				System.exit(-1);
			} 
		}

			
		
		//int count = 0; //index denoting the node number
		int totNodes = 0; //total nodes in the BN
		int howManyChild = 0;
		try {
			Scanner forBN = new Scanner(new File(fileBN));
			
			//line 1 title of dataset
			String bnTitle = forBN.nextLine();
			
			//line 2 total nodes in dataset
			String forNodeTot = forBN.nextLine();
			totNodes = Integer.parseInt(forNodeTot);
			
			//class object array created
			BayesNet.Node[] nodes = new Node[totNodes];
			int nodesCounter = 0;
				
			//map index to label
			//HashMap<Integer, String> map1 = new HashMap<>();
			//no of children for each node
			//ArrayList noOfChild = new ArrayList(); 
			//children of each node
			//ArrayList currentToCLink = new ArrayList();
			int nodeNo = 0;
			
			do {
				
				//line 3 is an empty line
				String emptyLine = forBN.nextLine();
				
				//creating object(node) of class(graph)
				BayesNet.Node node = new Node();
				
				//line 4 no. of states in the node
				String getStateNo = forBN.nextLine();
				node.noOfStates = Integer.parseInt(getStateNo);
				
				//line 5 name of the node and its respective index
				String[] labelWithIndex = forBN.nextLine().split("  ");
				node.name = labelWithIndex[0];
				node.id = Integer.parseInt(labelWithIndex[1]);
				
				//line 6 state 1
				String stateOne = forBN.nextLine();
				node.state1 = stateOne;
				//System.out.println("state1  "+node.state1);
				
				//line 7 state 2
				String stateTwo = forBN.nextLine();
				node.state2 = stateTwo;
				//System.out.println("state2  "+node.state2);
				
				//line 8 no. of parents and their respective list
				String parentLine = forBN.nextLine();
				Scanner parScanLine = new Scanner(parentLine);
				int numParents = parScanLine.nextInt();
				node.parentCount = numParents;
				node.nodes_parents = new  int[numParents];
				for(int i = 0; i < numParents; i++) {
					int parentId = parScanLine.nextInt();
					node.nodes_parents[i] = parentId;
				}
				
				//line 9 no. of children and their respective list
				String childrenLine = forBN.nextLine();
				Scanner chiScanLine = new Scanner(parentLine);
				int numChildren = chiScanLine.nextInt();
				node.childCount = numChildren;
				node.nodes_children = new  int[numChildren];
				for(int i = 0; i < numChildren; i++) {
					int childId = chiScanLine.nextInt();
					node.nodes_children[i] = childId;
				}
				
				//line 10 no. of probabilities
				String probs = forBN.nextLine();
				node.probCount = Integer.parseInt(probs);
				
				//line 11 list of probabilities
				String[] listProbs = forBN.nextLine().split("  ");
				node.nodes_probabilities = new double[node.probCount];
				for(int i = 0; i < node.probCount; i++) {
					node.nodes_probabilities[i] = Double.parseDouble(listProbs[i]);
				}
				
				//assign the newly created node into the previously created node array
				nodes[nodesCounter] = node;
				++nodesCounter;
				++nodeNo;
			
			//do..while loop runs till all nodes are entered in graph	
			} while(nodeNo < totNodes);
			//tester: System.out.println("value in cancer:  "+nodes[3].nodes_probabilities[0]);
			
			/*
			
			//SOLVING FEW QUERIES IN A BAYESIAN NETWORK
			
			
			
		
		*/
			//query 1: to find x-ray result from the bayesian network created
			String Q1 = "What is the probability of a patient's xray displaying tumour on the condition that he does have cancer?";
			String probQ1 = "Pr ( Xray = positive | Cancer = present )";
			//function to tokenize the query
			double ans1 = analyse(probQ1, nodes);
			System.out.println("value of ans1:  "+ans1);
			
			//query 2: to find dyspnea result from the bayesian network created
			String Q2 = "What is the probability of a patient NOT having dyspnea on the condition that he does have cancer?";
			String probQ2 = "Pr ( Dyspnea = no | Cancer = present )";
			//function to tokenize query
			double ans2 = analyse(probQ2, nodes);
			System.out.println("value of ans2:  "+ans2);
			
			//QUERY WITH 2 CONDITIONALS
			//query 3: to find cancer result from pollution and smoking
			String Q3 = "What is the probability of a patient having cancer on the condition that pollution is high and he is a heavy smoker?";
			String probQ3 = "Pr ( Cancer = present | Pollution = high , Smoker = max )";
			//function to tokenize the query
			double ans3 = analyse(probQ3, nodes);
			System.out.println("value of ans3:  "+ans3);
		}
		catch(FileNotFoundException e) {
			System.out.println("FileBN not found!!");
			e.printStackTrace();
		}
			

	}
	
	//tokenising a query
	double analyse(String query, BayesNet.Node[] nodes) {
		
		Scanner scan = new Scanner(query);
		String[] myQuery = scan.nextLine().split(" ");
		scan.close();
		for (int i = 0; i < myQuery.length; i++) {
			System.out.println("place  "+i+"  String:  "+myQuery[i]);
		}
		
		double ansQuery = 0.0;
		//specific conditions to seperate 2 and 3 variable queries {more functions needed}
		if(myQuery.length < 12)
			ansQuery = queryHandler1(myQuery[2], myQuery[4], myQuery[6], myQuery[8], nodes);
		else
			ansQuery = queryHandler2(myQuery[2], myQuery[4], myQuery[6], myQuery[8], myQuery[10], myQuery[12], nodes);
			
		return ansQuery;
		
	}
	
	
	//query with two variables
	double queryHandler1(String qpart2, String qpart4, String qpart6, String qpart8, BayesNet.Node[] nodes) {
		String flag1 = "", flag2 = "";
		int nodeID = 999;
		
		for (int i = 0; i < nodes.length; i++) {		
			if(qpart2.equals(nodes[i].name)) {
				nodeID = nodes[i].id;
				if (qpart4.equals(nodes[i].state1))
					flag1 = "0";
				if (qpart4.equals(nodes[i].state2))
					flag1 = "1";
			}
		}
		
		for (int i = 0; i < nodes.length; i++) {
			if(qpart6.equals(nodes[i].name)) {
				if (qpart8.equals(nodes[i].state1))
					flag2 = "0";	
				if (qpart8.equals(nodes[i].state2))
					flag2 = "1";				
			}
		}
		
		String bit = flag1.concat(",").concat(flag2);
		Scanner strCheck = new Scanner(bit);
		String[] split = strCheck.nextLine().split(",");
		int index = 0; 
		
		int j = split.length - 1;
		for( int i = 0; i < split.length; i++) {
			index += Integer.parseInt(split[i]) * Math.pow(2, j);
			--j;
		}
		strCheck.close();
		System.out.println("Index in CPT: "+index);
		return nodes[nodeID - 1].nodes_probabilities[index];
	}
	
	
	//query with 3 variables
	double queryHandler2(String qpart2, String qpart4, String qpart6, String qpart8, String qpart10, String qpart12, BayesNet.Node[] nodes) {
		String flag1 = "", flag2 = "", flag3 = "";
		int nodeID = 999;
		
		for (int i = 0; i < nodes.length; i++) {		
			if(qpart2.equals(nodes[i].name)) {
				nodeID = nodes[i].id;
				if (qpart4.equals(nodes[i].state1))
					flag1 = "0";
				if (qpart4.equals(nodes[i].state2))
					flag1 = "1";
			}
		}
		
		for (int i = 0; i < nodes.length; i++) {
			if(qpart6.equals(nodes[i].name)) {
				if (qpart8.equals(nodes[i].state1))
					flag2 = "0";	
				if (qpart8.equals(nodes[i].state2))
					flag2 = "1";				
			}
		}
		
		for (int i = 0; i < nodes.length; i++) {
			if(qpart10.equals(nodes[i].name)) {
				if (qpart12.equals(nodes[i].state1))
					flag3 = "0";	
				if (qpart12.equals(nodes[i].state2))
					flag3 = "1";				
			}
		}
		
		String bit = flag1.concat(",").concat(flag2).concat(",").concat(flag3);
		Scanner strCheck = new Scanner(bit);
		String[] split = strCheck.nextLine().split(",");
		int index = 0; 
		
		int j = split.length - 1;
		for( int i = 0; i < split.length; i++) {
			index += Integer.parseInt(split[i]) * Math.pow(2, j);
			--j;
		}
		System.out.println("Index in CPT: "+index);
		strCheck.close();
		return nodes[nodeID - 1].nodes_probabilities[index];
	}
	
	
/*
 * bn require query for each and every prob 
 * nn final value is enough to say if good or bad result
 * 
 */
	
	
}
	

