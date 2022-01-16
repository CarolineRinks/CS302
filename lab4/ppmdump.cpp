#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
using namespace std;

void ppmdump(istream &fin) {  
	string magicid;  
	int ncols, nrows;  
	int maxvalue;  
	
	fin >> magicid >> ncols >> nrows >> maxvalue;  
	
	cout << magicid << "\n";  
	cout << ncols << " " << nrows << "\n";  
	cout << maxvalue << "\n";  
	
	while (fin.get() != '\n') { /* skip past newline */ }  
	int nrgb = 5;         // pixels per line  
	int nrgb_read;        // pixels read per line  
	int npixels_read = 0; // pixels read in total  
	
	unsigned char *rgb_ptr;       // data pointer  
	unsigned char buf[3*nrgb];    // data buffer  
	
	char text[80];        // text buffer  
	
	while (1) {    
		fin.read((char *)buf, 3*nrgb);    
		nrgb_read = fin.gcount()/3;    
		
		if (nrgb_read == 0 && fin.eof())      
			break;    
		
		sprintf(text, "%07d ", npixels_read);     
		cout << text;    
		
		rgb_ptr = buf;    
		for (int i=0; i<nrgb_read; i++) {      
			cout << " ";      
			for (int j=0; j<3; j++) {        
				sprintf(text, " %03u", *rgb_ptr++);        
				cout << text;      
			}    
		}    
		cout << '\n';    
		
		npixels_read += nrgb_read;  
	}
}

int main (int argc, char *argv[]) {  
	if (argc != 2) {    
		cerr << "usage: " << argv[0] << " input.ppm\n";    
		return 0;  
	}  
	
	ifstream fin;  
	fin.open(argv[1]);  
	
	ppmdump(fin);  
	
	fin.close();

	return 0;
}	
