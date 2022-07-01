/* Caroline Rinks
 * Lab 7 (Maze Generator and Solver)
 * CS302 Spring 2021
 *
 * This file generates a maze using disjoint sets. The maze is generated as 
 * a file that holds the locations of walls in the maze. Mazesolve.cpp can 
 * then read this file and solve the maze using Depth-First Search.
*/

#include <cstdlib>
#include <ctime>
#include <cstdio>

#include "dset.h"
#include "dset.cpp"

struct cell { 
	cell() { x = -1; y = -1; }
	cell(int i, int j) { x = i; y = j; }

	void swap(cell &, cell &);

	int get_x() { return x; }
	int get_y() { return y; }

	private:
		int x;
		int y;
};

void cell::swap(cell &c1, cell &c2) { 
	int tmpx = c2.x;
	int tmpy = c2.y;

	c2.x = c1.x;
	c2.y = c1.y;

	c1.x = tmpx;
	c1.y = tmpy;
}

int main(int argc, char *argv[]) {
	
	if (argc != 4) {
		fprintf(stderr, "usage: ./Mazemake nrow ncol maze.txt\n");
		return -1;
	}

	int i, j, k = 0;
	int Nrow = atoi(argv[1]);
	int Ncol = atoi(argv[2]);
	int N =	(Nrow * (Ncol-1)) + (Ncol * (Nrow-1));	//number of interior grid walls
	
	cell wall[N][2];

	/* Populate wall[][] with interior grid walls by setting a wall to the 
	 * right of and below each cell */
	for (i = 0; i < Ncol-1; i++) {
		for (j = 0; j < Nrow; j++) {
			wall[k][0] = cell(i, j);		//current cell
			wall[k][1] = cell(i+1, j);		//right cell
			++k;
		}
	}
	for (i = 0; i < Ncol; i++) {
		for (j = 0; j < Nrow-1; j++) {
			wall[k][0] = cell(i, j);		//current cell
			wall[k][1] = cell(i, j+1);		//cell below
			++k;
		}
	}
	
	/* Shuffle */
	srand(time(NULL));
	for (i = N-1; i >= 0; --i) {    
		k = rand() % (i+1);
		wall[i][0].swap(wall[i][0], wall[k][0]);
		wall[i][1].swap(wall[i][1], wall[k][1]);
	}

	/* Write Maze to file */
	FILE * pFile;
	pFile = fopen(argv[3], "w");
	
	if (pFile == NULL) {
		fprintf(stderr, "Could not open file.\n");
		return -1;
	}

	fprintf(pFile, "MAZE %d %d\n", Nrow, Ncol);

	dset DS(Nrow*Ncol);
	
	/* Generate a maze by merging cells */
	for (k = 0; k < N; k++) {
		/* Convert 2D indexing to 1D indexing */
		i = wall[k][0].get_x() + (wall[k][0].get_y() * Ncol);
		j = wall[k][1].get_x() + (wall[k][1].get_y() * Ncol);
		
		/* Merge cells if they belong to different dsets, otherwise write a wall 
		 * location to file as 2 cell coordinates: i0 j0 i1 j1 */
		if (DS.find(i) != DS.find(j)) {
			DS.merge(i, j);
		}
		else { 
			fprintf(pFile, "%3d %3d %3d %3d\n", wall[k][0].get_y(), wall[k][0].get_x(), 
					wall[k][1].get_y(), wall[k][1].get_x());
		}
		
		if (DS.size() == 1){ // stop merging when all cells belong to same dset
			break;
		}	
	}
	k++;

	/* Write any remaining walls to file */
	for (i = k; i < N; i++) {
		fprintf(pFile, "%3d %3d %3d %3d\n", wall[i][0].get_y(), wall[i][0].get_x(),
				wall[i][1].get_y(), wall[i][1].get_x());
	}
	fclose(pFile);

	return 0;
}
