/* Caroline Rinks
 * Lab 4 Part 1 (Cryptography)
 * CS302 Spring 2021
 *
 * Encode mode encodes a message specified by the user into a copy of a P6 PPM image.
 * Decode mode decodes the message out of the image and presents it to the user.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

#include "PPM.h"

const char ETX = 0x3;

struct pixel {
	pixel(int r=0, int c=0) { row = r; col = c; }
	int row;
	int col;
};

/* @brief: Populates a list with even row, column indexed pixels 
 * @param img: A PPM object that stores the PPM image
 * @param pixel_list: The vector to populate with pixels */
void set_pixel_list(PPM &img, vector<pixel> &pixel_list) {
	int rows = img.get_Nrows();
	int cols = img.get_Ncols();	
	for (int i = 0; i <= rows; i += 2){
		for (int j = 0; j <= cols; j += 2){
			pixel n(i, j);
			pixel_list.push_back(n);
		}
	}
}

/* @brief: Encodes a message into a PPM image character by character by 
 *         replacing the LSB of a pixel with a bit from the character being encoded.
 * @param img: A PPM object used to store the PPM image.
 * @param pixel_list: A vector of pixels to encode the message into. */
void encode(PPM &img, vector<pixel> &pixel_list) {
	int r, c, color, pix_i = 0;
	char bit;
	while (cin.get(bit)) {
		if (pix_i >= (int)pixel_list.size()) {
			cerr << "Ran out of pixels to encode.\n";
			exit(EXIT_FAILURE);
		}
		
		for (int i = 0; i < 7; i++) {
			bit = (bit>>i) & 0x01;
			r = (pixel_list.at(pix_i)).row;
			c = (pixel_list.at(pix_i)).col;
			
			color = pix_i % 3;	// r=0, g=1, b=2

			/* Replace LSB of color value with bit extracted from character */
			if (bit == 0) {
				(*(((unsigned char *)&img[r][c]) + color)) &= 0xFE;
			}
			else if (bit == 1) {
				(*(((unsigned char *)&img[r][c]) + color)) |= 0x01;
			}
			else{
				cerr << "Invalid bit for lsb.\n";
				exit(EXIT_FAILURE);
			}
			++pix_i;
		}
	}
	
	/* Encode the ETX character */
	for (int i = 0; i < 7; i++){
		bit = ETX;
		bit = (bit>>i) & 0x01;
		r = (pixel_list.at(pix_i)).row;
		c = (pixel_list.at(pix_i)).col;
	
		color = pix_i % 3;

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
 *         LSB of each encoded pixel and using these bits to rebuild an encoded 
 *         character bit by bit.
 * @param: img A PPM object that stores the PPM image.
 * @param: pixel_list A list of pixels to decode. */
void decode(PPM &img, vector<pixel> &pixel_list) {
	int r, c, color, pix_i = 0; 
	char lsb, character;
	while (1) {
		if (pix_i >= (int)pixel_list.size()) {
			cerr << "Ran out of pixels to decode.\n";
			exit(EXIT_FAILURE);
		}	
		character = 0x00;

		for (int i = 0; i < 7; i++) {
			r = (pixel_list.at(pix_i)).row;
			c = (pixel_list.at(pix_i)).col;	
			color = pix_i % 3;	// r=0, g=1, b=2
			
			/* Extract the LSB of an encoded pixel and add the bit to 'character' */ 
			lsb = *(((unsigned char *)&img[r][c]) + color);
			lsb &= 0x01;
			character |= (lsb<<i);
			++pix_i;
		}	
		if (character == ETX){	// stop decoding when you reach the ETX character
			break;
		}
		cout.put(character);
	}
}

int main (int argc, char *argv[]) {
	
	PPM img;
	string file;
	vector<pixel> pixel_list;

	if (argc != 3){
		cerr << "usage: ./Crypto -encode|-decode image.ppm\n";
		return -1;
	}

	const string mode = argv[1];
	if (mode != "-encode" && mode != "-decode"){
		cerr << "usage: ./Crypto -encode|-decode image.ppm\n";
		return -1;
	}

	file = argv[argc-1];
	img.read(file);
	set_pixel_list(img, pixel_list);

	if (mode == "-encode") {
		encode(img, pixel_list);
		img.write(file);
	}
	else if (mode == "-decode"){ 
		decode(img, pixel_list);
	}
	return 0;
}
