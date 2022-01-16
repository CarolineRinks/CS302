/* Caroline Rinks
 * Lab 3
 * CS302 Spring 2021
 *
 * This lab reads data from a file (firstname, lastname, phone number), and stores it in a single-linked
 * list. Using smart-pointers, the list is then sorted via the sorting method specified by the user
 * (quicksort or mergesort) and the data is printed out in an organized manner. 
*/

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

#include "Slist.h"

class data { 
	public:
		data();
		
		bool operator<(const data &) const;
		friend istream & operator>>(istream &, data &);
		friend ostream & operator<<(ostream &, const data &);

	private:
		string firstname;
		string lastname;
		string phonenum;
		int ID;		// keeps track of order data is read in.
};

data::data() {
	firstname = "";
	lastname = "";
	phonenum = "";
	ID = 0;
}

bool data::operator<(const data &rhs) const {
	if (lastname != rhs.lastname) {
		return (lastname < rhs.lastname);
	}
	else if (firstname != rhs.firstname) {
		return (firstname < rhs.firstname);
	}
	else {
		return (phonenum < rhs.phonenum);
	}
}

istream & operator>> (istream &in, data &r) { 
	in >> r.firstname >> r.lastname >> r.phonenum;
	r.ID += 1;
	
	return in;
}

ostream & operator<< (ostream &out, const data &r) { 
	out << " " << setw(23) << left << (r.lastname + " " + r.firstname) << " " << r.phonenum;
    out << setw(9) << right << r.ID << '\n';
	
	return out;
}

void printlist(slist<data>::iterator p0, slist<data>::iterator p1) { 
	while (p0 != p1){
		cout << *p0;
		++p0;
	}
}

int main(int argc, char *argv[]) {
	
	ifstream fin;
	data din;
	string mode;
	slist<data> A;
	
	if ( (argc != 3) || (strstr(argv[argc-1], ".txt") == NULL) ){
		cerr << "usage: ./Slist -quicksort|-mergesort file.txt\n";
		return -1;
	}

	mode = argv[1];

	fin.open(argv[argc-1]);
	if (!fin.is_open()){
		cerr << "Could not open file.\n";
		return -1;
	}

	while (fin >> din){
		A.push_back(din);
	}
	fin.close();

	A.sort(mode);
	printlist(A.begin(), A.end());
}
