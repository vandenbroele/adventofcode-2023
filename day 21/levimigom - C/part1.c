#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define STEPS 64
#define START_PLOT 'S'
#define GARDEN_PLOT '.'
#define ROCK_PLOT '#'
#define DIRECTIONS 4

int row_directions[] = { 0, 0, 1, -1 };
int col_directions[] = { 1, -1, 0, 0 };

char *read_line(){
	int c, size, capacity;
	char *line;

	size = 0;
	capacity = 2;
	line = calloc(sizeof(char), capacity);
	while((c = getchar()) != '\n' && c != EOF){
		if(size == capacity){
			line = realloc(line, sizeof(char) * (capacity *= 2));
		}
		line[size++] = c;
	}

	if(size == capacity){
		line = realloc(line, sizeof(char) * ++capacity);
	}
	line[size++] = '\0';

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

void find_starting_position(char **grid, int rows, int cols, int *starting_row, int *starting_col){
	int i, j;

	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			if(grid[i][j] == START_PLOT){
				*starting_row = i;
				*starting_col = j;
				return;
			}
		}
	}
}

bool ***create_visited_array(int rows, int cols, int steps){
	int i, j, k;
	bool ***visited = calloc(sizeof(bool**), rows);

	for(i = 0; i < rows; i++){
		visited[i] = calloc(sizeof(bool*), cols);
		for(j = 0; j < cols; j++){
			visited[i][j] = calloc(sizeof(bool), steps + 1);
			for(k = 0; k < steps; k++){
				visited[i][j][k] = false;
			}
		}
	}

	return visited;
}

void destroy_visited_array(bool ***visited, int rows, int cols){
	int i, j;

	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			free(visited[i][j]);
		}
		free(visited[i]);
	}
	free(visited);
}

bool is_reachable_plot(char **grid, int rows, int cols, bool ***visited, int steps, int row, int col){
	return(0 <= row && row < rows && 0 <= col && col < cols && !visited[row][col][steps] && (grid[row][col] == GARDEN_PLOT || grid[row][col] == START_PLOT));
}

int count_reachable_garden_plots(char **grid, int rows, int cols, bool ***visited, int row, int col, int steps){
	visited[row][col][steps] = true;
	
	if(steps == 0){
		return 1;
	}

	int i, neighbour_row, neighbour_col, reachable_plots;

	reachable_plots = 0;	
	for(i = 0; i < DIRECTIONS; i++){
		neighbour_row = row + row_directions[i];
		neighbour_col = col + col_directions[i];

		if(is_reachable_plot(grid, rows, cols, visited, steps - 1, neighbour_row, neighbour_col)){
			reachable_plots += count_reachable_garden_plots(grid, rows, cols, visited, neighbour_row, neighbour_col, steps - 1);
		}
	}

	return reachable_plots;
}

void print_grid(char **grid, int rows, int cols){
	int i, j;

	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			printf("%c", grid[i][j]);
		}
		printf("\n");
	}
}

void destroy_grid(char **grid, int rows){
	int i;

	for(i = 0; i < rows; i++){
		free(grid[i]);
	}
	free(grid);
}

void solve(){
	int rows, cols, starting_row, starting_col;
	char **grid = read_grid(&rows, &cols);
	int steps = STEPS;

	find_starting_position(grid, rows, cols, &starting_row, &starting_col);
	bool ***visited = create_visited_array(rows, cols, steps);

	printf("%d\n", count_reachable_garden_plots(grid, rows, cols, visited, starting_row, starting_col, steps));

	int i, j;
	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			if(visited[i][j][0]){
				printf("O");
			} else {
				printf("%c", grid[i][j]);
			}
		}
		printf("\n");
	}	

	destroy_visited_array(visited, rows, cols);
	destroy_grid(grid, rows);
}

int main(){
	solve();
	return 0;
}
