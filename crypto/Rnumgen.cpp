/* Caroline Rinks
 * Lab 4
 * CS302 Spring 2021
 *
 * Implementation of rnumgen constructor and rand() method.
 * Produces random numbers with probabilities that are proportional 
 * to the histogram counts.
*/

#include <iostream>
#include <stdlib.h>
#include <vector>
using namespace std;

#include "Rnumgen.h"

/* @brief: Constructor for rnumgen class
 * @param v: The histogram created in Crypto.cpp
 * @param seed: The seed for the random number generator
*/
rnumgen::rnumgen(vector<int> &v, int seed) {
	srand(seed);

	F.resize(v.size());
	partial_sum(v.begin(), v.end(), F.begin());
	transform(F.begin(), F.end(), F.begin(),  bind2nd(divides<float>(), F.back()));
}

int rnumgen::rand() { 
	const double randmax = RAND_MAX+1.0;  
	const double p = (double)std::rand()/randmax;  
	
	return upper_bound(F.begin(), F.end(), p) - F.begin();
}
