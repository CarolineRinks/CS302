/* Caroline Rinks
 * Lab 5 Part 1
 * CS302 Spring 2021
 *
 * Implements a Friend network using dense adjacency matrices
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
using namespace std;

/* @brief: Randomly establishes people as friends.
 * @param names: List of all people.
 * @param friends: Matrix that shows friends.
 * @param M0: The minimum number of friends a person can have.
 * @param M1: The maximum number of friends a person can have. */
void set_oldfriends(vector <string> &names, vector < vector<int> > &friends, int M0, int M1) {
	int N = (int)names.size();
	int num_friends, name_i;
	
	friends.assign(N, std::vector<int>(N, 0));

	for (int i = 0; i < N; i++) {
		/* Create a set for each person that holds the indexes of people they are friends with */
		set<int> doknow;

		num_friends = M0 + rand() % M1;	// calculate a random number of friends

		/* Repeatedly select a random name to add to person's friend list
		 * until their number of friends has been reached */
		while ((int)doknow.size() < num_friends) {
			name_i = rand() % N;
			while (name_i == i) {
				name_i = rand() % N;
			}
			doknow.insert(name_i);
		}
		
		/* Update matrix entries to show friends */
		set<int>::iterator it = doknow.begin();
		while (it != doknow.end()){
			friends[i][*it] = 1;
			friends[*it][i] = 1;
			++it;
		}
	}
}

/* @brief: Creates new friends by connecting each person with friends of their friends.
 * @param friends: A 2D vector that shows already established friends.
 * @param new_friends: A 2D vector that shows potential new friends. */
void set_newfriends(vector < vector<int> > &friends, vector < vector<int> > &new_friends) {
	
	int N = (int)friends.size();
	new_friends.assign(N, std::vector<int>(N, 0));

	/* Find two people who are friends, indicated by friends[i][j] == 1 where
	 * person i and person j are friends. Find all friends of person j and make
	 * them friends of person i */
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (friends[i][j] == 1) {	
				for (int k = 0; k < N; k++) {
					if (friends[j][k] == 1) {
						if (k != i && friends[i][k] != 1){
							new_friends[i][k] = 1;
							new_friends[k][i] = 1;
						}
					}
				}
			}
		}
	}
}

/* @brief: Creates a file with relationship info for each person
 * @param fname: Name of the file to create, either "doknow.txt" or "mightknow.txt"
 * @param names: List of all people
 * @param vec: The dense adjacency matrix that shows if two people are friends */
void writetofile(const char *fname, vector<string> &names, vector < vector<int> > &vec) {
	
	ofstream fout;
	int count, len, max_len = 0;
	int N = (int)names.size();
	
	fout.open(fname);
	
	/* Determine length of longest name for formatting */
	for (int i = 0; i < N; i++){
		len = (int)strlen(names[i].c_str());
		if (len > max_len){
			max_len = len;
		}
	}

	/* Write each person and their friends or potential friends to the file */ 
	for (int i = 0; i < N; i++) {
		fout << setw(max_len) << left << names[i] << " :";
		count = 8;

		for (int j = 0; j < N; j++){
			if (vec[i][j] == 1){
				if (count == 0){
					fout << '\n' << setw(max_len) << left << names[i] << " :";
					count = 8;
				}
				fout << " " << setw(max_len) << names[j];
				--count;
			}
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
	else{
		cerr << "usage: cat datafile.txt | Friendnet1 [-seed N]\n";
		return -1;
	}

	srand(seed);

	while (cin >> name){
		names.push_back(name);
	}

	int M0 = 1; // min number of friends
	int M1 = 3; // max number of friends

	vector < vector<int> > friends;
	vector < vector<int> > new_friends;

	set_oldfriends(names, friends, M0, M1);
	writetofile("doknow1.txt", names, friends);

	set_newfriends(friends, new_friends);
	writetofile("mightknow1.txt", names, new_friends);

	return 0;
}
