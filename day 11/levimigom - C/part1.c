#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int abs(int a){
	return(a < 0 ? -a : a);
}

char* read_row(){
	int c, row_length, row_size;
	char *row;

	// read chars until end of line or end of file
	row_length = 0;
	row_size = 1;
	row = calloc(sizeof(char), row_size);
	while((c = getchar()) != EOF && c != '\n'){
		if(row_length == row_size){
			row_size *= 2;
			row = realloc(row, sizeof(char) * row_size);
		}

		row[row_length++] = c;
	}

	// add string terminator
	if(row_length == row_size){
		row = realloc(row, sizeof(char) * (row_size + 1));
	}
	row[row_length] = '\0';

	return row;
}

char** read_image(int *rows){
	char **image, *row;
	int image_row_count, image_row_size;

	// allocate memory for image
	image_row_size = 1;
	image = calloc(sizeof(char*), image_row_size);

	// read rows until you read an empty string
	image_row_count = 0;
	row = read_row();
	while(0 < strlen(row)){
		// allocate more memory when needed
		if(image_row_size == image_row_count){
			image_row_size *= 2;
			image = realloc(image, sizeof(char*) * image_row_size);
		}

		image[image_row_count++] = row;
		row = read_row();
	}
	free(row);

	*rows = image_row_count;
	return image;
}

int** find_galaxies(char **image, int rows, int cols, int *galaxies_count_output){
	int i, j, **galaxies, galaxies_size, galaxies_count;
	
	galaxies_count = 0;
	galaxies_size = 1;
	galaxies = calloc(sizeof(int*), galaxies_size);

	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			if(image[i][j] == '#'){
				if(galaxies_size == galaxies_count){
					galaxies_size *= 2;
					galaxies = realloc(galaxies, sizeof(int*) * galaxies_size);
				}

				galaxies[galaxies_count] = calloc(sizeof(int), 2);
				galaxies[galaxies_count][0] = i;
				galaxies[galaxies_count][1] = j;
				galaxies_count++;
			}
		}
	}
	*galaxies_count_output = galaxies_count;
	
	return galaxies;
}

void solve(char **image, int rows, int cols){
	// count how many rows and cols should be expanded
	int *expanded_row_count = calloc(sizeof(int), rows);
	int *expanded_col_count = calloc(sizeof(int), cols);
	int i, j;

	for(i = 0; i < rows; i++){
		expanded_row_count[i] = 1 + (i == 0 ? 0 : expanded_row_count[i - 1]);

		for(j = 0; j < cols; j++){
			if(image[i][j] == '#'){
				expanded_row_count[i]--;
				break;
			}
		}
	}

	for(i = 0; i < cols; i++){
		expanded_col_count[i] = 1 + (i == 0 ? 0 : expanded_col_count[i - 1]);

		for(j = 0; j < rows; j++){
			if(image[j][i] == '#'){
				expanded_col_count[i]--;
				break;
			}
		}
	}

	// find all the galaxies in the image
	int galaxies_count;
	int **galaxies = find_galaxies(image, rows, cols, &galaxies_count);

	// calculate the total distance
	int total_distance = 0;

	for(i = 0; i < galaxies_count; i++){
		for(j = i + 1; j < galaxies_count; j++){
			// row distance
			total_distance += abs(galaxies[i][0] - galaxies[j][0]) + 
				abs(expanded_row_count[galaxies[i][0]] - expanded_row_count[galaxies[j][0]]);
			// col distance
			total_distance += abs(galaxies[i][1] - galaxies[j][1]) + 
				abs(expanded_col_count[galaxies[i][1]] - expanded_col_count[galaxies[j][1]]);
		}
	}

	printf("The total distance is %d\n", total_distance);

	// release memory
	for(i = 0; i < galaxies_count; i++){
		free(galaxies[i]);
	}
	free(galaxies);

	free(expanded_row_count);
	free(expanded_col_count);
}

int main(){
	int i, rows;
	char **image = read_image(&rows);

	// solve the problem
	solve(image, rows, strlen(image[0]));

	// release memory
	for(i = 0; i < rows; i++){
		free(image[i]);
	}
	free(image);

	return 0;
}
