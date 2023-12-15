#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BOX_COUNT 256

struct instruction {
	char *label, operation;
	int focus_length;
};

void free_instruction(struct instruction *instruction){
	free(instruction->label);
	free(instruction);
}

struct lens {
	char *label;
	int focus_length;
	struct lens *next;	
};

struct lens *create_lens(char *label, int focus_length){
	struct lens *lens = malloc(sizeof(struct lens));
	lens->label = calloc(sizeof(char), strlen(label) + 1);

	int i;

	for(i = 0; i < strlen(label); i++){
		lens->label[i] = label[i];
	}
	lens->label[i] = '\0';
	lens->focus_length = focus_length;
	lens->next = 0;

	return lens;
}

void free_lens(struct lens *lens){
	free(lens->label);
	free(lens);
}

struct box {
	struct lens *lenses;
};

void free_box(struct box *box){
	struct lens *current, *next;

	current = box->lenses;
	while(current != 0){
		next = current->next;
		free_lens(current);
		current = next;
	}
	free(box);
}

int hash(char *string){
	int i, hash;

	hash = 0;
	for(i = 0; i < strlen(string); i++){
		hash = ((hash + string[i]) * 17) % 256;
	}

	return hash;
}

struct instruction *read_next_instruction(){
	int c, size, length;
	struct instruction *instruction = malloc(sizeof(struct instruction));
	instruction->label = malloc(sizeof(char));

	// read the label
	size = 1;
	length = 0;
	while((c = getchar()) != EOF && c != '-' && c != '='){
		if(length == size){
			size *= 2;
			instruction->label = realloc(instruction->label, sizeof(char) * size);
		}	
		instruction->label[length++] = c;
	}

	if(c == EOF){
		free_instruction(instruction);
		return 0;
	}

	if(length == size){
		instruction->label = realloc(instruction->label, sizeof(char) * (size + 1));
	}
	instruction->label[length] = '\0';

	// read the operation
	instruction->operation = c;
	
	// read the focus length (if instruction is '=')
	if(instruction->operation == '='){
		instruction->focus_length = (c = getchar()) - '0';
	}
	getchar();

	return instruction;
}

void add_lens(struct box *box, char *label, int focus_length){
	struct lens *lens = box->lenses;

	while(lens->next != 0 && strcmp(lens->next->label, label) != 0){
		lens = lens->next;
	}

	// if lens with same label exists, replace it
	if(lens->next != 0){
		lens->next->focus_length = focus_length;
	}
	// otherwise, add a new lens to the end of the box
	else {
		lens->next = create_lens(label, focus_length);
	}
}

void remove_lens(struct box *box, char *label){
	struct lens *lens = box->lenses;

	while(lens->next != 0 && strcmp(lens->next->label, label) != 0){
		lens = lens->next;
	}

	// if the lens exists, remove it
	if(lens->next != 0){
		struct lens *remove = lens->next;
		lens->next = lens->next->next;
		free_lens(remove);
	}
}

void handle_instruction(struct instruction *instruction, struct box *boxes[BOX_COUNT]){
	int hashcode = hash(instruction->label);

	if(instruction->operation == '='){
		add_lens(boxes[hashcode], instruction->label, instruction->focus_length);
	} else {
		remove_lens(boxes[hashcode], instruction->label);
	}
}

int calculate_total_focusing_power(struct box *boxes[BOX_COUNT]){
	int i, j, total_focusing_power;
	struct lens *lens;

	total_focusing_power = 0;
	for(i = 0; i  < BOX_COUNT; i++){
		lens = boxes[i]->lenses;
		for(j = 0; lens != 0; j++){
			total_focusing_power += (1 + i) * j * lens->focus_length;
			lens = lens->next;
		}
	}

	return total_focusing_power;
}

void solve(){
	struct instruction *instruction;
	struct box *boxes[BOX_COUNT];
	int i;

	// init "empty" boxes
	for(i = 0; i < BOX_COUNT; i++){
		boxes[i] = malloc(sizeof(struct box));
		boxes[i]->lenses = create_lens("\0", 0);
	}

	instruction = read_next_instruction();
	while(instruction != 0){
		handle_instruction(instruction, boxes);
		free_instruction(instruction);
		instruction = read_next_instruction();
	}

	printf("%d\n", calculate_total_focusing_power(boxes));

	// cleanup
	for(i = 0; i < BOX_COUNT; i++){
		free_box(boxes[i]);
	}
}

int main(){
	solve();

	return 0;
}
