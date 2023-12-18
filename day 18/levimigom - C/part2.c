#include <stdio.h>
#include <stdlib.h>
#define HEX_CODE_SIZE 10

struct instruction {
	char direction;
	int meters;
};

int hex_digit_to_int(char digit){
	if('0' <= digit && digit <= '9'){
		return digit - '0';
	} else {
		return 10 + digit - 'a';
	}
}

struct instruction *parse_instruction(char *hex_code){
	struct instruction *instruction = malloc(sizeof(struct instruction));
	int i;

	instruction->direction = "RDLU"[hex_code[7] - '0'];
	instruction->meters = 0;
	for(i = 2; i < 7; i++){
		instruction->meters = (instruction->meters * 16) + hex_digit_to_int(hex_code[i]);
	}

	return instruction;
}

struct instruction **read_instructions(int *instruction_count){
	struct instruction **instructions;
	int size, capacity;
	char hex_code[HEX_CODE_SIZE];

	size = 0;
	capacity = 1;
	instructions = calloc(sizeof(struct instruction*), capacity);
	while(scanf(" %*c %*d %s", hex_code) != EOF){
		if(size == capacity){
			capacity *= 2;
			instructions = realloc(instructions, sizeof(struct instruction*) * capacity);
		}
		instructions[size++] = parse_instruction(hex_code);
	}

	*instruction_count = size;
	return instructions;
}

long my_abs(long n){ return(n < 0 ? -n : n); }

long calculate_area(struct instruction **instructions, int instruction_count){
	long area, area_between_points, x, y, next_x, next_y;
	int i;

	area = x = y = 0;
	for(i = 0; i < instruction_count; i++){
		next_x = x;
		next_y = y;

		switch(instructions[i]->direction){
			case 'U':
				next_x -= instructions[i]->meters;
				break;
			case 'D':
				next_x += instructions[i]->meters;
				break;
			case 'L':
				next_y -= instructions[i]->meters;
				break;
			case 'R':
				next_y += instructions[i]->meters;
				break;
		}

		area_between_points = (next_x * y) - (x * next_y);
		area += area_between_points + my_abs(next_x - x) + my_abs(next_y - y);
		
		x = next_x;
		y = next_y;
	}

	return my_abs(area) / 2 + 1;
}

void solve(){
	// read instructions
	struct instruction **instructions;
	int i, instruction_count;

	instructions = read_instructions(&instruction_count);
	printf("%ld\n", calculate_area(instructions, instruction_count));

	// free memory
	for(i = 0; i < instruction_count; i++){
		free(instructions[i]);
	}

	free(instructions);
}

int main(){
	solve();
	return 0;
}
