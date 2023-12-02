#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LEN 256
#define COLOURS 3
#define bool unsigned char
#define true 1
#define false 0

char *colour_names[]  = { "red", "green", "blue" };
int max_colour_amount[] = { 12, 13, 14 };

char **split(char *s, char delimiter, int *result_count){
	int i, strings;

	// count number of substrings in result
	strings = 1;
	for(i = 1; i < strlen(s); i++){
		if(s[i] == delimiter){
			strings++;
		}
	}

	// allocate memory
	char **result = calloc(sizeof(char *), strings);

	// fill substrings
	int j, substring_start;
       
	strings = 0;
	substring_start = 0;
	for(i = 1; i <= strlen(s); i++){
		if(i == strlen(s) || s[i] == delimiter){
			result[strings] = calloc(sizeof(char), i - substring_start + 1);
			for(j = substring_start; j < i; j++){
				result[strings][j - substring_start] = s[j];
			}
			result[strings][j - substring_start] = 0;
			strings++;
			substring_start = i + 1;
		}
	}

	*result_count = strings; 

	return result;
}

bool starts_with(char *haystack, char *needle){
	int i;

	for(i = 0; i < strlen(needle); i++){
		if(strlen(haystack) <= i || haystack[i] != needle[i]){
			return false;
		}
	}

	return true;
}

int max(int a, int b) { return(a < b ? b : a); }

int get_power_of_minimum_cube_count(char* line){
	int i, substring_count;
	char **substrings;

	// split the line on spaces
	substrings = split(line, ' ', &substring_count);

	// get game number
	int game_number = 0;
		
	for(i = 0; i < strlen(substrings[1]) && substrings[1][i] != ':'; i++){
		game_number = (game_number * 10) + (substrings[1][i] - '0');
	}
	
	// find the max number of cubes shown in one hand for each colour
	int j, cubes, colour_amount[COLOURS] = { 0 };

	for(i = 2; i < substring_count; i += 2){
		cubes = atoi(substrings[i]);
			
		for(j = 0; j < COLOURS; j++){
			if(starts_with(substrings[i + 1], colour_names[j])) {
				colour_amount[j] = max(colour_amount[j], cubes);
				break;
			}
		}

		free(substrings[i]);
		free(substrings[i + 1]);
	}
	free(substrings[0]);
	free(substrings[1]);
	free(substrings);

	return colour_amount[0] * colour_amount[1] * colour_amount[2];
}

void solve(){
	char line[MAX_LINE_LEN];
	int power_sum = 0;

	while(fgets(line, MAX_LINE_LEN, stdin) != NULL){
		power_sum += get_power_of_minimum_cube_count(line);
	}

	printf("%d\n", power_sum);
}

int main(){
	solve();

	return 0;
}
