#include <stdio.h>
#include <string.h>
#define MAX_ROWS 256
#define MAX_COLS 256
#define bool unsigned char
#define true 1
#define false 0

bool is_digit(char c){ return('0' <= c && c <= '9'); }

bool is_part_number(char engine[MAX_ROWS][MAX_COLS], int rows, int cols, int number_row, int number_col_start, int number_col_end){
	int i, j;

	for(i = number_row - 1; i <= number_row + 1; i++){
		for(j = number_col_start - 1; j <= number_col_end + 1; j++){
			if(0 <= i && i < rows && 0 <= j && j < cols && engine[i][j] != '.' && !is_digit(engine[i][j])){
				return true;
			}
		}	
	}

	return false;
}

void solve(char engine[MAX_ROWS][MAX_COLS], int rows, int cols){
	int i, j, part_number_sum, part_number, number_start, number_end;

	part_number_sum = 0;
	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			if(is_digit(engine[i][j])){
				number_start = j;
				part_number = 0;

				for(number_end = number_start; number_end < cols && is_digit(engine[i][number_end]); number_end++){
					part_number = (part_number * 10) + (engine[i][number_end] - '0');
				}

				if(is_part_number(engine, rows, cols, i, number_start, number_end - 1)){
					part_number_sum += part_number;
				}

				j = number_end;
			}
		}
	}

	printf("%d\n", part_number_sum);
}

int main(){
	char engine[MAX_ROWS][MAX_COLS];
	int rows;

	for(rows = 0; scanf("%s", engine[rows]) != EOF; rows++);
	solve(engine, rows, strlen(engine[0]));
	
	return 0;
}
