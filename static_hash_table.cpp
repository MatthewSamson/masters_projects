#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

/*
 * STATIC HASH TABLE
 * 
 */
 
// static hash tables
vector<string> s1; //the strings
vector<int> i1; //the indices
vector<int> h1; //hash of str
vector<int> h2; //hash of val from str
vector<int> b1; //binary of hash
vector<vector<string>> vv; // the static table

// database tables
vector<string> all_keys;
vector<int> block_table;
vector<int> block_addr;


// DISPLAY THE FINAL STATIC HASH TABLE  
void show_static(vector<vector<string>> vec){
	int idx0, idx1 = vec.size();
	int i, j;
	cout << endl << endl;
	cout << "--The Static Hash Table--" << endl;
	cout << " Keys " << " Values" << endl;
	for (i = 0; i < idx1; i++){
		cout << "Index " << i << " : ";
		idx0 = vec[i].size();
		for (j = 0; j < idx0; j++){
			if (vec[i][j] != " "){
				cout << vec[i][j] << " -- ";
			}
			else{
				continue;
			}
		}
		cout << endl;
	}
}


// CONVERTS STRING KEYS INTO INTEGER KEY VALUES
int hashings1(string str, int x){
	int m;
	int has = 0; 
	int sum = 0;
	int str_si = str.size();
	
	for (m = 0; m < str_si; m++){
		has = int(str[m]) * pow(x, m);
		//cout << has << " + " ;
		sum = sum + has;
    }
    //cout << " = " << sum << endl;
    return sum;
}


// CONVERTS INTEGER KEY VALUES INTO POSITION WITHIN THE INDEX
int hashings2(int cur_hash, int tablen){
	int mod = 0;
	//cout << cur_hash << " " << tablen << endl;
	mod = cur_hash % tablen;
	//cout << mod << endl;
	return mod;
}


// INSERT CONVERTED VALUES INTO THE HASH TABLE
void make_static(int tablen){
	vector<vector<string>> v1; //the static hash table
	
	cout << "Inserting all the converted values into the static table..." << endl;
	int i, j;
	int si = h2.size();
	
	// moves positions over the table
	for (i = 0; i < tablen; i++){
		vector<string> svec;
		// collects keys for that position in the table
		for (j = 0; j < si; j++){
			if (i == h2[j]){
				svec.push_back(s1[j]);
			}
			else{
				continue;
			}
		}
		v1.push_back(svec);
	}
	::vv = v1;
	show_static(v1);
} 


// SEARCH AND DISPLAY THE STATIC HASH TABLE
int search_static(string s_key, int hash2){
	int i, flag = 0;
	
	vector<string> svec = vv[hash2];
	int slen = svec.size();
	for (i = 0; i < slen; i++){
		if (s_key == svec[i]){
			cout << s_key << " found at index " << hash2 << " of the table" << endl;
			flag = 1;
			break;
		}
		else{
			flag = 0;
			continue;
		}
	}
	return flag;
} 


// REMOVE A KEY FROM THE STATIC HASH TABLE
void remove_static(string s_key, int hash2){
	int i;
	
	vector<string> svec = ::vv[hash2];
	int slen = svec.size();
	for (i = 0; i < slen; i++){
		if (s_key == svec[i]){
			svec[i] = " ";
		}
	}
	::vv[hash2] = svec;
} 



// APPLY THE HASH FUNCTIONS AND INSERT TO TABLE
void do_hash(int hasx, int xval){
	int n;
	int tot1, tot2;
	int h1_val, h2_val;
	tot1 = s1.size();
	for (n = 0; n < tot1; n++){
		h1_val = hashings1(s1[n], xval);
		h1.push_back(h1_val);
	}
	cout << "Hash 1: Function to convert string keys - Complete!" << endl;
	tot2 = h1.size();
	for (n = 0; n < tot2; n++){
		h2_val = hashings2(h1[n], hasx);
		h2.push_back(h2_val);
	}
	cout << "Hash 2: Function to fit keys to hash table - Complete!" << endl;
	make_static(hasx);
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


// MAIN FUNCTION
int main(){
	
	string filename;
	string str;
	char ch1, ch2, ch3;
	vector<string> liststr;
	vector<string> sortdone;
	vector<string> sorted;
	int sr_si, i;
	int tab_si, key_si;
	
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
	
	// creating the database file
	string writefile = "db_static.txt";
	cout << "Database will be written into: " << writefile << endl << endl;
	
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
		
		
		//cout << rec_size << " " << cur_size << endl;
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
		
		s1.push_back(keyval);
		i1.push_back(i);
		block_table.push_back(block_no);
	}
	
	thefile.close();
	cout << "All insertions have been completed!!" << endl;
	cout << "Database file " << writefile << " has been created!!" << endl << endl;
	cout << "Now creating the static hash table..." << endl;
	cout << "Enter number of buckets of the static hash table.... ";
	cin >> tab_si;
	cout << "\nEnter x value to use for the polynomial rolling hashing function (less than 10 gives best distribution).... ";
	cin >> key_si;
	
	while (ch2 == 'n'){
		cout << "Static Hash Table used on: " << s1.size() << " number of records." << endl;
		cout << "Static Hash Table of Size: " << tab_si << endl; 
		cout << "Hashing Function Key: " << key_si << endl;
		cout << "Would you like to continue? (y/n): ";
	    cin >> ch2;
	    cout << "\n\n";
	}
	cout << endl << endl;
	
	// static hash table is being created
	do_hash(tab_si, key_si);
	
	do{
		cout << endl << endl;
		cout << "Press v to view all the keys..." << endl;
		cout << "Press t to view the static hash table..." << endl;
		cout << "Press s to search for a key..." << endl;
		cout << "Press i to add a new record..." << endl;
		cout << "Press d to delete a record..." << endl;
		cout << "Press c to exit..." << endl;
		cin >> ch3;
		cout << endl;
		
		if (ch3 == 'v'){
			int i, si = s1.size();
			for (i = 0; i < si; i++){
				cout << s1[i] << " ";
			}
			cout << endl;
		}
		
		else if (ch3 == 't'){
			cout << "Current Static Hash Table with " << vv.size() << " buckets!";
			show_static(vv);
		}
		
		else if (ch3 == 's'){
			string astr;
			cout << "Enter key to search..." << endl;
			cin >> astr;
			int n, id, ns = s1.size();
			int afh1 = hashings1(astr, key_si);
			int ahf2 = hashings2(afh1, tab_si);
			int f1 = search_static(astr, ahf2);
			if (f1 == 1){
				for (n = 0; n < ns; n++){
					if (s1[n] == astr){
						id = block_table[n];
					}
				}
				cout << "Key " << astr << " is present in the Database at block no. " << id << endl;
				
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
				cout << "Key is not present in table!" << endl;
			}
		}
		
		else if (ch3 == 'i'){
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
			cout << "Testing if key is already in the static hash table..." << endl;
			
			// tree search
			int afh1 = hashings1(new_key, key_si);
			int afh2 = hashings2(afh1, tab_si);
			int sea = search_static(new_key, afh2);
			if (sea == 1){
				cout << "Unable to insert, Record already in the Database!!" << endl;
			}
			
			else{
				// resorting the keys
				cout << "Key not found in the static hash table, Preparing to insert..." << endl;
				all_keys.push_back(new_key);
				vector<string> resort = sort_strings(::all_keys);
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
				
				// apply insertions to the static hash table
				s1.push_back(new_key);
				h1.push_back(afh1);
				h2.push_back(afh2);
				
				// test block size: either insert into a current block or create a new block
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
				cout << "Insertion of key to the static hash table complete, adjusted table is: " << endl;
				make_static(tab_si);
				cout << new_key << " has been inserted at index " << afh2 << " of the table!" << endl; 
			}
		}
		
		else if (ch3 == 'd'){
			string astr;
			cout << "Enter the key whose record is to be deleted..." << endl;
			cin >> astr;
			int afh1 = hashings1(astr, key_si);
			int afh2 = hashings2(afh1, tab_si);
			int fi = search_static(astr, afh2);
			if (fi == 1){
				cout << "Key is found in the static hash table!" << endl;
				cout << "Preparing to delete the key..." << endl;
				remove_static(astr, afh2);
			}
			else{
				cout << "Key not found, cannot delete an absent key!" << endl;
			}
		}
		
		else if (ch3 == 'c'){
			break;
		}
		
		else{
			cout << "Please enter valid option..." << endl;
		}
	}while(ch3 != 'c');
}
