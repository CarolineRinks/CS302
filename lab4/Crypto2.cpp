#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

#include "PPM.h"
#include "Rnumgen.h"

const char ETX = 0x3;

struct pixel {
	pixel(int r=0, int c=0) { row = r; col = c; }
	int row;
	int col;
};

/* @brief: Populates a vector with all pixels and creates a histogram of 12-bit pixel colors. 
 * @param img: A PPM object that stores the PPM image
 * @param pixel_list: The vector to populate with pixels
 * @param N: Seed for the random number generator 
*/
void set_pixel_list(PPM &img, vector<pixel> &pixel_list, const int N) {
	int twelve_bit, temp;
	int rows = img.get_Nrows();
	int cols = img.get_Ncols();

	/* Create a histogram with color values of all pixels */
	vector<int> histogram;
	histogram.resize(4096, 0);

	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			/* Add pixel to pixel_list */
			pixel n(i, j);
			pixel_list.push_back(n);
			
			/* Create 12-bit color */
			twelve_bit = 0x000;
			temp = 0x000;
			
			temp = ((img[i][j].R)>>3);	// Extract R6...R3 bits
			temp &= 0x0F;
			twelve_bit |= (temp<<8);
			
			temp = ((img[i][j].G)>>3);	// Extract G6...G3 bits
			temp &= 0x0F;
			twelve_bit |= (temp<<4);
			
			temp = ((img[i][j].B)>>3);	// Extract B6...B3 bits
			temp &= 0x0F;
			twelve_bit |= temp;

			histogram.at(twelve_bit) += 1;	// Add occurence of 12-bit color to histogram
		}
	}

	rnumgen RNG(histogram, N);	// instantiate random number generator

	int r1_12, r2_12, r_24;

	for (int i = (int)pixel_list.size()-1; i > 0; --i) {    
		/* Form 24-bit number from 2 randomly picked 12-bit numbers */
		r1_12 = RNG.rand();
		r2_12 = RNG.rand();
		r_24 = (r1_12 << 12) | (r2_12);
		
		swap(pixel_list[i], pixel_list[r_24 % (i+1)]);  // Permute pixel_list
	}
}

/* @brief: Encodes a message into a PPM image character by character by     
 *    replacing the LSB of a pixel with a bit from the character being encoded.
 * @param img: A PPM object used to store the PPM image.
 * @param pixel_list: A vector of pixels to encode the message into. */
void encode(PPM &img, vector<pixel> &pixel_list) {
	int r, c, color, pix_i = 0;
	char bit;

	while (cin.get(k)) {
		if (pix_i >= (int)pixel_list.size()){
			cerr << "Ran out of pixels to encode.\n";
			exit(EXIT_FAILURE);
		}
		
		for (int i = 0; i < 7; i++){
			bit = (bit>>i) & 0x01;

			r = (pixel_list.at(pix_num)).row;
			c = (pixel_list.at(pix_num)).col;
			
			color = pix_num % 3; // r=0, g=1, b=2

			/* Replace LSB of color value with bit extracted from character */
			if (bit == 0){
				(*(((unsigned char *)&img[r][c]) + color)) &= 0xFE;
			}
			else if (bit == 1){
				(*(((unsigned char *)&img[r][c]) + color)) |= 0x01;
			}
			else{
				cerr << "Invalid bit for lsb.\n";
				exit(EXIT_FAILURE);
			}
			++pix_i;
		}
	}
	
	/* Encode ETX character */
	for (int i = 0; i < 7; i++){
		bit = ETX;
		bit = (bit>>i) & 0x01;

		r = (pixel_list.at(pix_num)).row;
		c = (pixel_list.at(pix_num)).col;
	
		color = pix_num % 3;

		if (bit == 0){
			(*(((unsigned char *)&img[r][c]) + color)) &= 0xFE;
		}
		else if (bit == 1){
			(*(((unsigned char *)&img[r][c]) + color)) |= 0x01;
		}
		else{
			cerr << "Invalid bit for ETX character.\n";
			exit(EXIT_FAILURE);
		}	
		++pix_i;
	}
}

/* @brief: Decodes each character of the encoded message by extracting the  
 *	LSB of each encoded pixel and using these bits to rebuild an encode
 *	character bit by bit.
 * @param: img A PPM object that stores the PPM image.
 * @param: pixel_list A list of pixels to decode. 
*/
void decode(PPM &img, vector<pixel> &pixel_list) {
	int r, c, color, pix_i = 0;
	char lsb, character;
	
	while (1) {
		if (pix_i >= (int)pixel_list.size()){
			cerr << "Ran out of pixels to decode.\n";
			exit(EXIT_FAILURE);
		}
		
		character = 0x00;

		for (int i = 0; i < 7; i++){
			r = (pixel_list.at(pix_num)).row;
			c = (pixel_list.at(pix_num)).col;
			
			color = pix_num % 3; // r=0, g=1, b=2
			
			/* Extract the LSB of an encoded pixel and add the bit to 'character' */
			lsb = *(((unsigned char *)&img[r][c]) + color);
			lsb &= 0x01;

			character |= (lsb<<i);

			++pix_i;
		}
		
		if (character == ETX){	//stop decoding when you reach the ETX character
			break;
		}
		cout.put(character);
	}
}

int main (int argc, char *argv[]) {
	
	int seed = 0;
	char * ptr;
	PPM img;
	string file;
	vector<pixel> pixel_list;

	if (argc < 3 || argc > 4){
		cerr << "usage: ./Crypto -encode|-decode [-seed=N] image.ppm\n";
		return -1;
	}

	const string mode = argv[1];
	if (mode != "-encode" && mode != "-decode"){
		cerr << "usage: ./Crypto -encode|-decode [-seed=N] image.ppm\n";
		return -1;
	}
	
	/* Set seed for random number generator (0 by default) */
	if (argc == 4) {
		ptr = argv[2] + 6;
		if (!isdigit(*ptr)){
			cerr << "Invalid seed given.\n";
			cerr << "usage: ./Crypto -encode|-decode [-seed=N] image.ppm\n";
			return -1;
		}
		seed = atoi(ptr);
	}

	file = argv[argc-1];

	img.read(fname);

	set_pixel_list(img, pixel_list, seed);
	
	if (mode == "-encode") {
		encode(img, pixel_list);
		img.write(file);
	}
	else if (mode == "-decode"){ 
		decode(img, pixel_list);
	}

	return 0;
}
