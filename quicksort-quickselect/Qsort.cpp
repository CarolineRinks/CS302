/* Caroline Rinks
 * Lab 2
 * CS302 Spring 2021
 *
 * This program implements the Quicksort and Quickselect algorithms. Data is read from a file 
 * (firstname, lastname, and phone number), and stored in a vector. The vector is then sorted 
 * lexicographically using the sorting algorithm specified by the user (std::sort(), quicksort, 
 * or quickselect) and the resulting sorted list is printed to the user.
 * Quicksort and Quickselect are both implemented using Random Pivot Selection.
*/

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>
using namespace std;

class data {
	public:
		bool operator<(const data &) const;
		friend istream & operator>>(istream &, data &);
		friend ostream & operator<<(ostream &, const data &);

	private:
		string firstname;
		string lastname;
		string phonenum;
};

bool data::operator<(const data &rhs) const{
	if (lastname != rhs.lastname){
		return (lastname < rhs.lastname);
	}
	else if (firstname != rhs.firstname){
		return (firstname < rhs.firstname);
	}
	else{
		return (phonenum < rhs.phonenum);
	}
}

istream & operator>>(istream &in, data &r) { 
	in >> r.firstname >> r.lastname >> r.phonenum;	
	return in;
}

ostream & operator<<(ostream &out, const data &r) {
	out << setw(23) << left << (r.lastname + ", " + r.firstname) << " " << r.phonenum << '\n';
	return out;
}

template <typename T>
void quickselect(vector<T> &vec, int left, int k, int right) {  
	int pindex, i, j, end;
	T pivot;

	while (1) {    
		/* Select Random Pivot */
		pindex = left + (rand() % (right - left + 1));
		pivot = vec[pindex];

		/* partition vec like {<=}, {pivot}, {=>} */
		swap(vec[pindex], vec[right]);

		i = left - 1;
		j = right;
		end = right;

		while (1) {
			while (++i != end && vec[i] < pivot) { }
			if (i == end) {
				break;
			}

			while (--j != -1 && pivot < vec[j]) { }
			if (j == -1) {
				break;
			}

			if (i >= j) {
				break;
			}
			swap(vec[i], vec[j]);
		}

		pindex = i;
		swap(vec[pindex], vec[right]);
		
		if (pindex == k) {	// stopping condition
			return;
		}

		if (k < pindex){ 
			right = pindex-1;    
		}
		else {            
			left = pindex+1;  
		}
	}
}

template <typename T>
void quicksort(vector<T> &vec, int left, int right) {  
	int pindex, i, j, end;
	T pivot;
	
	if (left >= right) {	// stopping condition
		return;
	}
	
	/* Select random pivot */
	pindex = left + (rand() % (right - left + 1));	
	pivot = vec[pindex];

	/* Partition vec into: {<=}, {pivot}, {=>} */	
	swap(vec[pindex], vec[right]);

	i = left - 1;   
	j = right;   
	end = right;

	while (1) {    
		while (++i != end && vec[i] < pivot) { } 
		if (i == end) {
			break;
		}

		while (--j != -1 && pivot < vec[j]) { }
		if (j == -1) {
			break;
		}
		
		if (i >= j) { 
			break;
		}
		swap(vec[i], vec[j]);  
	}  
	
	pindex = i;  
	swap(vec[pindex], vec[right]);		//pivot is now in correct position

	quicksort(vec, left, pindex-1);
	quicksort(vec, pindex+1, right);
}

void printlist(vector<data>::iterator p0, vector<data>::iterator p1) { 
	while (p0 != p1){
		cout << *p0;
		++p0;
	}
}

int main(int argc, char *argv[]) {
	
	ifstream fin;
	data din;
	string mode;
	int N, k0, k1; 
	vector<data> A;

	if ( (argc < 3) || (strstr(argv[argc-1], ".txt") == NULL) ){
		cerr << "usage: ./Qsort -stl|-rpivot [k0 k1] file.txt\n"; 
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

	/* Sort data according to mode specified by user */
	if (mode == "-stl"){
		sort(A.begin(), A.end());
	} 
	else { 
		if (mode == "-rpivot"){
			N = (int)A.size();

			k0 = 0;
			k1 = N-1;

			/* if specified, update k0, k1 and apply quickselect 
			 * to ensure that A[0:k0-1] <= A[k0:k1] <= A[k1+1:N-1] */
			if (argc == 5){
				k0 = atoi(argv[2]);
				k1 = atoi(argv[3]);
				
				if (k0 < 0){
					k0 = 0;
				}
				else if (k1 >= N){
					k1 = N-1;
				}
				quickselect(A, 0, k0, N-1);
				quickselect(A, k0, k1, N-1);
			}
			quicksort(A, k0, k1);
		}
	}
	
	printlist(A.begin(), A.end());

	return 0;
}
