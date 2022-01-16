/* Caroline Rinks
 * Lab 4
 * CS302 Spring 2021
 *
 * RGB struct and PPM class definitions
*/

#ifndef __PPM_H__
#define __PPM_H__

#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

struct RGB { 
	RGB() { R = 0; G = 0; B = 0; }

	unsigned char R, G, B;
};

class PPM { 
	public:
		PPM() { img = NULL; width = 0; height = 0; max_color = 0; }
		~PPM() { delete [] img[0]; delete [] img; }

		void read(const string &);
		void write(const string &);	

		RGB *operator[](int i) { return img[i]; }

		int get_Nrows() { return height; }
		int get_Ncols() { return width; }

	private:
		int width;
		int height;
		int max_color;

		RGB **img;
};

#endif
