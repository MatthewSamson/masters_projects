/* 
 * DATABASE PARAMETERS:
 * size of each block of records = 1024
 * no. of records in each block (upto 80% size of a block) = 11
 * 
 * B+TREE PARAMETERS:
 * no. of keys per node = 8
 * no. of ptrs per node = 9
 * middle point of each node = 4
 * starting index of new split node = 4
 *
 * B+TREE NODE SPLIT:
 * new_node[0] = old_full[4]
 * split1[0 to 3] = old_full[0 to 3]
 * split2[0 to 3] = old_full[4 to 7]
 */


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// table 1: list of keys
vector<string> all_keys;
// table 2: rec_no to block_addr
vector<int> block_table;

// table 3: block_no to ptr
vector<int> block_addr;

// B+Tree structure and functions
// MODEL OF EACH NODE IN THE TREE
struct BPT{
   string keys[8];   
   BPT **child; 
   bool leaf; 
   int num; 
}*root = NULL, *nex = NULL, *ex = NULL;


// CREATE A NEW NODE AND INITIALISE ITS VALUES
BPT* create(){
   int i;
   nex = new BPT;
   nex->child = new BPT *[9];
   nex->leaf = true;
   nex->num = 0;
   
   for (i = 0; i < 9; i++) {
      nex->child[i] = NULL;
   }
   for (i = 0; i < 8; i++){
	  nex->keys[i] = "";
   }
   return nex;
}


// PRINT ALL THE VALUES IN A PARTICULAR NODE
void print_node(BPT *node){
	int k, tot = node->num;
	cout << "isLeaf? " << node->leaf << endl;
	cout << "tot child " << node->num << endl;
	for(k = 0; k < tot; k++){
		cout << "Child no: " << k+1 << endl;
		cout << "child id: " << node->keys[k] << endl;
		cout << "nex child: " << node->child[k] << endl;
	}
	cout << "fin child: " << node->child[tot] << endl;
	cout << endl;
}

 
// LOCATES THE NODE AND THE POSITION OF WHERE THE NEW VALUE IS TO BE INSERTED
int find_location(BPT *cur, string val){
	int k, loc;
	for (k = 0; k < cur->num; k++){
		// search if value is present at the root or interior node
		if (val == cur->keys[k]){
		   k++;
		   break;
		}
		// search if value is in the previous node
		else if (val < cur->keys[0]){
		   k = 0;
		   break;
		} 
		// search if value is in the next node
		else if (val > cur->keys[cur->num-1]){
			k = cur->num;
			break;
		}
		// search if value can be placed in between two elements in the current node
		else if ((val > cur->keys[k]) && (val < cur->keys[k + 1])){
			k++;
			break;
		}
		else {
		   continue;
		}
	}
	loc = k;
	return loc;
}


// PRINT THE TREE AND EXPLORE EACH NODE
void show_tree(BPT *pt, bool exp){
	cout << endl;
	int i;
	bool ch = exp;
	static int cc = 0;
	cout << cc;
	for (i = 0; i < pt->num; i++){
		if (pt->keys[i] != " "){
			cout << " : " << pt->keys[i];
		}
	}
	if (ch == true){
		cout << endl;
		print_node(pt);
	}
	for (i = 0; i < pt->num+1; i++){
		if (pt->leaf == false){
			++cc;
			show_tree(pt->child[i], ch);
		}
		else{
			continue;
		}
	}
}


// SORT ALL THE KEYS IN ONE NODE
void sort_node(string *keyset, int num){
   int i, j;
   string temp;
   // using selection sort
   for (i = 0; i < num-1; i++) {
      for (j = i+1; j < num; j++) {
		 // basic swapping
         if (keyset[i] > keyset[j]) {
            temp = keyset[i];
            keyset[i] = keyset[j];
            keyset[j] = temp;
         }
      }
   }
}


// INSERT KEYS TO TREE

// split case 1: full leaf split into two leaves with new interior node
   /*
    * i/p: full leaf node
    * o/p: an interior node (new root), two leaf nodes
    * initialises new node
    * breaks full leaf into two new leaf nodes
    * connects the two leaf nodes to newly created interior node
    * 
    */
    
string break_leaf(BPT *ex){
   int itr;
   string mid;
   BPT *ob1, *ob2;
   ob2 = create();
   mid = ex->keys[4];
   ob1 = create();
   ob1->leaf= false;
   ex->leaf= true;
   ob2->leaf = true;
   for(itr = 4; itr < 8; itr++){
       ob2->keys[itr - 4] = ex->keys[itr];
	   ob2->num++;
	   ex->keys[itr] = "";
	   ex->num--;
    }
    for(itr = 0; itr < 9; itr++){
	   ex->child[itr] = NULL;
	   ob2->child[itr] = NULL;
    }
	ob1->keys[0] = mid;
	ob1->child[ob1->num] = ex;
	ob1->child[ob1->num + 1] = ob2;
	ob1->num++;
	root = ob1;
	return mid;
}
 

// split case 2: full root node is split into two interior nodes (at any level)
   /*
    * i/p: a full root node
    * o/p: new root and its two branches interior nodes
    * creates new root
    * breaks current root into two interior nodes
    * sets the two interior nodes as leaves to new root
    * 
    */
    
string break_root(BPT *ex){
	int itr;
	string mid;
    BPT *ob1, *ob2;
    ob2 = create();
    mid = ex->keys[4];
    ob1 = create();
    ob1->leaf= false;
    ex->leaf= false;
    ob2->leaf = false;
    for (itr = 5; itr < 9; itr++){
	    ob2->child[itr - 5] = ex->child[itr];
    }
    for (itr = 5; itr < 8; itr++){
       ob2->keys[itr - 5] = ex->keys[itr];
       ob2->num++;
       ex->keys[itr] = "";
       ex->num--;
       ex->child[itr] = NULL;
    }
    ex->keys[4] = "";
    ex->num--;
    ob1->keys[ob1->num] = mid;
    ob1->child[ob1->num] = ex;
    ob1->child[ob1->num + 1] = ob2;
    ob1->num++;
    root = ob1;
    return mid;
}


// split case 3: create new nodes and join them to interior nodes
// split case 3a: split a full leaf and then join it to an existing parent node
// split case 3b: split a full interior node and then join it to its existing parent node  

string update_node(BPT *ex, int i){
	int itr;
	string mid;
    BPT *ob2, *ob3, *prev;
    ob2 = create();
    prev = create();
	ob3 = ex->child[i];
	mid = ob3->keys[4];
	
   /*
    * i/p: a (!not full!) interior node (works only for interior nodes)
    * o/p: split full child(leaf)
    * break child leaf into two
    * join the two new child-leaves to interior i/p
    * 
    */
    
    // in case the value and node is to be inserted anywhere before the end
    if ((ex->child[i])->leaf == true){
		if (i < ex->num){
			for (itr = i; itr < 9; itr++){
				prev->child[itr] = ex->child[itr];
				ex->child[itr] = prev->child[itr - 1];
			}
			ex->child[i] = NULL;
		}
	   ob3->leaf = true;
	   ob2->leaf = true;
	   for (itr = 4; itr < 8; itr++) {
		  ob2->keys[itr - 4] = ob3->keys[itr];
		  ob2->num++;
		  ob3->keys[itr] = "";
		  ob3->num--;
	   }
	   for (itr = 0; itr < 9; itr++) {
		   ob3->child[itr] = NULL;
		}
    }
	   
   /*
	* i/p: a (!not full!) existing interior node
	* o/p: new addition to upper interior, split the one below the root
	* split the full interior node below the upper interior
	* join the new splitted interiors to the upper interior
	* 
	*/ 
  
	else{
		// in case the value and node is to be inserted anywhere before the end
		if (i < ex->num){
			for (itr = i; itr < 9; itr++){
				prev->child[itr] = ex->child[itr];
				ex->child[itr] = prev->child[itr - 1];
			}
			ex->child[i] = NULL;
		}
	    ob3->leaf = false;
	    ob2->leaf = false;
		for (itr = 5; itr < 9; itr++){
		    ob2->child[itr - 5] = ob3->child[itr];
	    }
	    for (itr = 5; itr < 8; itr++) {
		   ob2->keys[itr - 5] = ob3->keys[itr];
		   ob2->num++;
		   ob3->keys[itr] = "";
		   ob3->num--;
		   ob3->child[itr] = NULL;
	    }
	   ob3->keys[4] = "";
	   ob3->num--; 
	}  
	
	// newly created nodes/leaves are being attached to the exiting node
    ex->child[i] = ob3;
    ex->child[i + 1] = ob2;
    return mid;
}


// SEARCH KEY IN AN EXISTING TREE
int search(BPT *sr, string s_key){
	int k, nn, flag = 0;
	// move from root to leaf
	while (sr->leaf == false){
		print_node(sr);
		nn = find_location(sr, s_key);
		sr = sr->child[nn];
	}
	// search the leaf
	for (k = 0; k < sr->num; k++){
		if (s_key == sr->keys[k]){
			flag =  1;
			break;
		}
		else{
			flag = 0;
		}
	}
	return flag;
}


// DELETING A KEY IN THE EXISTING TREE
void delete_key(BPT *sr, string s_key){
	int k, nn;
	// move from root to leaf
	while (sr->leaf == false){
		print_node(sr);
		nn = find_location(sr, s_key);
		sr = sr->child[nn];
	}
	// search the leaf
	for (k = 0; k < sr->num; k++){
		if (s_key == sr->keys[k]){
			sr->keys[k] = " ";
			break;
		}
	}
}


// INSERTING VALUES INTO THE TREE
void add_key(string a) {
   int i, pt;
   string t;
   ex = root;
   
   // case 1: in case of an empty tree, we first create a tree by initialising the root node
   if (ex == NULL) {
	  cout << "No existing tree!, so we create one by initialising the root..." << endl;
      root = create();
      cout << "Root is initialised with inserted value: " << a << endl; 
      ex = root;
   }
   
   // traverse tree to prevent duplicates
   int f1 = search(ex, a);
   if (f1 == 1){
      cout << "Already present in tree!";  
      ex = root;
      return;
	}
	
   // cases where tree is already initialised
   else {
	  cout << "Key not in tree!" << endl;
	  cout << "Inserting key into the tree..." << endl;
	  
	  // case 2: in case of a full leaf node - create a new parent node
	  // searches over every leaf node
      if (ex->leaf == true && ex->num == 8){
		 cout << "Current leaf is full!, creating new parent node..." << endl;
         t = break_leaf(ex);
         cout << "Value is now inserted into the split leaf: " << a << endl;
         ex = root;
         pt = find_location(ex, a);
         ex = ex->child[pt];
      } 
      
      // case 3: in case of a full leaf or parent node - split child and attach to existing parent node
      else { 
         while (ex->leaf == false) {
			pt = find_location(ex, a);
            if ((ex->child[pt])->num == 8) {
				t = update_node(ex, pt);
				ex->keys[ex->num] = t;
				ex->num++;
				sort_node(ex->keys, ex->num);
				ex = root;
				cout << "Value is now inserted into the split node: " << a << endl;
				sort_node(ex->keys, ex->num);
                continue;
            } 
            
            // case 4: in the case of a full root - root is not a leaf as in case 1
			if (root->num == 8){
				 cout << "Resetting the root..." << endl;
				 t = break_root(root);
				 ex = root;
				 cout << "Root is now reset!" << endl;
				 cout << "New root contains...";
				 for (i = 0; i < root->num; i++){
					 cout << root->keys[i];
				 } 
				 cout << endl;
			}
            else {
			   // go to the next node
               ex = ex->child[pt]; 
            }
         }
      }
   }
   // value is being inserted and node is sorted
   ex->keys[ex->num] = a;
   ex->num++;
   sort_node(ex->keys, ex->num);
}

// Functions used to setup the database
// SORTING ALL THE KEYS IN THE DATABASE
vector<string> sort_strings(vector<string> strset){
	// basic sorting algorithm to sort strings
	cout << "Sorting the strings..." << endl;
	int si = strset.size();
	for(int i = 0; i < si; i++){
		for(int j = i + 1; j < si; j++){
			if(strset[i] > strset[j]){
				string temp = strset[i];
				strset[i] = strset[j];
				strset[j] = temp;
			}
		}
	}
	cout << "Strings sorted!!" << endl;
	return strset;
}

// LOOKUP THE DATABASE ADDRESS FILE
int lookup_db(int skey){
	int bloc = block_table[skey];
	return bloc;
}

int main(){
	
	string filename;
	string str;
	char ch1, ch2, ch3, ch4;
	vector<string> liststr;
	vector<string> sortdone;
	vector<string> sorted;
	int sr_si, i;
	
	// choosing the file of records to load
	cout << "Enter the name of the file containing records..." << endl;
	cout << "Press 'd' to use the data given for the assignment..." << endl;
	cout << "Press 'a' to use another file..." << endl;
	cin >> ch1;
	
	if (ch1 == 'a'){
		cout << "Enter name of file (make sure it is in the current directory)" << endl;
		cin >> filename;
	}
	
	else{
		cout << "Using the file of records given as part of the assignment..." << " ";
		filename = "data.txt";
		cout << filename << endl;
	}
	
	
	// loading and opening up the file
	// open up the file
	ifstream file(filename);
	while(getline(file, str)){
		liststr.push_back(str);
	}
	cout << endl << "Records in " << filename << " have been loaded..." << endl;
	
	// sorting the strings
	sortdone = sort_strings(liststr);
	sr_si = sortdone.size();
	cout << "Records in " << filename << " have been sorted..." << endl;
	
	do{
		cout << endl << "Press v to view the sorted strings..." << endl;
		cout << "Press c to continue..." << endl;
		cin >> ch2;
		
		if(ch2 == 'v'){
			for (i = 0; i < sr_si; i++){
				cout << sortdone[i] << endl;
			}
		}
		else{
			break;
		}
	} while(ch2 == 'v');
	
	
	// creating the database file
	string writefile = "db_bptree.txt";
	cout << "Database will be written into: " << writefile << endl;
	ofstream thefile;
	thefile.open(writefile);
	cout << "The Format of a BLOCK..." << endl;
	cout << "Field-1 key" << " --> ";
	cout << "Record size (header 2)" << " --> ";
	cout << "Field-2 val" << endl;
	
	
	// list of fields in the database file
	string keyval;
	int lettercnt;
	string remaining;
	int tot_req = 0.8*1024;
	int block_no = 1;
	int cur_size = 0;
	int rec_size = 0;
	int ful_size = 0;
	block_addr.push_back(ful_size);
	
	for(i = 0; i < sr_si; i++){
		string cur_str = sortdone[i];
		keyval = cur_str.substr(0, 9);
		remaining = cur_str.substr(10, cur_str.length());
		lettercnt = remaining.length();
		
		// record size used to keep track of remaining space in the block
		rec_size = sizeof(keyval) + sizeof(remaining) + sizeof(lettercnt) + 3;
		cur_size = cur_size + rec_size;
		
		
		cout << rec_size << " " << cur_size << endl;
		if(cur_size > tot_req){
			ful_size = ful_size + cur_size;
			block_addr.push_back(ful_size);
			block_no++;
			cur_size = 0;
			thefile << endl;
		}
		
		block_table.push_back(block_no);
		thefile << lettercnt << "\t"; 
		all_keys.push_back(keyval);
		thefile << keyval << "\t";
		thefile << remaining; 
		
		add_key(keyval);
		
	}
	thefile.close();
	
	cout << "All insertions have been completed!!" << endl;
	cout << "Database file " << writefile << " has been created!!" << endl;
	cout << "B+Tree indices have been filled!!" << endl << endl;
	
	do{
		cout << endl << endl;
		cout << "Press v to view the B+Tree..." << endl;
		cout << "Press e to examine the B+Tree..." << endl;
		cout << "Press k to view keys in the B+Tree..." << endl;
		cout << "Press c to continue..." << endl;
		cin >> ch3;
		cout << endl;
		
		if (ch3 == 'v'){
			show_tree(root, false);
		}
		else if (ch3 == 'e'){
			show_tree(root, true);
		}
		else if (ch3 == 'k'){
			int j, key_si;
			key_si = all_keys.size();
			for (j = 0; j < key_si; j++){
				cout << all_keys[j] << " ";
			}
			cout << endl;
		}
		else if (ch3 == 'c'){
			break;
		}
		else{
			cout << "Please enter valid option..." << endl;
		}
	}while(ch3 != 'c');
	
	// tree and database manipulation:
	do{
		cout << endl << endl;
		cout << "Press a to add another record..." << endl;
		cout << "Press s to search a record..." << endl;
		cout << "Press d to delete a record..." << endl;
		cout << "Press v to view the B+Tree..." << endl;
		cout << "Press k to view all the keys..." << endl;
		cout << "Press r to perform a range search between the keys..." << endl;
		cout << "Open file " << writefile << " to view the blocks of records..." << endl;
		cout << "Press c to exit..." << endl;
		cin >> ch4;
		cout << endl;
		
		
		if (ch4 == 'a'){
			int idx;
			string new_rec;
			string new_key;
			string new_field;
		
			// get the new record and break it up
			cout << "Record format is as follows: " << endl;
			cout << "Field 1: Key containing 9 characters" << endl;
			cout << "Field 2: Variable length field containing n characters" << endl;
			cout << "Enter a full record to be added..." << endl;
			cin.ignore();
			getline(cin, new_rec);
			new_key = new_rec.substr(0, 9);
			new_field = new_rec.substr(10, new_rec.length());
			
			cout << "New key: " << new_key << endl;
			cout << "New field val: " << new_field << endl;
			cout << "Testing if key is already in the B+Tree..." << endl;
			
			// tree search
			int sea = search(root, new_key);
			if (sea == 1){
				cout << "Unable to insert, Record already in the Database!!" << endl;
			}
			
			else{
				// resorting the keys
				cout << "Key not found in B+Tree, Preparing to insert..." << endl;
				all_keys.push_back(new_key);
				vector<string> resort = sort_strings(::all_keys);
				add_key(new_key);
				int resize = resort.size();
				for (i = 0; i < resize; i++){
					if (resort[i] == new_key){
						idx = i;
					}
				}
				
				int loc = lookup_db(idx);
				int count = 1;
				string myblock;
				int nexsize;
				
				// read the block
				ifstream readDB(writefile);
				while(getline(readDB, str)){
					if (count == loc){
						myblock = str;
						break;
					}
					else{
						count++;
					}
				}
				
				// test block size: either insert into a curren block or create a new block
				nexsize = myblock.length() + new_rec.length();
				if (nexsize > 1024){
					int maxblock = 0;
					int bt_size = block_table.size();
					for (i = 0; i < bt_size; i++){
						if (maxblock < block_table[i]){
							maxblock = block_table[i];
						}
					}
					maxblock++;
					block_table.push_back(maxblock);
					int newptr = new_rec.length() + 4 + block_addr[block_addr.size()-1];
					block_addr.push_back(newptr);
					
					// create and insert the new block
					ofstream outfile(writefile, ios::app);
					outfile << "\n" << new_key << "\t" << new_field.length() << "\t" << new_field << "\t";
					outfile.close();
					cout << "New block created as previous block is full!" << endl;
					cout << "New block contains the record: \n" << new_field.length() << "\t" << new_key << "\t" << new_field << endl;
					
				}
				
				else{
					int k;
					string neblock = myblock;
					int newptr = new_rec.length() + 5;
					neblock.append(to_string(new_field.length()));
					neblock.append(" ");
					neblock.append(new_key);
					neblock.append(" ");
					neblock.append(new_field);
					neblock.append(" ");
					block_table.push_back(loc);
					
					// insert the block
					string astr;
					vector<string> blocks;
					ifstream inner(writefile);
					while(getline(inner, astr)){
						blocks.push_back(astr);
					}
					blocks[count] = neblock;
					ofstream outer;
					outer.open(writefile);
					int fsize = blocks.size();
					for (i = 0; i < fsize; i++){
						outer << blocks[i] << "\n";
					}
					outer.close();
					cout << "New record insert complete!!" << endl;
					cout << "Insertion done in block number: " << count << endl;
					// modify the address pointers
					int bladdr = block_addr.size();
					for (k = loc; k < bladdr; k++){
						block_addr[k] = block_addr[k] + newptr;
					}
				}
				cout << "Insert into database file complete!!" << endl;
				cout << "Insertion of key to B+Tree complete, adjusted tree is: " << endl;
				show_tree(root, false);
			}
		}
		
		else if (ch4 == 'd'){
			string rem_key;
			cout << "Enter key to be deleted..." << endl;
			cin >> rem_key;
			int fi = search(root, rem_key);
			if (fi == 1){
				cout << "Key is found in tree..." << endl;
				cout << "Key will now be deleted..." << endl;
				delete_key(root, rem_key);
				show_tree(root, false);
				cout << "Key " << rem_key << " is deleted!" << endl;
			}
			else{
				cout << "Key to be deleted is not present in database, deletion not possible!" << endl;
			}
		}
		
		else if (ch4 == 's'){
			string sea_key;
			cout << "Enter record key to search in the B+Tree..." << endl;
			cin >> sea_key;
			int cur_bl;
			int k, id, sk = all_keys.size();
			for (k = 0; k < sk; k++){
				if (all_keys[k] == sea_key){
					id = k;
					break;
				}
			}
			show_tree(root, false);
			cur_bl = lookup_db(id);
			int fi = search(root, sea_key);
			if (fi == 1){
				cout << "Record key is present in B+Tree in the database in block no: " << cur_bl << endl;
				
				ifstream inp(writefile);
				int count;
				string str;
				while(getline(inp, str)){
					if (count == id){
						cout << "BLOCK: " << id << " : " << str << endl;
						break;
					}
					else{
						count++;
					}
				}
			}
			else{
				cout << "Record key absent in B+Tree, so the record is not in the dataase!" << endl;
			}
		}
		
		else if (ch4 == 'v'){
			cout << "Current B+Tree consists of..." << endl;
			show_tree(root, false);
		}
		
		else if (ch4 == 'k'){
			cout << "Current list of keys in the tree..." << endl;
			int i, kl = all_keys.size();
			for (i = 0; i < kl; i++){
				cout << all_keys[i] << " ";
			}
			cout << endl;
		}
		
		else if (ch4 == 'r'){
			string st, ed;
			int i, al = all_keys.size();
			int id1, id2;
			cout << "NOTE: make sure starting key is before the ending! (sorted)" << endl;
			cout << "Enter starting key of range search...";
			cin >> st;
			cout << endl << "Enter ending key of range search...";
			cin >> ed;
			cout << endl;
			
			vector<string> resort = sort_strings(all_keys);
			for (i = 0; i < al; i++){
				if (resort[i] == st){
					id1 = i;
				}
				if (resort[i] == ed){
					id2 = i;
				}
			}
			
			if (id1 < id2){
				cout << "Range Search between " << st << " and " << ed << endl;
				for (i = id1; i <= id2; i++){
					cout << resort[i] << " ";
				}
				cout << endl;
			}
			else{
				cout << "Starting and ending keys not mentioned properly!" << endl;
			}
		}
		
		else{
			continue;
		}
			
	}while(ch4 != 'c');	
			
	return 0;
}
