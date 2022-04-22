/* Caroline Rinks
 * Lab 5 Part 3
 * CS302 Spring 2021
 *
 * Implements a friend network using binary search trees
*/

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <iomanip>
#include <set>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
using namespace std;

/* @brief: Randomly establishes people as friends.
 * @param names: List of all people.
 * @param friends: BST that shows friends.
 * @param M0: The minimum number of friends a person can have.
 * @param M1: The maximum number of friends a person can have. */ 
void set_oldfriends(vector <string> &names, vector < set<int> > &friends, int M0, int M1) {
	int N = (int)names.size();
	int num_friends, name_i;

	friends.resize(N);

	for (int i = 0; i < N; i++) {
		/* Create a set for each person that holds the indexes of people they are friends with */
		set<int> doknow;
	
		num_friends = M0 + rand() % M1;

		/* Repeatedly select a random name to add to person's friend list 
		 * until their number of friends has been reached */
		while ((int)doknow.size() < num_friends) {
			name_i = rand() % N;
			while (name_i == i){
				name_i = rand() % N;
			}
			doknow.insert(name_i);
		}
		
		/* Update BST entires to show friends */
		set<int>::iterator it = doknow.begin();
		while (it != doknow.end()){
			friends[i].insert(*it);
			friends[*it].insert(i);
			++it;
		}
	}
}

/* @brief: Creates new friends by connecting each person with friends of their friends.
 * @param friends: A 2D vector that shows already established friends. 
 * @param new_friends: A 2D vector that shows potential new friends. */
void set_newfriends(vector < set<int> > &friends, vector < set<int> > &new_friends) {
	
	int N = (int)friends.size();
	set<int>::iterator j, k, t;
	bool add;

	new_friends.resize(N);

	/* For each friend j of person i, add friend j's friends to        
	 * person i's list of friends */
	for (int i = 0; i < N; i++) {
		j = friends[i].begin();

		while (j != friends[i].end()) {
			k = friends[*j].begin();	// Friend in friend j's list
			
			while (k != friends[*j].end()) {	
				add = true;

				/* Check if potential friend is already a friend */
				if (friends[i].find(*k) != friends[i].end()) {
					add = false;
				}
				
				if (add == true && i != *k) {
					new_friends[i].insert(*k);
				}
				++k;
			}
			++j;
		}
	}
}

/* @brief: Creates a file with relationship info for each person            
 * @param fname: Name of the file to create, either "doknow.txt" or "mightknow.txt"
 * @param names: List of all people
 * @param vec: The sparce 2D vector */
void writetofile(const char *fname, vector<string> &name, vector < set<int> > &vec) {
	
	ofstream fout;
	int count, len, max_len = 0;
	int N = (int)name.size();
	set<int>::iterator it;
	
	fout.open(fname);
	
	/* Determine length of longest name for formatting */
	for (int i = 0; i < N; i++){
		len = strlen(name[i].c_str());
		if (len > max_len){
			max_len = len;
		}
	}
	
	/* Write each person and their friends or potential friends to the file */
	for (int i = 0; i < N; i++) {
		count = 8;
		fout << setw(max_len) << left << name[i] << " :";

		it = vec[i].begin();
		while (it != vec[i].end()){
			if (count == 0){
				fout << '\n' << setw(max_len) << left << name[i] << " :";
				count = 8;
			}
			fout << " " << setw(max_len) << name[*it];
			--count;
			++it;
		}
		fout << '\n';
	}
	fout.close();
}

int main(int argc, char *argv[]) {

	int seed;
	string name;
	vector<string> names;

	if (argc == 3) {
		seed = atoi(argv[2]);
	}
	else if (argc == 2) {
		seed = 0;
	}
	else if (argc == 1) {
		seed = time(NULL);	
	}
	else {
		cerr << "usage: cat datafile.txt | Friendnet1 [-seed N]\n";
		return -1;
	}

	srand(seed);

	while (cin >> name){
		names.push_back(name);
	}

	int M0 = 1; // min number of friends
	int M1 = 3; // max number of friends

	vector < set<int> > friends;
	vector < set<int> > new_friends;

	set_oldfriends(names, friends, M0, M1);
	writetofile("doknow3.txt", names, friends);

	set_newfriends(friends, new_friends);
	writetofile("mightknow3.txt", names, new_friends);

	return 0;
}
