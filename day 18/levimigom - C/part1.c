#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define DIRECTIONS 4

int row_increment[] = { 1, -1, 0, 0 };
int col_increment[] = { 0, 0, 1, -1 };

struct instruction {
	char direction;
	int meters;
};

struct instruction **read_instructions(int *instruction_count){
	struct instruction **instructions;
	int meters, size, capacity;
	char direction;

	size = 0;
	capacity = 1;
	instructions = calloc(sizeof(struct instruction*), capacity);
	while(scanf(" %c %d %*s", &direction, &meters) != EOF){
		if(size == capacity){
			capacity *= 2;
			instructions = realloc(instructions, sizeof(struct instruction*) * capacity);
		}
		instructions[size] = malloc(sizeof(struct instruction));
		instructions[size]->direction = direction;
		instructions[size++]->meters = meters;
	}

	*instruction_count = size;
	return instructions;
}

void follow_instruction(char direction, int meters, int *x, int *y){
	switch(direction){
		case 'U':
			*x -= meters;
			break;
		case 'D':
			*x += meters;
			break;
		case 'L':
			*y -= meters;
			break;
		case 'R':
			*y += meters;
			break;
	}
}

void calculate_dimensions(struct instruction **instructions, int instruction_count, int *min_x, int *min_y, int *max_x, int *max_y){
	int i, x, y;

	*min_x = *min_y = *max_x = *max_y = x = y = 0;
	for(i = 0; i < instruction_count; i++){
		follow_instruction(instructions[i]->direction, instructions[i]->meters, &x, &y);

		if(x < *min_x){
			*min_x = x;
		} else if(*max_x < x){
			*max_x = x;
		} else if(y < *min_y){
			*min_y = y;
		} else if(*max_y < y){
			*max_y = y;
		}
	}	
}

bool **draw_trench(struct instruction **instructions, int instruction_count, int min_x, int min_y, int rows, int cols){
	int i;
	bool **plan = calloc(sizeof(bool*), rows);
	
	for(i = 0; i < rows; i++){
		plan[i] = calloc(sizeof(bool), cols);
	}
	
	int j, x, y;

	x = y = 0;
	for(i = 0; i < instruction_count; i++){
		plan[x - min_x + 1][y - min_y + 1] = 1;
		for(j = 0; j < instructions[i]->meters; j++){
			follow_instruction(instructions[i]->direction, 1, &x, &y);
			plan[x - min_x + 1][y - min_y + 1] = 1;
		}
	}
	
	return plan;
}

int dfs_calculate_empty_area_size(bool **plan, int rows, int cols, int row, int col, bool **visited){
	// stop when out of bounds, already visited or part of trench
	if(row < 0 || rows <= row || col < 0 || cols <= col || visited[row][col] || plan[row][col]){
		return 0;
	}
	
	visited[row][col] = true;

	int i, area;

	area = 1;
	for(i = 0; i < 4; i++){
		area += dfs_calculate_empty_area_size(plan, rows, cols, row + row_increment[i], col + col_increment[i], visited);	
	}
	
	return area;
}

void solve(){
	// read instructions
	struct instruction **instructions;
	int instruction_count;

	instructions = read_instructions(&instruction_count);

	// calculate dimensions
	int min_x, min_y, max_x, max_y;

	calculate_dimensions(instructions, instruction_count, &min_x, &min_y, &max_x, &max_y);

	// draw the plan and add an empty border
	int i, j, rows, cols;
	bool **plan;

	rows = max_x - min_x + 3;
	cols = max_y - min_y + 3;
	plan = draw_trench(instructions, instruction_count, min_x, min_y, rows, cols);	

	// find the size of the empty area
	bool **visited = calloc(sizeof(bool*), rows);
	for(i = 0; i < rows; i++){
		visited[i] = calloc(sizeof(bool), cols);
	}

	printf("%d\n", rows * cols - dfs_calculate_empty_area_size(plan, rows, cols, 0, 0, visited));
	
	// free memory
	for(i = 0; i < rows; i++){
		free(plan[i]);
		free(visited[i]);
	}
	free(plan);
	free(visited);

	for(i = 0; i < instruction_count; i++){
		free(instructions[i]);
	}

	free(instructions);
}

int main(){
	solve();
	return 0;
}
