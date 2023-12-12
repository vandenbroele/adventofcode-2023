#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct list* read_springs(){
	int c;
	char *spring;
	struct list *springs = create_list();

	while((c = getchar()) != ' ' && c != EOF){
		spring = malloc(sizeof(char));
		*spring = c;
		add_to_list(springs, spring);	
	}

	return springs;
}

struct list* read_groups(){
	int c, *group;
	struct list *groups = create_list();
	
	group = malloc(sizeof(int));
	*group = 0;
	while((c = getchar()) != '\n' && c != EOF){
		if('0' <= c && c <= '9'){
			*group = (10 * (*group)) + c - '0'; 
		} else {
			add_to_list(groups, group);
			group = malloc(sizeof(int));
			*group = 0;
		}
	}
	add_to_list(groups, group);

	return groups;
}

int count_valid_arrangements(struct list *springs, struct list *groups, int spring_index, int group_index, int adjacent_spring_count){
	// arrangement end has been reached
	if(spring_index == springs->length){
		// arrangement is valid if all groups have been filled exactly
		return((group_index == groups->length && adjacent_spring_count == 0) || (group_index == groups->length - 1 && adjacent_spring_count == *((int*) groups->values[group_index])));
	}
	
	char spring = *((char*) springs->values[spring_index]);	
	int arrangements = 0;	
	
	// damaged spring
	if(spring == '#' || spring == '?'){
		arrangements += count_valid_arrangements(springs, groups, spring_index + 1, group_index, adjacent_spring_count + 1);
	}

	// operational spring
	if(spring == '.' || spring == '?'){
		if(adjacent_spring_count == 0){
			arrangements += count_valid_arrangements(springs, groups, spring_index + 1, group_index, 0);
		} else if(group_index < groups->length && *((int*)groups->values[group_index]) == adjacent_spring_count){
			arrangements += count_valid_arrangements(springs, groups, spring_index + 1, group_index + 1, 0);
		}
	}

	return arrangements;
}

void solve(){
	struct list *springs, *groups;
	int i, total_arrangement_count;

	total_arrangement_count = 0;
	springs = read_springs();
	while(0 < springs->length){
		groups = read_groups();
		
		total_arrangement_count += count_valid_arrangements(springs, groups, 0, 0, 0);
		
		destroy_list(springs);
		destroy_list(groups);

		springs = read_springs();
	}
	destroy_list(springs);

	printf("Total arrangements = %d\n", total_arrangement_count);
}

int main(){
	solve();

	return 0;
}
