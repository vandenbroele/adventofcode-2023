#include <stdio.h>
#include <stdlib.h>
#define MAPS 7

struct range {
	long destination_range_start, source_range_start, range_length;
};

struct range_list {
	struct range **ranges;
	int items;	
};

struct range_list *create(int size){
	struct range_list *ranges = malloc(sizeof(struct range_list));
	ranges->ranges = calloc(sizeof(struct range*), size);
	ranges->items = 0;
	return ranges;
}

void add_to_range_list(struct range_list *range_list, struct range *range){
	range_list->ranges[range_list->items++] = range;
}

void destroy(struct range_list *range_list){
	int i;

	for(i = 0; i < range_list->items; i++){
		free(range_list->ranges[i]);
	}
	free(range_list->ranges);
	free(range_list);
}

long map_source(long source, struct range_list *map){
	int i;

	for(i = 0; i < map->items; i++){
		struct range* range = map->ranges[i];

		if(range->source_range_start <= source && source < range->source_range_start + range->range_length){
			return range->destination_range_start + (source - range->source_range_start);
		}
	}

	return source;
}

void solve(){
	// read seeds
	int i, number_of_seeds;
	
	scanf("%d", &number_of_seeds);

	long* seeds = calloc(sizeof(long), number_of_seeds);	
	for(i = 0; i < number_of_seeds; i++){
		scanf("%ld", &seeds[i]);
	}

	// read maps
	int j, size;
	struct range_list *maps[MAPS];

	for(i = 0; i < MAPS; i++){
		scanf("%d", &size);

		maps[i] = create(size);
		for(j = 0; j < size; j++){
			struct range* range = malloc(sizeof(struct range));
			scanf("%ld %ld %ld", &range->destination_range_start, &range->source_range_start, &range->range_length);
			add_to_range_list(maps[i], range);
		}
	}

	// solve the problem
	long lowest_location_number;

	for(i = 0; i < number_of_seeds; i++){
		long location_number = seeds[i];
		
		for(j = 0; j < MAPS; j++){
			location_number = map_source(location_number, maps[j]);
		}

		if(i == 0 || location_number < lowest_location_number){
			lowest_location_number = location_number;
		}
	}

	printf("%ld\n", lowest_location_number);

	// clean up
	for(i = 0; i < MAPS; i++){
		destroy(maps[i]);
	}
	free(seeds);
}

int main(){
	/* Modified the input to the following format to make it easier to process
	 * 
	 * number_of_seeds
	 * seed1 seed2 seed3 ... seedn
	 *
	 * number_of_ranges_map1
	 * destination_range_start_range1 destination_range_start_range1 range_length_range1
	 * destination_range_start_range2 destination_range_start_range2 range_length_range2
	 * ...
	 */
	solve();

	return 0;
}
