/* Caroline Rinks
 * Lab #9
 *
 * Smart pointer class definition and implementation.
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

template <typename T>
class sptr {
	public:    
		sptr(T *_ptr=NULL) { ptr = _ptr; }    
		
		bool operator< (const sptr &rhs) const { return **ptr < **rhs.ptr; }    
		operator T * () const { return ptr; }  
	
	private:    
		T * ptr;
};

template <typename T>
void sptrsort(vector<T *> &A) {
	vector< sptr<T *> > Ap;
	Ap.resize(A.size());
	for (int i = 0; i < (int)A.size(); i++){
		Ap[i] = &A[i];
	}

	sort(Ap.begin(), Ap.end());

	int j, nextj;
	for (int i = 0; i < (int)A.size(); i++){
		if (Ap[i] != &A[i]) {      
			T * tmp = A[i];      
			
			for (j=i; Ap[j] != &A[i]; j=nextj) {        
				nextj = Ap[j] - &A[0];        
				A[j] = *Ap[j];        
				Ap[j] = &A[j];      
			}      
			
			A[j] = tmp;      
			Ap[j] = &A[j];    
		}
	}
}
