/* Caroline Rinks
 * Lab 1 Part 1
 * CS302 Spring 2021
 *
 * This program stores fruit purchases in an array, sorts them as specified by the user,
 * and then prints out the sorted list of purchases in an organized format.
*/

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
using namespace std;

struct fruit {
	public:
		bool operator<(const fruit &rhs) const { return (fruitname < rhs.fruitname); }
		void insert(string);
		void print(float &);
		void combine(vector<fruit> &, int);

	private:
		string fruitname;
		float quantity;
		float unit_cost;
};

void fruit::insert(string line) {
	istringstream sin(line);
	sin >> fruitname >> quantity >> unit_cost;
}

void fruit::print(float &total_cost) {	
	
	total_cost += (quantity * unit_cost);

	cout << left << setw(20) << setfill('.') << fruitname << setfill(' ') << " "
		 << setw(5) << fixed << setprecision(2) << right << quantity << " x " << unit_cost << " = " 
		 << setw(7) << right << quantity * unit_cost << "  :"
		 << setw(9) << right << total_cost;
}

void fruit::combine(vector<fruit> &list, int index) {
	while (list[index+1].fruitname == fruitname) {
		quantity += list[index+1].quantity;
		list.erase(list.begin()+index+1);		// erase duplicate fruit from list after combining
	}
}

int main(int argc, char *argv[]) {

	ifstream fin;
	fruit obj;
	string mode, line;
	float total_cost = 0;
	vector<fruit> list;

	if ( (argc != 3) || (strstr(argv[argc-1], ".txt") == NULL) ) {
		cerr << "usage: ./Fruit1 <-inorder|sortall|sortone> <file.txt>\n";
		return -1;
	}

	mode = argv[1];

	fin.open(argv[2]);
	if (!fin.is_open()) {
		cerr << "Could not open file.\n";
		return -1;
	}

	while (getline(fin, line)) {
		obj.insert(line);
		list.push_back(obj);
	}

	fin.close();

	if (mode == "-inorder") {
		// do nothing
	}
	else if (mode == "-sortall") {
		// sort fruit purchases alphabetically
		stable_sort(list.begin(), list.end());
	}
	else if (mode == "-sortone") {
		// sort fruit purchases alphabetically and combine duplicate purchases
		stable_sort(list.begin(), list.end());
		for (unsigned int i = 0; i < list.size()-1; i++) {
			list[i].combine(list, i);
		}
	}
	else {
		cerr << "usage: ./Fruit1 <-inorder|sortall|sortone> <file.txt>\n";
		return -1;
	}

	for (unsigned int i = 0; i < list.size(); i++) {
		list[i].print(total_cost);
		cout << '\n';
	}

	return 0;
}
