#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DIM 256
#define bool unsigned char
#define true 1
#define false 0

int max(int a, int b) { return(a < b ? b : a); }

int dfs(char maze[DIM][DIM], int row, int col, bool **visited){
	if(visited[row][col]){
		return 0;
	}

	visited[row][col] = true;

	// | => traverse north and south
	if(maze[row][col] == '|'){
		return 1 + max(dfs(maze, row - 1, col, visited), dfs(maze, row + 1, col, visited));
	}

	// - => traverse east and west
	if(maze[row][col] == '-'){
		return 1 + max(dfs(maze, row, col - 1, visited), dfs(maze, row, col + 1, visited));
	}

	// L => traverse north and east
	if(maze[row][col] == 'L'){
		return 1 + max(dfs(maze, row - 1, col, visited), dfs(maze, row, col + 1, visited));
	}

	// J => traverse north and west
	if(maze[row][col] == 'J'){
		return 1 + max(dfs(maze, row - 1, col, visited), dfs(maze, row, col - 1, visited));
	}

	// 7 => traverse south and west
	if(maze[row][col] == '7'){
		return 1 + max(dfs(maze, row + 1, col, visited), dfs(maze, row, col - 1, visited));
	}

	// F => traverse south and east
	if(maze[row][col] == 'F'){
		return 1 + max(dfs(maze, row + 1, col, visited), dfs(maze, row, col + 1, visited));
	}

	// . => stop
	return 0;
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

	// find length of the loop using dfs
	int loop_length = dfs(maze, connected_row, connected_col, visited);

	printf("Distance to fartherst tile: %d\n", loop_length / 2 + 1);

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
