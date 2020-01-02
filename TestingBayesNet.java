import java.util.*;
import java.io.*;



public class TestingBayesNet {

	/**
	 * @param args
	 */
	
	public class Node {
		public String name;
		public int id;
		public int noOfStates;
		public String nodeType;
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
		TestingBayesNet run = new TestingBayesNet(args);
	}
	
	public TestingBayesNet(String[] args) {
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
			TestingBayesNet.Node[] nodes = new Node[totNodes];
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
				TestingBayesNet.Node node = new Node();
				
				//line 4 no. of states in the node and type of node
				String[] getStateNode = forBN.nextLine().split("  ");
				node.noOfStates = Integer.parseInt(getStateNode[0]);
				node.nodeType = getStateNode[1];
				
				
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
			
			
			
			//SOLVING FEW QUERIES IN A BAYESIAN NETWORK
			
			//query 1: to find x-ray result from the bayesian network created
			String Q1 = "What is the probability of a patient's xray displaying tumour on the condition that he does have cancer?";
			String probQ1 = "Pr ( Xray = positive | Cancer = present )";
			//function to tokenize the query
			double ans1 = analyse(probQ1, nodes);
			System.out.println("value of ans to query1:  ");
			System.out.println(""+probQ1+" = "+ans1);
			System.out.println("-------------------------------");
			
			
			
			//query 2: to find dyspnea result from the bayesian network created
			String Q2 = "What is the probability of a patient NOT having dyspnea on the condition that he does have cancer?";
			String probQ2 = "Pr ( Dyspnea = no | Cancer = present )";
			//function to tokenize query
			double ans2 = analyse(probQ2, nodes);
			System.out.println("value of ans2:  "+ans2);
			System.out.println(""+probQ2+" = "+ans2);
		

			
		}
		catch(FileNotFoundException e) {
			System.out.println("FileBN not found!!");
			e.printStackTrace();
		}
			

	}
	//tokenising a query
		double analyse(String query, TestingBayesNet.Node[] nodes) {
			
			Scanner scan = new Scanner(query);
			String[] myQuery = scan.nextLine().split(" ");
			scan.close();
			System.out.println("Tokenising the given query......");
			for (int i = 0; i < myQuery.length; i++) {
				System.out.println("place  "+i+"  String:  "+myQuery[i]);
			}
			
			double ansQuery = 0.0;
			//specific conditions to seperate 2 and 3 variable queries 
			if(myQuery.length < 12)
				ansQuery = queryHandler1(myQuery[2], myQuery[4], myQuery[6], myQuery[8], nodes);
			else
				ansQuery = queryHandler2(myQuery[2], myQuery[4], myQuery[6], myQuery[8], myQuery[10], myQuery[12], nodes);
				
			return ansQuery;
			
		}
		
		
		//query with two variables
		double queryHandler1(String qpart2, String qpart4, String qpart6, String qpart8, TestingBayesNet.Node[] nodes) {
			String flag1 = "", flag2 = "";
			int nodeID = 999, parNodeID = 999;
			
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
			System.out.println("Node Name: "+qpart2+" index of CPT: "+index);
			for (int i = 0; i < nodes.length; i++) {
				if(qpart6.equals(nodes[i].name))
					parNodeID = nodes[i].id - 1;
					if("testing".equals(nodes[i].nodeType)) {
						System.out.println("Before testing... CPT = "+nodes[nodeID - 1].nodes_probabilities[index]);
						double testRes = 0.1010;
						testRes = testBN(i, index, parNodeID, qpart8, nodes);
						return testRes;
					}
			}
			return nodes[nodeID - 1].nodes_probabilities[index];
		}
		
		
		
		//query with 3 variables
		double queryHandler2(String qpart2, String qpart4, String qpart6, String qpart8, String qpart10, String qpart12, TestingBayesNet.Node[] nodes) {
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
			System.out.println("Node Name: "+qpart2+" index of CPT: "+index);
			strCheck.close();
			return nodes[nodeID - 1].nodes_probabilities[index];
		}
		
		
		//dealing with testing nodes
		//function receives the index of node in graph, index of prob in respective node CPT and index of parent
		double testBN(int indOfNode, int indOfCPT, int indOfPar, String parState, TestingBayesNet.Node[] nodes) {
			
			Scanner scanPar = new Scanner(System.in);
			//setting threshold value statically [other option is to read from file]
			double threshold = 0.5;
			//a num higher than node index
			double currentID = nodes[indOfNode].id;
			
			//testing node cpt
			double testCPT = nodes[indOfNode].nodes_probabilities[indOfCPT];
			
			//parents of current node
			int[] parOfParList = new int[nodes[indOfPar].nodes_parents.length];
			for(int i = 0; i < nodes[indOfPar].nodes_parents.length; i++) {
				parOfParList[i] =  nodes[indOfPar].nodes_parents[i] - 1;
			}
			
			//names of parents of parents list
			String[] parOfParNamesList = new String[parOfParList.length];
			for(int i = 0; i < nodes[indOfPar].nodes_parents.length; i++) {
				parOfParNamesList[i] =  nodes[parOfParList[i]].name;
			}
			
			//get state1 list
			String[] parOfParState1List = new String[parOfParList.length];
			for(int i = 0; i < parOfParList.length; i++) {
				parOfParState1List[i] = nodes[parOfParList[i]].state1;
			}
			
			//get state2 list 
			String[] parOfParState2List = new String[parOfParList.length];
			for(int i = 0; i < parOfParList.length; i++) {
				parOfParState2List[i] = nodes[parOfParList[i]].state2;
			}
			
			//gets the states of parents of parents (grandparents) from the user
			String[] userIPStates = new String[parOfParList.length];
			for(int i = 0; i < nodes[indOfPar].noOfStates; i++) {
				System.out.println("Enter state of par "+parOfParNamesList[i]+" "+parOfParState1List[i]+" or "+parOfParState2List[i]+":");
				userIPStates[i] = scanPar.nextLine();
			}
			
			
			String parQueryStart = "Pr ( ";
			String parQueryMid = "";
			String parQueryEnd = ")";
			String fullQuery = "";
			
			for(int i = 0; i < parOfParList.length; i++) {
				parQueryMid = parQueryMid.concat(parOfParNamesList[i]).concat(" = ").concat(userIPStates[i]).concat(" , ");
			}
			parQueryStart = parQueryStart.concat(nodes[indOfPar].name).concat(" = ").concat(parState).concat(" | ");
			fullQuery = fullQuery.concat(parQueryStart).concat(parQueryMid).concat(parQueryEnd);
			
			
			double cptValOfPar = analyse(fullQuery, nodes);
			System.out.println("Value of parent during test: "+cptValOfPar);
			if (cptValOfPar >= threshold) {
				System.out.println("Since parent's CPT with its respective parents is greater than or equal to threshold, Current node value remains unchanged");
				return testCPT;
			}
				
			else {
				System.out.println("Since parent's CPT with its respective parents is lesser than threshold, Current node value is assigned 0");
				return 0;
			}
				
			
		}
		
	
}