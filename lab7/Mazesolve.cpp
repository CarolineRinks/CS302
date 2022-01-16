/* Caroline Rinks
 * Lab 7 (Maze Generator and Solver)
 * CS302 Spring 2021
 *
 * Reads a file that describes a maze, finds a path through the maze
 * using a Depth First Search, and writes the path to another file.
*/

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

typedef enum direction { LEFT, UP, RIGHT, DOWN } DIR_T;

struct cell { 
	public:
		cell() { x = -1; y = -1; }
		cell(int i, int j) { x = i; y = j; }

		bool solve(int, int, cell &, cell &, cell *, bool *, bool *, int &);

		int get_x() { return x; }
		int get_y() { return y; }

	private:
		int x;
		int y;
};

/* @brief: Returns the direction cell x2,y2 is from cell x1,y1.
 * @param x1: x-coordinate of first cell
 * @param y1: y-coordinate of first cell
 * @param x2: x-coordinate of second cell
 * @param y2: y-coordinate of second cell
 * @return: LEFT, UP, RIGHT, or DOWN */
DIR_T get_direction(int x1, int y1, int x2, int y2) {
	if (y1 == y2) { 	
		if (x2 > x1) {
			return RIGHT;
		}
		else if (x2 < x1) {
			return LEFT;
		}
	}
	else if (x1 == x2) {
		if (y2 > y1) {
			return DOWN;
		}
		else if(y2 < y1) {
			return UP;
		}
	}
}

/* @brief: Finds a path from source to sink using DFS
 * @param Nrow: Number of rows
 * @param Ncol: Number of columns
 * @param current: The current cell being explored
 * @param sink:	The end cell trying to be reached
 * @param stack: Holds the path of cells traveled
 * @param wall: A pointer to an array of bools which indicate if a wall exists at that location
 * @param iswhite: A pointer to an array of bools that indicate if a cell has been visited
 * @stack_sz: The size of the stack
 * @return: True if a path is found, otherwise False */
bool cell::solve(int Nrow, int Ncol, cell &current, cell &sink, cell *stack, bool *wall, bool *iswhite, int &stack_sz){
	/* Pointer Arithmetic:
	 *	2D:  array[i][j]	== array[(i*Ncol) + j]
	 *	3D:  array[i][j][z] == array[(i*Ncol*4) + (j*4) + z]
	*/
	cell next;

	/* Set current cell as visited (non-white) and add it to the stack */
	iswhite[(current.y * Ncol) + current.x] = false;
	stack[stack_sz] = current;
	++stack_sz;

	if (current.x == sink.x && current.y == sink.y){
		return true;
	}

	for (int dir = 0; dir < 4; dir++){
		if (wall[(current.y*Ncol*4) + (current.x*4) + dir] == true) {
			continue;	//wall is in the way
		}

		switch(dir) {
			case 0: //LEFT
				next.x = current.x - 1;
				next.y = current.y;
				break;
			
			case 1: //UP
				next.x = current.x;
				next.y = current.y - 1;
				break;

			case 2: //RIGHT
				next.x = current.x + 1;
				next.y = current.y;
				break;

			case 3: //DOWN
				next.x = current.x;
				next.y = current.y + 1;
				break;
		}
		
		if (iswhite[(next.y * Ncol) + next.x] == false) {
			continue;  //cell has already been visited
		}

		if (solve(Nrow, Ncol, next, sink, stack, wall, iswhite, stack_sz)){
			return true; 
		}
	}
	
	/* Path not found, backtrack */
	iswhite[(current.y * Ncol) + current.x] = true;
	--stack_sz;

	return false;
}

int main(int argc, char *argv[]) {
	
	FILE * inFile, * outFile;
	int Nrow, Ncol;
	bool * wall;
	
	if (argc != 3) {
		fprintf(stderr, "usage: ./Mazesolve maze.txt path.txt\n");
		return -1;
	} 
	
	inFile = fopen(argv[1], "r");
	if (inFile == NULL){
		fprintf(stderr, "Could not open file to read from.\n");
		return -1;
	}

	outFile = fopen(argv[2], "w");
	if (outFile == NULL){
		fprintf(stderr, "Could not open file to write to.\n");
		return -1;
	}

	fscanf(inFile, "MAZE %d %d", &Nrow, &Ncol);

	/* Create an array that stores bools to indicate if a wall exists to a 
	 * certain direction of a certain cell. Initialize the array to all false */
	wall = new bool [Nrow*Ncol*4];		// wall[][][]
	bool * wall_copy = wall;

	for (int i = 0; i < Nrow; i++){
		for (int j = 0; j < Ncol; j++){
			for (int z = 0; z < 4; z++){
				wall[(i*Ncol*4) + (j*4) + z] = false;
			}
		}
	}

	/* Set the maze boundary walls */
	for (int i = 0; i < Nrow; i++){
		for (int j = 0; j < Ncol; j++){
			if (i == 0){
				wall[(i*Ncol*4) + (j*4) + UP] = true;		//top row
			}
			else if (i == Nrow-1){
				wall[(i*Ncol*4) + (j*4) + DOWN] = true;		//bottom row
			}

			if (j == 0){
				wall[(i*Ncol*4) + (j*4) + LEFT] = true;		//leftmost column
			}
			else if (j == Ncol-1){
				wall[(i*Ncol*4) + (j*4) + RIGHT] = true;	//rightmost column
			}
		}
	}

	int y1, x1, y2, x2;

	/* Read from a file that holds the locations of interior walls and set
	 * these walls in the wall array */
	while(1) {
		if (feof(inFile)) {
			break;
		}
		fscanf(inFile, "%d %d %d %d", &y1, &x1, &y2, &x2);	//extract 2 cells from file
		
		wall[(y1*Ncol*4) + (x1*4) + get_direction(x1, y1, x2, y2)] = true;		//cell#1 has a wall to the ___
		wall[(y2*Ncol*4) + (x2*4) + get_direction(x2, y2, x1, y1)] = true;		//cell#2 has a wall to the ___
	}
	
	/* Initalize DFS path computation */
	cell source(0,0);
	cell sink(Ncol-1, Nrow-1);

	cell *stack = new cell[Nrow*Ncol];
	cell *stack_copy = stack;

	int stack_sz = 0;	//current position in stack

	bool *iswhite = new bool [Nrow*Ncol];
	bool *iswhite_copy = iswhite;

	/* Set all cells as white (not visited) */
	for (int i = 0; i < Nrow; i++){
		for (int j = 0; j < Ncol; j++){
			iswhite[(i*Ncol) + j] = true;
		}
	}
	
	/* Start DFS source-to-sink computation */
	bool result = source.solve(Nrow, Ncol, source, sink, stack, wall, iswhite, stack_sz);
	if (result == false) {
		fprintf(stderr, "Path could not be found\n");
		return -1;
	}

	/* Write path to file */	
	fprintf(outFile, "PATH %d %d\n", Nrow, Ncol);
	for (int i = 0; i < stack_sz; i++){
		fprintf(outFile, "%d %d\n", stack[i].get_y(), stack[i].get_x());
	}
	
	delete[] stack_copy;
	delete[] wall_copy;
	delete[] iswhite_copy;

	fclose(inFile);
	fclose(outFile);

	return 0;
}
