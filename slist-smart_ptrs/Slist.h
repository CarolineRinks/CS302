/* Caroline Rinks
 * Lab 3
 * CS302 Spring 2021
 *
 * Header file containing a single-linked list class declaration and implementation.
 * Also implements a smart pointer and iterator class.
*/

#ifndef SLIST_H
#define SLIST_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

template <class T>
class slist {
	private:
		struct node {
			node() { data = T(); next = NULL; }
			node(const T &key) { data = key; next = NULL; }
			bool operator<(const node &rhs) { return (data < rhs.data); }

			T data;
			node * next;
		};

	private:
		class sptr {
			public:    
				sptr(node *_ptr=NULL) { ptr = _ptr; }    
				bool operator< (const sptr &rhs) const { return *ptr < *rhs.ptr; }    
				operator node * () const { return ptr; }  
			
			private:    
				node *ptr;
		};

	public:
		class iterator {
			public:
				iterator() { p = NULL; }
				T & operator*() { return p->data; }
				iterator & operator++() { p = p->next; return *this; }
				bool operator!=(const iterator & rhs) const { return p != rhs.p; }

			private:
				iterator(node * n_p) { p = n_p; }
				node * p;

				friend class slist<T>;
		};

	public:
		slist();
		~slist();

		void push_back(const T &);
		void sort(const string &);

		iterator begin() { return iterator(head->next); }
		iterator end() { return iterator(NULL); }

	private:
		node * head;
		node * tail;
};

template <typename T> 
slist<T>::slist() {
	head = new node();
	tail = head;
}

template <typename T>
slist<T>::~slist() {
	node * p;
	while (head->next != NULL) {
		p = head->next;
		head->next = p->next;
		delete p;
	}
	delete head;

	head = NULL;
	tail = NULL;
}

template <typename T>
void slist<T>::push_back(const T &din) {
	tail->next = new node(din);
	tail = tail->next;
}

template <typename T>
void slist<T>::sort(const string &algname) {

	int count;
	node * p;
	vector<sptr> Ap;	// Smart-Pointer vector

	/* Count elements in list */
	count = 0;
	p = head->next;
	while (p != NULL) {
		++count;
		p = p->next;
	}

	/* Store pointers in smart-pointer vector */
	Ap.resize(count);
	p = head;
	for (int i = 0; i < count; i++) {
		Ap[i] = p->next;
		p = p->next;
	}

	/* Sort smart-pointer vector using specified algorithm */
	if (algname == "-quicksort") {
		std::sort(Ap.begin(), Ap.end());
	}
	else if (algname == "-mergesort") {
		stable_sort(Ap.begin(), Ap.end());
	}
	else {
		cerr << "usage: ./Slist -quicksort|-mergesort file.txt\n";
		exit(1);
	}

	/* Copy sorted smart-pointer vector to list */
	p = head;
	for (int i = 0; i < count; i++){
		p->next = Ap[i];
		p = p->next;
	}
	p->next = NULL;
}

#endif // SLIST_H
