#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_DIGITS 20
#define MIN_X 200000000000000
#define MAX_X 400000000000000
#define MIN_Y 200000000000000
#define MAX_Y 400000000000000

struct hailstone {
	long px, py, vx, vy;
};

long my_atoi(char *s){
	long value = 0;
	bool negate = false;

	if(*s == '-'){
		negate = true;
		s++;
	}

	while('0' <= *s && *s <= '9'){
		value = (value * 10) + *s - '0';
		s++;
	}

	if(negate){
		value = -value;
	}

	return value;
}

struct hailstone *read_hailstone(){
	char s[4][MAX_DIGITS];

	if(scanf("%s %s %*s %*s %s %s %*s", s[0], s[1], s[2], s[3]) == EOF){
		return 0;
	}

	struct hailstone *stone = malloc(sizeof(struct hailstone));
	stone->px = my_atoi(s[0]);
	stone->py = my_atoi(s[1]);
	stone->vx = my_atoi(s[2]);
	stone->vy = my_atoi(s[3]);
	
	return stone;
}

struct hailstone **read_hailstones(int *hailstone_count){
	int size, capacity;
	struct hailstone *stone, **stones;

	size = 0;
	capacity = 2;
	stones = calloc(sizeof(struct hailstone*), capacity); 
	while(0 < (stone = read_hailstone())){
		if(size == capacity){
			stones = realloc(stones, sizeof(struct hailstone*) * (capacity *= 2));
		}
		stones[size++] = stone;
	}

	*hailstone_count = size;
	return stones;
}

void free_hailstones(struct hailstone **hailstones, int hailstone_count){
	int i;

	for(i = 0; i < hailstone_count; i++){
		free(hailstones[i]);
	}
	free(hailstones);
}

bool stones_intersect(struct hailstone *a, struct hailstone *b, long double *intersection_x, long double *intersection_y){
	long double dx = b->px - a->px;
    	long double dy = b->py - a->py;
	long double det = (b->vx * a->vy) - (b->vy * a->vx);
	
	if(det != 0){
		long double u = (dy * b->vx - dx * b->vy) / det;
		long double v = (dy * a->vx - dx * a->vy) / det;

		if(0 <= u && 0 <= v){
			*intersection_x = a->px + (u * a->vx);
			*intersection_y = a->py + (u * a->vy);
		       return true;	
		}
	}

	return false;
}	
	
int count_intersections(struct hailstone **hailstones, int hailstone_count){
	int i, j, count;
	long double intersect_x, intersect_y;
	
	count = 0;
	for(i = 0; i < hailstone_count; i++){
		for(j = i + 1; j < hailstone_count; j++){
			if(stones_intersect(hailstones[i], hailstones[j], &intersect_x, &intersect_y)){
				if(MIN_X <= intersect_x && intersect_x <= MAX_X && MIN_Y <= intersect_y && intersect_y <= MAX_Y){
					count++;
					printf("Line %d and %d intersect at position %Lf,%Lf within the search space!\n", i, j, intersect_x, intersect_y);
				} else {
					printf("Line %d and %d intersect outside of the search space!\n", i, j);
				}
			}
		}
	}

	return count;
}

void solve(){
	int hailstone_count;
	struct hailstone **hailstones = read_hailstones(&hailstone_count);

	printf("Found %d intersections\n", count_intersections(hailstones, hailstone_count));

	free_hailstones(hailstones, hailstone_count);
}

int main(){
	solve();
	return 0;
}
