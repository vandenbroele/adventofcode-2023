#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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

long min(long a, long b) { return(a < b ? a : b); }

long map_source(long source, struct range_list *map, long *source_with_same_path){
	int i;

	// check if it is in any ranges
	for(i = 0; i < map->items; i++){
		struct range* range = map->ranges[i];

		if(range->source_range_start <= source && source < range->source_range_start + range->range_length){
			*source_with_same_path = min(*source_with_same_path, range->source_range_start + range->range_length - source);
			return range->destination_range_start + (source - range->source_range_start);
		}
	}
	
	// if its not, find the distance to the closest range
	long distance_to_source_range = LONG_MAX; 
	
	for(i = 0; i < map->items; i++){
		struct range* range = map->ranges[i];

		if(source < range->source_range_start){
			distance_to_source_range = min(distance_to_source_range, range->source_range_start - source);
		}
	}

	*source_with_same_path = min(*source_with_same_path, distance_to_source_range);

	return source;
}

void solve(){
	/* Optimisation strategy:
	 * If you keep track of the ranges during a mapping, you can know how many of the next
	 * seeds will take the exact same path. The lowest seed number will always have a lower location
	 * number than all other seeds taking the same path, which means you can just skip calculating
	 * the path for those seeds. 
	 *
	 * In the worst case, where each individual seed number has a unique path, this would be exactly
	 * as slow as the brute force solution. In reality, it is alot faster, because the ranges are
	 * really big in the input data, leading to alot of seeds with the same mapping path.  
	 */

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
	long lowest_location_number, k;

	lowest_location_number = -1;
	for(i = 0; i < number_of_seeds; i += 2){
		for(k = 0; k < seeds[i + 1]; k++){
			long source = seeds[i] + k;
			long seeds_with_same_path = seeds[i + 1] - k - 1;

			for(j = 0; j < MAPS; j++){
				source = map_source(source, maps[j], &seeds_with_same_path);
			}

			if(lowest_location_number == -1 || source < lowest_location_number){
				lowest_location_number = source;
			}

			if(0 < seeds_with_same_path){
				k += seeds_with_same_path - 1;
			}
		}	

		printf("Finished checking range %ld through %ld\n", seeds[i], seeds[i] + seeds[i + 1]);
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
