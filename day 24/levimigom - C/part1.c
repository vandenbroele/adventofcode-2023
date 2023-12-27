#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_DIGITS 20
#define MIN_X 200000000000000
#define MAX_X 400000000000000
#define MIN_Y 200000000000000
#define MAX_Y 400000000000000

struct hailstone {
	long px, py, vx, vy, ex, ey;
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

long min(int a, int b) { return(a < b ? a : b); }

void calculate_line_end(struct hailstone *hailstone){
	long times;

	if(hailstone->vx < 0){
		times = (hailstone->px - MIN_X) / (-hailstone->vx);
	} else {
		times = (MAX_X - hailstone->px) / hailstone->vx;
	}

	if(hailstone->vy < 0){
		times = min(times, (hailstone->py - MIN_Y) / (-hailstone->vy));
	} else {
		times = min(times, (MAX_Y - hailstone->py) / hailstone->vy);
	}

	hailstone->ex = hailstone->px + (times * hailstone->vx);
	hailstone->ey = hailstone->py + (times * hailstone->vy);
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
	calculate_line_end(stone);
	
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

// TODO: incorrect because the multiplication overflows
bool lines_intersect(long double p0_x, long double p0_y, long double p1_x, long double p1_y, long double p2_x, long double p2_y, long double p3_x, long double p3_y, double *i_x, double *i_y){
    long double s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;
    s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;
    s2_y = p3_y - p2_y;

    long double s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
	if (i_x != NULL)
            *i_x = p0_x + (t * s1_x);
        if (i_y != NULL)
            *i_y = p0_y + (t * s1_y);
        return true;
    }

    return false;
}

int count_intersections(struct hailstone **hailstones, int hailstone_count){
	int i, j, count;
	double intersect_x, intersect_y;
	
	count = 0;
	for(i = 0; i < hailstone_count; i++){
		for(j = i + 1; j < hailstone_count; j++){
			if(lines_intersect(hailstones[i]->px, hailstones[i]->py, hailstones[i]->ex, hailstones[i]->ey,
						hailstones[j]->px, hailstones[j]->py, hailstones[j]->ex, hailstones[j]->ey, &intersect_x, &intersect_y)){
				printf("Line %d and %d intersect at position %lf,%lf!\n", i, j, intersect_x, intersect_y);
				count++;
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
