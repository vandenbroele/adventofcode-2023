#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define DIRECTIONS 4

enum Direction {
	North = 0,
	East = 1,
	South = 2,
	West = 3
};

char *read_line(){
	int c, line_length, line_size;
	char *line;

	line_length = 0;
	line_size = 2;
	line = calloc(sizeof(char), line_size);
	while((c = getchar()) != '\n' && c != EOF){
		if(line_length == line_size){
			line_size *= 2;
			line = realloc(line, sizeof(char) * line_size);
		}
		line[line_length++] = c;
	}

	if(line_length == line_size){
		line = realloc(line, sizeof(char) * (line_size + 1));
		line[line_length] = '\0';
	}

	return line;
}

char **read_contraption(int *rows, int *cols){
	char **contraption, *line;
	int length, size;

	length = 0;
	size = 2;
	contraption = calloc(sizeof(char*), size);
	while(0 < strlen(line = read_line())){
		if(length == size){
			size *= 2;
			contraption = realloc(contraption, sizeof(char*) * size);
		}
		contraption[length++] = line;
	}
	free(line);

	*rows = length;
	*cols = strlen(contraption[0]);
		
	return contraption;
}

void mark_energized_tiles(char **contraption, int rows, int cols, int row, int col, enum Direction direction, bool ***visited);

void mark_next_tile(char **contraption, int rows, int cols, int row, int col, enum Direction direction, bool ***visited){
	switch(direction){
		case North:
			mark_energized_tiles(contraption, rows, cols, row - 1, col, direction, visited);
			break;
		case East:
			mark_energized_tiles(contraption, rows, cols, row, col + 1, direction, visited);
			break;
		case South:
			mark_energized_tiles(contraption, rows, cols, row + 1, col, direction, visited);
			break;
		case West:
			mark_energized_tiles(contraption, rows, cols, row, col - 1, direction, visited);
			break;
	}
}

enum Direction reflect_direction(enum Direction direction, char mirror){
	if((direction == East && mirror == '/') || (direction == West && mirror == '\\')){
		return North;
	} else if((direction == West && mirror == '/') || (direction == East && mirror == '\\')){
		return South;
	} else if((direction == North && mirror == '/') || (direction == South && mirror == '\\')){
		return East;
	} else {
		return West;
	}
}

void mark_energized_tiles(char **contraption, int rows, int cols, int row, int col, enum Direction direction, bool ***visited){
	// stop when the beam is out of bounds
	if(row < 0 || rows <= row || col < 0 || cols <= col){
		return;
	}

	// stop when we have already visited this tile in this direction
	if(visited[row][col][direction]){
		return;
	}

	visited[row][col][direction] = true;
	
	// empty space
	if(contraption[row][col] == '.'){
		mark_next_tile(contraption, rows, cols, row, col, direction, visited);
	}

	// mirror
	else if(contraption[row][col] == '/' || contraption[row][col] == '\\'){
		enum Direction new_direction = reflect_direction(direction, contraption[row][col]);
		mark_next_tile(contraption, rows, cols, row, col, new_direction, visited);
	}
	
	// vertical splitter
	else if(contraption[row][col] == '|'){
		if(direction == North || direction == South){
			mark_next_tile(contraption, rows, cols, row, col, direction, visited);
		} else {
			mark_next_tile(contraption, rows, cols, row, col, North, visited);
			mark_next_tile(contraption, rows, cols, row, col, South, visited);
		}
	}

	// horizontal splitter
	else if(contraption[row][col] == '-'){
		if(direction == East || direction == West){
			mark_next_tile(contraption, rows, cols, row, col, direction, visited);
		} else {
			mark_next_tile(contraption, rows, cols, row, col, East, visited);
			mark_next_tile(contraption, rows, cols, row, col, West, visited); 
		}
	}
}

void solve(){
	int rows, cols, i, j, k, energized_count;
	char **contraption = read_contraption(&rows, &cols); 

	// init all situations as unvisited
	bool ***visited = calloc(sizeof(bool**), rows);
	for(i = 0; i < rows; i++){
		visited[i] = calloc(sizeof(bool*), cols);
		for(j = 0; j < cols; j++){
			visited[i][j] = calloc(sizeof(bool), DIRECTIONS);
			for(k = 0; k < DIRECTIONS; k++){
				visited[i][j][k] = false;
			}
		}
	}

	mark_energized_tiles(contraption, rows, cols, 0, 0, East, visited);

	energized_count = 0;
	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			bool energized = false;
			for(k = 0; !energized && k < DIRECTIONS; k++){
				energized = visited[i][j][k];
			}

			if(energized){
				energized_count++;
			}
		}
	}

	printf("%d\n", energized_count);

	// free memory
	for(i = 0; i < rows; i++){
		free(contraption[i]);
		for(j = 0; j < cols; j++){
			free(visited[i][j]);
		}
		free(visited[i]);
	}
	free(contraption);
	free(visited);
}

int main(){
	solve();

	return 0;
}
