/* Caroline Rinks
 * Lab 4
 * CS302 Spring 2021
 *
 * PPM::read() and PPM::write() function implementations
*/

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;

#include "PPM.h"

/* @brief: Processes a P6 PPM image using binary file I/O 
 * @param fname_in: The name of the image to process. */
void PPM::read(const string &fname_in) { 
	
	ifstream fin;
	string magicid;
	
	fin.open(fname_in.c_str(), std::ios::binary);
	if(!fin.is_open()){
		cerr << "Could not open file to read from.\n";
		exit(EXIT_FAILURE);
	}

	/* Store P6 header info */
	fin >> magicid;
	if (magicid != "P6"){
		cerr << "File not of P6 type.\n";
		exit(EXIT_FAILURE);
	}
	
	fin >> width >> height >> max_color;

	if (max_color != 255){
		cerr << "Invalid maximum color value.\n";
		exit(EXIT_FAILURE);
	}

	/* Allocate memory to store image */	
	img = new RGB*[height];
	img[0] = new RGB[width*height];

	for (int i = 1; i < height; i++) {
		img[i] = img[0] + i*width;	// set 1st pixel of each row
	}
   
	while (fin.get() != '\n') {}

	int nrgb = width;
	int pix_read;					// pixels read per row
	int total_read = 0;				// total pixels read
	int column = 0, row = 0;
	unsigned char buffer[3*width];

	/* Read image data row by row */
	while (1) {
		fin.read((char *)buffer, 3*nrgb);
		pix_read = fin.gcount() / 3;

		if (pix_read == 0 && fin.eof()){
			break;
		}
		
		column = 0;
		for (int i = 0; i < 3*nrgb; i += 3){
			img[row][column].R = *(buffer + i);
			img[row][column].G = *(buffer + i + 1);
			img[row][column].B = *(buffer + i + 2);
			++column;
		}
		total_read += pix_read;
		++row;
	}                                              

	if (total_read != width*height){
		cerr << fin.gcount() << " bytes read. Target: " << width*height << '\n';
		exit(EXIT_FAILURE);
	}
	fin.close();
}

/* @brief: Creates a new P6 PPM image file.
 * @param: fname_in The name of the image used. */
void PPM::write(const string &fname_in) { 
	ofstream fout;
	string fname_out = fname_in;
	
	/* Create name for new ppm image */
	fname_out.erase(fname_out.end()-4, fname_out.end());
	fname_out += "_wmsg.ppm";	

	fout.open(fname_out.c_str(), std::ios::binary);
	if (!fout.is_open()){
		cerr << "Could not open file to write to.\n";
		exit(EXIT_FAILURE);
	}

	/* Write P6 header info */
	fout << "P6" << '\n';
	fout << width << " " << height << '\n';
	fout << max_color << '\n';

	/* Write image data to new file */
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			fout << img[i][j].R;
			fout << img[i][j].G;
			fout << img[i][j].B;
		}
	}
	fout.close();
}
