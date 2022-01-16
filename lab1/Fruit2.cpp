/* Caroline Rinks
 * Lab 1 Part 2
 * CS302 Spring 2021
 *
 * This program stores fruit purchases in the order specified by the user using 
 * a double-linked list and then prints out the sorted list of purchases in an 
 * organized format.
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
		/* Overloaded operators */
		bool operator<(const fruit &rhs) const { return (fruitname < rhs.fruitname); }
		bool operator>(const fruit &rhs) const { return (fruitname > rhs.fruitname); }
		bool operator==(const fruit &rhs) const { return (fruitname == rhs.fruitname); }
		void operator+=(const fruit &add) { quantity = quantity + add.quantity; }
		
		/* Support functions */
		void insert(string &);
		void print(float &);

	private:
		string fruitname;
		float quantity;
		float unit_cost;
};

struct node {
	node(string);

	fruit data;
	node *next;
	node *prev;
};

node::node(string line){
	next = NULL;
	prev = NULL;
	data.insert(line);
}

void fruit::insert(string &line){
	istringstream sin(line);
	sin >> fruitname >> quantity >> unit_cost;
}

void fruit::print(float &total_cost){		
	total_cost += (quantity * unit_cost);

	cout << left << setw(20) << setfill('.') << fruitname << setfill(' ') << " "
		 << setw(5) << fixed << setprecision(2) << right << quantity << " x " << unit_cost << " = " 
		 << setw(7) << right << quantity * unit_cost << "  :"
		 << setw(9) << right << total_cost << '\n';
}

int main(int argc, char *argv[]){

	ifstream fin;
	string mode, line;
	node * head, * p1, * p2;
	float total_cost = 0;

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

	head = new node("");

	if (mode == "-inorder"){
		/* Store data in order read from file */
		p1 = head;

		while (getline(fin, line)){

			p2 = new node(line);	

			/* Append new node to end of list */
			p2->next = head;
			p2->prev = p1;
			p1->next = p2;
			head->prev = p2;

			p1 = p2; // advance temporary ptr to end of list
		}
	}
	else if (mode == "-sortall" || mode == "-sortone"){
		/* Store data in alphabetical order */
		while (getline(fin, line)){

			p2 = new node(line);

			/* insert new node into correct position in list */
			if (head->next == NULL) {
				/* List is empty */
				head->next = p2;
				head->prev = p2;
				p2->next = head;
				p2->prev = head;
			}
			else if (p2->data > head->prev->data) {
				/* Add to end of list */
				head->prev->next = p2;
				p2->prev = head->prev;
				p2->next = head;
				head->prev = p2;
			}
			else {
				/* Find correct spot to insert */
				p1 = head->next;
				while (true) {
					if (p2->data == p1->data) {
						if (mode == "-sortone") {
							/* Combine duplicate fruit purchases */
							p1->data += p2->data;
							delete p2;
							break;
						}
						else {
							/* insert after last instance of the fruit */
							while (p1->next->data == p2->data) {
								p1 = p1->next;
							}
							p2->next = p1->next;
							p1->next->prev = p2;
							p1->next = p2;
							p2->prev = p1;
							break;
						}
					}
					else if (p2->data < p1->data) {
						p1->prev->next = p2;
						p2->prev = p1->prev;
						p2->next = p1;
						p1->prev = p2;
						break;
					}
					p1 = p1->next;
				}
			}
		}
	}
	else {
		cerr << "usage: ./Fruit1 <-inorder|sortall|sortone> <file.txt>\n";
		return -1;
	}

	fin.close();

	/* Output list content */
	p1 = head->next;
	while (p1 != head) {
		p1->data.print(total_cost);
		p1 = p1->next;
	}

	/* Free allocated memory */
	while (head->next != head) {
		p1 = head->next;
		head->next = p1->next;
		head->next->prev = head;

		delete p1;
	}
	delete head;

	return 0;
}
