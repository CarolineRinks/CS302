/* Caroline Rinks
 * Lab 5 Part 2
 * CS302 Spring 2021
 *
 * Implements a Friend network using a Sparse 2D vector
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
 * @param friends: A sparse 2D vector that holds lists of friends.
 * @param M0: The minimum number of friends a person can have.
 * @param M1: The maximum number of friends a person can have. */  
void set_oldfriends(vector <string> &name, vector < vector<int> > &friends, int M0, int M1) {
	int N = (int)name.size();
	int num_friends, name_i;

	friends.resize(N);

	for (int i = 0; i < N; i++) {
		/* Create a set for each person that holds the indexes of people they are friends with */	
		set<int> doknow;
	
	num_friends = M0 + rand() % M1;	// calculate a random number of friends
		/* Repeatedly select a random name to add to person's friend list
		 * until their number of friends has been reached */
		while ((int)doknow.size() < num_friends) {
			name_i = rand() % N;
			while (name_i == i){
				name_i = rand() % N;
			}
			doknow.insert(name_i);
		}

		/* Add friends to person's friends list */
		set<int>::iterator it = doknow.begin();
		while (it != doknow.end()){
			friends[i].push_back(*it);
			friends[*it].push_back(i);
			++it;
		}
	}

	/* Sort person's friends list and remove duplicates */
	vector<int>::iterator it;
	for (int i = 0; i < N; i++){
		sort(friends[i].begin(), friends[i].end());
		it = unique(friends[i].begin(), friends[i].end());
		friends[i].resize(it - friends[i].begin());
	}
}

/* @brief: Creates new friends by connecting each person with friends of their friends.
 * @param friends: A 2D vector that shows already established friends. 
 * @param new_friends: A 2D vector that shows potential new friends. */
void set_newfriends(vector < vector<int> > &friends, vector < vector<int> > &new_friends) {
	
	int N = (int)friends.size();
	vector<int>::iterator j;
	vector<int>::iterator k;
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
				
				/* Check if potential friend is already your friend */
				for (vector<int>::iterator t = friends[i].begin(); t != friends[i].end(); t++){
					if (*k == *t) {
						add = false;
						break;
					}
				}

				if (add == true && i != *k){
					new_friends[i].push_back(*k);
				}
				++k;
			}
			++j;
		}
	}

	/* Sort friends and remove duplicates */
	vector<int>::iterator it;
	for (int i = 0; i < N; i++){
		sort(new_friends[i].begin(), new_friends[i].end());
		it = unique(new_friends[i].begin(), new_friends[i].end());
		new_friends[i].resize(it - new_friends[i].begin());
	}
}

/* @brief: Creates a file with relationship info for each person       
 * @param fname: Name of the file to create, either "doknow.txt" or "mightknow.txt"                                                         
 * @param names: List of all people
 * @param vec: The sparce 2D vector */
void writetofile(const char *fname, vector<string> &names, vector < vector<int> > &vec) {
	
	ofstream fout;
	int count, len, max_len = 0;
	int N = (int)names.size();
	vector<int>::iterator it;
	
	fout.open(fname);
	
	/* Determine length of longest name for formatting */
	for (int i = 0; i < N; i++){
		len = strlen(names[i].c_str());
		if (len > max_len){
			max_len = len;
		}
	}

	/* Write each person and their friends or potential friends to the file */
	for (int i = 0; i < N; i++) {
		count = 8;
		fout << setw(max_len) << left << names[i] << " :";

		it = vec[i].begin();
		while (it != vec[i].end()){
			if (count == 0){
				fout << '\n' << setw(max_len) << left << names[i] << " :";
				count = 8;
			}
			fout << " " << setw(max_len) << names[*it];
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
	
	while (cin >> name) {
		names.push_back(name);
	}

	int M0 = 1; // min number of friends
	int M1 = 3; // max number of friends

	vector < vector<int> > friends;
	vector < vector<int> > new_friends;

	set_oldfriends(names, friends, M0, M1);
	writetofile("doknow2.txt", names, friends);

	set_newfriends(friends, new_friends);
	writetofile("mightknow2.txt", names, new_friends);

	return 0;
}
