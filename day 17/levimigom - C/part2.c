#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "priority_queue.h"
#define DIRECTIONS 4
#define STEPS 10

int direction_row_increment[] = { -1, 0, 1, 0 };
int direction_col_increment[] = { 0, 1, 0, -1 };

enum Direction {
	North = 0,
	East = 1,
	South = 2,
	West = 3
};

struct position {
	int row, col, steps, heat_loss;
	enum Direction previous_direction;
};

struct position *create_position(int row, int col, int heat_loss, enum Direction previous_direction, int steps){
	struct position *position = malloc(sizeof(struct position));
	position->row = row;
	position->col = col;
	position->heat_loss = heat_loss;
	position->previous_direction = previous_direction;
	position->steps = steps;
	return position;
}

char *read_line(){
	int c, size, length;
	char *line;

	size = 2;
	length = 0;
	line = calloc(sizeof(char), size);
	while((c = getchar()) != '\n' && c != EOF){
		if(size == length){
			line = realloc(line, sizeof(char) * (size *= 2));
		}
		line[length++] = c;
	}

	if(length == size){
		line = realloc(line, sizeof(char) * (size + 1));
	}
	line[length] = '\0';

	return line;
}

char **read_input(int *rows, int *cols){
	int size, length;
	char **input, *line;

	size = 2;
	length = 0;
	input = calloc(sizeof(char*), size);
	while(strlen(line = read_line())){
		if(size == length){
			input = realloc(input, sizeof(char*) * (size *= 2));
		}
		input[length++] = line;
	}
	free(line);

	*rows = length;
	*cols = strlen(input[0]);

	return input;
}

bool is_within_bounds(int row, int col, int rows, int cols){
	return(0 <= row && row < rows && 0 <= col && col < cols);
}

int find_optimal_path(char **crucible, int rows, int cols){
	struct priority_queue *minimum_priority_queue = create_priority_queue();
	struct position *current_position, *new_position;
	enum Direction new_direction;

	int i, j, k, l, new_heat_loss, new_row, new_col, optimal_heat_loss;
	bool ****visited;

	// keep track of which tiles have been visited before
	visited = calloc(sizeof(bool***), rows);
	for(i = 0; i < rows; i++){
		visited[i] = calloc(sizeof(bool**), cols);
		for(j = 0; j < cols; j++){
			visited[i][j] = calloc(sizeof(bool*), DIRECTIONS);
			for(k = 0; k < DIRECTIONS; k++){
				visited[i][j][k] = calloc(sizeof(bool), STEPS + 1);
				for(l = 0; l <= STEPS; l++){
					visited[i][j][k][l] = false;
				}
			}
		}
	}

	// enqueue the starting position
	enqueue_priority_queue(minimum_priority_queue, create_position(0, 0, 0, North, 0), 0);
	enqueue_priority_queue(minimum_priority_queue, create_position(0, 0, 0, East, 0), 0);
	enqueue_priority_queue(minimum_priority_queue, create_position(0, 0, 0, South, 0), 0);
	enqueue_priority_queue(minimum_priority_queue, create_position(0, 0, 0, West, 0), 0);
	
	optimal_heat_loss = -1;
	while(0 < minimum_priority_queue->size && optimal_heat_loss == -1){
		current_position = dequeue_priority_queue(minimum_priority_queue);

		if(current_position->row == rows - 1 && current_position->col == cols - 1 && 4 <= current_position->steps){
			optimal_heat_loss = current_position->heat_loss;
		} 
		else if(!visited[current_position->row][current_position->col][current_position->previous_direction][current_position->steps]){
			// mark position as visited so we dont process it again
			visited[current_position->row][current_position->col][current_position->previous_direction][current_position->steps] = true;

			// continue in the same direction
			if(current_position->steps < 10){
				new_row = current_position->row + direction_row_increment[current_position->previous_direction];
				new_col = current_position->col + direction_col_increment[current_position->previous_direction];

				if(is_within_bounds(new_row, new_col, rows, cols) && !visited[new_row][new_col][current_position->previous_direction][current_position->steps + 1]){
					new_heat_loss = current_position->heat_loss + crucible[new_row][new_col] - '0';
					new_position = create_position(new_row, new_col, new_heat_loss, current_position->previous_direction, current_position->steps + 1);
					enqueue_priority_queue(minimum_priority_queue, new_position, new_position->heat_loss);
				}
			}

			// turn 90 degrees left or right
			if(4 <= current_position->steps){
				for(i = 1; i < DIRECTIONS; i += 2){
					new_direction = (current_position->previous_direction + i) % DIRECTIONS;
					new_row = current_position->row + direction_row_increment[new_direction];
					new_col = current_position->col + direction_col_increment[new_direction];
				
					if(is_within_bounds(new_row, new_col, rows, cols) && !visited[new_row][new_col][new_direction][1]){
						new_heat_loss = current_position->heat_loss + crucible[new_row][new_col] - '0';
						new_position = create_position(new_row, new_col, new_heat_loss, new_direction, 1);
						enqueue_priority_queue(minimum_priority_queue, new_position, new_heat_loss);
					}
				}
			}
		} 

		free(current_position);
	}

	// free memory
	while(0 < minimum_priority_queue->size){
		free(dequeue_priority_queue(minimum_priority_queue));
	}
	free_priority_queue(minimum_priority_queue);

	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			for(k = 0; k < DIRECTIONS; k++){
				free(visited[i][j][k]);
			}
			free(visited[i][j]);
		}
		free(visited[i]);
	}
	free(visited);

	return optimal_heat_loss;
}

void solve(){
	int i, j, rows, cols;
	char **crucible = read_input(&rows, &cols);

	printf("Read %d rows and %d cols\n", rows, cols);
	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			printf("%c", crucible[i][j]);
		}
		printf("\n");
	}

	printf("%d\n", find_optimal_path(crucible, rows, cols));

	// free memory
	for(i = 0; i < rows; i++){
		free(crucible[i]);
	}
	free(crucible);
}

int main(){
	solve();
	return 0;
}
