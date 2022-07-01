/* Caroline Rinks
 * Lab 4
 * CS302 Spring 2021
 *
 * Defines a random number generator with a non-uniform, data driven
 * distribution.
*/

#ifndef __RNUMGEN_H__
#define __RNUMGEN_H__

#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <numeric>
using namespace std;

class rnumgen {
	public:
		rnumgen(vector<int> &, int seed=0);

		int rand();

	private:
		vector<float> F;
};

#endif
