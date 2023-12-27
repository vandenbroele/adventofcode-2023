#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define DIRECTIONS 4
int row_directions[] = { 0, 0, 1, -1 };
int col_directions[] = { 1, -1, 0, 0 };

char *read_line(){
	int c, size, capacity;
	char *line;

	size = 0;
	capacity = 2;
	line = calloc(sizeof(char), capacity);
	while((c = getchar()) != EOF && c != '\n'){
		if(size == capacity){
			line = realloc(line, sizeof(char) * (capacity *= 2));
		}
		line[size++] = c;
	}

	if(size == capacity){
		line = realloc(line, sizeof(char) * (++capacity));
	}
	line[size] = '\0';

	return line;
}

char **read_grid(int *rows, int *cols){
	int size, capacity;
	char *line, **grid;

	size = 0;
	capacity = 2;
	grid = calloc(sizeof(char*), capacity);
	while(0 < strlen(line = read_line())){
		if(size == capacity){
			grid = realloc(grid, sizeof(char*) * (capacity *= 2));
		}
		grid[size++] = line;
	}
	free(line);

	*rows = size;
	*cols = strlen(grid[0]);
	return grid;
}

bool **create_visited_array(int rows, int cols){
	bool **visited = calloc(sizeof(bool*), rows);
	int i, j;

	for(i = 0; i < rows; i++){
		visited[i] = calloc(sizeof(bool), cols);
		for(j = 0; j < cols; j++){
			visited[i][j] = false;
		}
	}

	return visited;
}

void free_2d_arr(void **arr, int rows){
	int i;

	for(i = 0; i < rows; i++){
		free(arr[i]);
	}
	free(arr);
}

int find_starting_column(char **grid, int cols){
	int i;

	for(i = 0; i < cols; i++){
		if(grid[0][i] != '#'){
			return i;
		}
	}

	return -1;
}

int max(int a, int b){ return(a < b ? b : a); }

bool is_valid_path_tile(char **grid, int rows, int cols, int row, int col, bool **visited){
	return 0 <= row && row < rows && 0 <= col && col < cols && !visited[row][col] && grid[row][col] != '#';
}

int dfs_longest_path(char **grid, int rows, int cols, int row, int col, bool **visited, int path_length){
	if(row == rows - 1){
		printf("Found a path of length %d\n", path_length);
		return path_length;
	}

	visited[row][col] = true;

	int i, neighbour_row, neighbour_col, longest_path;
	
	longest_path = -1;
	for(i = 0; i < DIRECTIONS; i++){
		neighbour_row = row + row_directions[i];
		neighbour_col = col + col_directions[i];

		if(is_valid_path_tile(grid, rows, cols, neighbour_row, neighbour_col, visited)){
			longest_path = max(longest_path, dfs_longest_path(grid, rows, cols, neighbour_row, neighbour_col, visited, path_length + 1));
		}
	}

	visited[row][col] = false;

	return longest_path;
}

void solve(){
	int rows, cols, starting_col;
	char **grid = read_grid(&rows, &cols);
	bool **visited = create_visited_array(rows, cols);

	printf("Read a grid with %d rows and %d cols\n", rows, cols);

	starting_col = find_starting_column(grid, cols);
	printf("Starting dfs from position %d, %d\n", 0, starting_col);

	printf("The longest path is %d steps\n", dfs_longest_path(grid, rows, cols, 0, starting_col, visited, 0));

	free_2d_arr((void **) visited, rows);
	free_2d_arr((void **) grid, rows);
}

int main(){
	solve();
	return 0;
}
