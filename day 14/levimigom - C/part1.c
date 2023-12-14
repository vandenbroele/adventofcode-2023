#include <stdio.h>
#include <stdlib.h>

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

void solve(){
	int i, j, rows, cols;
	char **platform = read_platform(&rows, &cols);

	slide_north(platform, rows, cols);
	printf("Total load = %d\n", calculate_total_load(platform, rows, cols));

	// free memory
	for(i = 0; i < rows; i++){
		free(platform[i]);
	}
	free(platform);
}

int main(){
	solve();
	return 0;
}
