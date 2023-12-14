#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define CYCLES 1000000000

char **read_platform(int *rows, int *cols){
	int c, platform_length, platform_size, row_length, row_size;
	char **platform;

	platform_length = row_length = 0;
	platform_size = row_size = 2;
	platform = calloc(sizeof(char*), platform_size);

	while((c = getchar()) != EOF){
		if(platform_length == platform_size){
			platform_size *= 2;
			platform = realloc(platform, sizeof(char*) * platform_size);
		}

		if(c == '\n'){
			platform_length++;
			*cols = row_length;
		       	row_length = 0;
		} else {
			if(row_length == 0){
				row_size = 2;
				platform[platform_length] = calloc(sizeof(char), row_size);
			} else if(row_length == row_size){
				row_size *= 2;
				platform[platform_length] = realloc(platform[platform_length], sizeof(char) * row_size);
			}

			platform[platform_length][row_length++] = c;
		}
	}

	*rows = platform_length;
	return platform;
}

void slide_north(char **platform, int rows, int cols){
	int i, j, k;

	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			if(platform[i][j] == 'O'){
				k = i - 1;
				
				while(0 <= k && platform[k][j] == '.'){
					platform[k + 1][j] = '.';
					platform[k--][j] = 'O';
				}	
			}
		}
	}
}

int calculate_total_load(char **platform, int rows, int cols){
	int i, j, total_load;

	total_load = 0;
	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			if(platform[i][j] == 'O'){
				total_load += rows - i;
			}
		}
	}

	return total_load;
}

char** rotate_right(char **platform, int *rows, int *cols){
	int i, j, placeholder;
	char **rotated = calloc(sizeof(char*), *cols);
	
	for(i = 0; i < *cols; i++){
		rotated[i] = calloc(sizeof(char), *rows);
	}

	for(i = 0; i < *rows; i++){
		for(j = 0; j < *cols; j++){
			rotated[j][i] = platform[*rows - i - 1][j];
		}
	}
	
	for(i = 0; i < *rows; i++){
		free(platform[i]);	
	}
	free(platform);

	placeholder = *rows;
	*rows = *cols;
	*cols = placeholder;

	return rotated;
}

char** cycle(char **platform, int *rows, int *cols){
	// slide north
	slide_north(platform, *rows, *cols);
	// slide west
	platform = rotate_right(platform, rows, cols);
	slide_north(platform, *rows, *cols);
	// slide south
	platform = rotate_right(platform, rows, cols);
	slide_north(platform, *rows, *cols);
	// slide east
	platform = rotate_right(platform, rows, cols);
	slide_north(platform, *rows, *cols);
	// return to normal orientation
	platform = rotate_right(platform, rows, cols);
	return platform;
}

char **copy_platform(char **platform, int rows, int cols){
	int i, j;
	char **copy = calloc(sizeof(char*), rows);

	for(i = 0; i < rows; i++){
		copy[i] = calloc(sizeof(char), cols);
		for(j = 0; j < cols; j++){
			copy[i][j] = platform[i][j];
		}
	}

	return copy;
}

bool platform_equals(char **platform_a, char **platform_b, int rows, int cols){
	int i, j;

	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			if(platform_a[i][j] != platform_b[i][j]){
				return false;
			}
		}
	}

	return true;
}

void free_platform(char **platform, int rows, int cols){
	int i, j;

	for(i = 0; i < rows; i++){
		free(platform[i]);
	}
	free(platform);
}

void solve(){
	int i, j, rows, cols;
	char **platform = read_platform(&rows, &cols);

	int platforms_length, pattern_start, pattern_length, platforms_size = 2;
	char ***platforms = calloc(sizeof(char**), platforms_size);
	bool found_repeating_pattern = false;

	platforms[0] = copy_platform(platform, rows, cols);
	for(platforms_length = 1; !found_repeating_pattern; platforms_length++){
		platform = cycle(platform, &rows, &cols);
		
		if(platforms_length == platforms_size){
			platforms_size *= 2;
			platforms = realloc(platforms, sizeof(char**) * platforms_size);
		}
		
		platforms[platforms_length] = copy_platform(platform, rows, cols);

		// try to find the pattern
		for(j = 0; j < platforms_length; j++){
			if(platform_equals(platforms[platforms_length], platforms[j], rows, cols)){
				pattern_start = j;
				pattern_length = platforms_length - j;
				printf("Found the pattern starting at the %dth cycle and repeating every %d cycles\n", pattern_start, pattern_length);
				found_repeating_pattern = true;
				break;
			}
		}
	}	

	printf("Total load after %d cycles = %d\n", CYCLES, calculate_total_load(platforms[pattern_start + ((CYCLES - pattern_start) % pattern_length)], rows, cols));

	// free memory
	free_platform(platform, rows, cols);

	for(j = 0; j < platforms_length; j++){
		free_platform(platforms[j], rows, cols);
	}
	free(platforms);
}

int main(){
	solve();
	return 0;
}
