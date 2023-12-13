#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char** read_pattern(int *rows, int *cols){
	int c, pattern_size, pattern_length, row_size, row_length;
        char *row, **pattern;

	pattern_length = 0;
	pattern_size = 2;
	pattern = calloc(sizeof(char*), pattern_size);
	while((c = getchar()) != '\n' && c != EOF){
		if(pattern_length == pattern_size){
			pattern_size *= 2;
			pattern = realloc(pattern, sizeof(char*) * pattern_size);
		}	

		row_size = 2;
		row_length = 1;
		row = calloc(sizeof(char), row_size);
		row[0] = c;
		while((c = getchar()) != '\n'){
			if(row_size == row_length){
				row_size *= 2;
				row = realloc(row, sizeof(char) * row_size);
			}
			row[row_length++] = c;
		}
		pattern[pattern_length++] = row;
		*cols = row_length;
	}

	*rows = pattern_length;
	return pattern;
}

int min(int a, int b){ return(a < b ? a : b); }

int count_reflections(char **pattern, int rows, int cols){
	int i, j, k;
	bool reflects;

	for(i = 0; i < cols - 1; i++){
		reflects = true;
		for(j = 0; j < rows && reflects; j++){
			for(k = 0; k <= i && i + k + 1 < cols; k++){
				if(pattern[j][i - k] != pattern[j][i + k + 1]){
					reflects = false;
					break;
				}
			}
		}

		if(reflects){
			return i + 1;
		}
	}

	return 0;
}

char **rotate(char **pattern, int *rows, int *cols){
	char** rotated = calloc(sizeof(char*), *cols);
	int i, j;

	for(i = 0; i < *cols; i++){
		rotated[i] = calloc(sizeof(char), *rows);
	}

	for(i = 0; i < *cols; i++){
		for(j = 0; j < *rows; j++){
			rotated[i][j] = pattern[j][i];
		}
	}

	for(i = 0; i < *rows; i++){
		free(pattern[i]);
	}
	free(pattern);

	int placeholder = *rows;
	*rows = *cols;
	*cols = placeholder;

	return rotated;
}

void solve(){
	int i, j, rows, cols, total_reflections;
	char **pattern;

	total_reflections = 0;
	pattern = read_pattern(&rows, &cols);
	while(0 < rows){
		// find vertical reflection
		total_reflections += count_reflections(pattern, rows, cols);
		// find horizontal reflection
		pattern = rotate(pattern, &rows, &cols);
		total_reflections += 100 * count_reflections(pattern, rows, cols);

		// free memory
		for(i = 0; i < rows; i++){
			free(pattern[i]);
		}
		free(pattern);
		
		pattern = read_pattern(&rows, &cols);
	}
	free(pattern);

	printf("Total: %d\n", total_reflections);
}

int main(){
	solve();

	return 0;
}
