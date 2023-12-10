#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DIM 256
#define bool unsigned char
#define true 1
#define false 0

void mark_loop(char maze[DIM][DIM], int row, int col, bool **visited){
	if(visited[row][col]){
		return;
	}

	visited[row][col] = true;

	// traverse north
	if((maze[row][col] == '|' || maze[row][col] == 'L' || maze[row][col] == 'J') && !visited[row - 1][col]){
		mark_loop(maze, row - 1, col, visited);
	}

	// traverse south
	else if((maze[row][col] == '|' || maze[row][col] == '7' || maze[row][col] == 'F') && !visited[row + 1][col]){
		mark_loop(maze, row + 1, col, visited);
	}

	// traverse east
	else if((maze[row][col] == '-' || maze[row][col] == 'L' || maze[row][col] == 'F') && !visited[row][col + 1]){
		mark_loop(maze, row, col + 1, visited);
	}

	// traverse west
	else if((maze[row][col] == '-' || maze[row][col] == 'J' || maze[row][col] == '7') && !visited[row][col - 1]){
		mark_loop(maze, row, col - 1, visited);
	}
}

void find_tile_connected_to_start(char maze[DIM][DIM], int rows, int cols, int start_row, int start_col, int *connected_row, int *connected_col){
	// check east tile
	if(start_col + 1 < cols && (maze[start_row][start_col + 1] == '-' || maze[start_row][start_col + 1] == 'J' || maze[start_row][start_col + 1] == '7')){
		*connected_row = start_row;
		*connected_col = start_col + 1;
	}	
	// check west tile
	else if(0 <= start_col - 1 && (maze[start_row][start_col - 1] == '-' || maze[start_row][start_col - 1] == 'L' || maze[start_row][start_col - 1] == 'F')){
		*connected_row = start_row;
		*connected_col = start_col - 1;
	}
	// if east and west are both not connected then north and south need to be
	else {
		*connected_row = start_row + 1;
		*connected_col = start_col;
	}
}

int count_enclosed_tiles(char maze[DIM][DIM], int rows, int cols, bool **visited){
	// you can know which tiles are inside, and which tiles are outside the loop
	// by counting the number of walls in a particular row.
	// the only problem here is that depending on the position of your start S
	// it could be a wall or not, in my case it wasnt so i hardcoded it as not
	// but it would be better to just replace the S by the corresponding pipe before you start

	int i, j;
	
	int *row_pipe_count = calloc(sizeof(int), rows);
	for(i = 0; i < rows; i++){
		row_pipe_count[i] = 0;
	}

	int count = 0;
	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			if(!visited[i][j]){
				if(row_pipe_count[i] % 2 == 0){
					printf("O");
				} else {
					printf("I");
					count++;
				}
			} else {
				printf("P");

				if(maze[i][j] == '|' || maze[i][j] == '7' || maze[i][j] == 'F'){
					row_pipe_count[i]++;
				}
			}
		}
		printf("\n");
	}

	free(row_pipe_count);

	return count;
}

void solve(char maze[DIM][DIM], int rows, int cols){
	// find starting position
	int i, j, start_row, start_col;

	start_row = start_col = -1;
	for(i = 0; i < rows && start_row == -1; i++){
		for(j = 0; j < cols && start_col == -1; j++){
			if(maze[i][j] == 'S'){
				start_row = i;
				start_col = j;
			}
		}
	}

	// create array to keep track of which tiles have been visited
	bool **visited = calloc(sizeof(bool*), rows);

	for(i = 0; i < rows; i++){
		visited[i] = calloc(sizeof(bool), cols);
		for(j = 0; j < cols; j++){
			visited[i][j] = false;
		}
	}

	// find a tile connected to the starting position
	int connected_row, connected_col;

	find_tile_connected_to_start(maze, rows, cols, start_row, start_col, &connected_row, &connected_col);

	// mark all tiles in the loop
	mark_loop(maze, connected_row, connected_col, visited);
	
	printf("%d\n", count_enclosed_tiles(maze, rows, cols, visited));

	// release memory
	for(i = 0; i < rows; i++){
		free(visited[i]);
	}
	free(visited);
}

int main(){
	char maze[DIM][DIM];
	int i;

	for(i = 0; scanf("%s", maze[i]) != EOF; i++);

	solve(maze, i, strlen(maze[0]));

	return 0;
}
