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

void unfold_springs(struct list *springs){
	int i, j, original_springs_length; 
	char *c;

	original_springs_length = springs->length;
	for(i = 0; i < 4; i++){
		c = malloc(sizeof(char));
		*c = '?';
		add_to_list(springs, c);
			
		for(j = 0; j < original_springs_length; j++){
			c = malloc(sizeof(char));
			*c = *((char*) get_from_list(springs, j));
			add_to_list(springs, c);
		}
	}
}

void unfold_groups(struct list *groups){
	int i, j, original_groups_length;
	int *group;

	original_groups_length = groups->length;
	for(i = 0; i < 4; i++){
		for(j = 0; j < original_groups_length; j++){
			group = malloc(sizeof(int));
			*group = *((int*) get_from_list(groups, j));
			add_to_list(groups, group);
		}
	}
}

long ***create_cache(struct list *springs, struct list *groups){
	int i, j, k;
        long ***cache;

	cache = calloc(sizeof(long**), springs->length);
	for(i = 0; i < springs->length; i++){
		cache[i] = calloc(sizeof(long*), groups->length + 1);
		
		for(j = 0; j <= groups->length; j++){
			cache[i][j] = calloc(sizeof(long), springs->length);

			for(k = 0; k < springs->length; k++){
				cache[i][j][k] = -1;
			}
		}
	}	

	return cache;
}

void destroy_cache(struct list *springs, struct list *groups, long ***cache){
	int i, j;

	for(i = 0; i < springs->length; i++){
		for(j = 0; j <= groups->length; j++){
			free(cache[i][j]);
		}
		free(cache[i]);
	}
	free(cache);
}

long count_valid_arrangements(struct list *springs, struct list *groups, int spring_index, int group_index, int adjacent_spring_count, long ***cache){
	// arrangement end has been reached
	if(spring_index == springs->length){
		// arrangement is valid if all groups have been filled exactly
		return((group_index == groups->length && adjacent_spring_count == 0) || (group_index == groups->length - 1 && adjacent_spring_count == *((int*) groups->values[group_index])));
	}

	if(cache[spring_index][group_index][adjacent_spring_count] == -1){
		char spring = *((char*) springs->values[spring_index]);	
		long arrangements = 0;	
	
		// damaged spring
		if(spring == '#' || spring == '?'){
			arrangements += count_valid_arrangements(springs, groups, spring_index + 1, group_index, adjacent_spring_count + 1, cache);
		}

		// operational spring
		if(spring == '.' || spring == '?'){
			if(adjacent_spring_count == 0){
				arrangements += count_valid_arrangements(springs, groups, spring_index + 1, group_index, 0,  cache);
			} else if(group_index < groups->length && *((int*)groups->values[group_index]) == adjacent_spring_count){
				arrangements += count_valid_arrangements(springs, groups, spring_index + 1, group_index + 1, 0, cache);
			}
		}

		cache[spring_index][group_index][adjacent_spring_count] = arrangements;
	}

	return cache[spring_index][group_index][adjacent_spring_count];
}

void solve(){
	struct list *springs, *groups;
	int i;
	long total_arrangement_count, ***cache;

	total_arrangement_count = 0;
	springs = read_springs();
	while(0 < springs->length){
		groups = read_groups();
		
		unfold_springs(springs);
		unfold_groups(groups);
		
		cache = create_cache(springs, groups);
		total_arrangement_count += count_valid_arrangements(springs, groups, 0, 0, 0, cache);
		destroy_cache(springs, groups, cache);

		destroy_list(springs);
		destroy_list(groups);

		springs = read_springs();
	}
	destroy_list(springs);

	printf("Total arrangements = %ld\n", total_arrangement_count);
}

int main(){
	solve();

	return 0;
}
