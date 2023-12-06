#include <stdio.h>
#include <stdlib.h>

int calculate_number_of_ways_to_win(int time, int distance){
	int i, number_of_ways_to_win;

	number_of_ways_to_win = 0;
	for(i = 1; i < time; i++){
		if(distance < (time - i) * i){
			number_of_ways_to_win++;
		}
	}

	return number_of_ways_to_win;
}

void solve(int races, int *times, int *distances){
	int i, solution;
	
	solution = 1;
	for(i = 0; i < races; i++){
		solution *= calculate_number_of_ways_to_win(times[i], distances[i]);
	}

	printf("%d\n", solution);
}

int main(){
	/* Modified the input to the following format:
	 * number_of_races
	 * time(0) distance(0)
	 * time(1) distance(1)
	 * ...
	 * time(number_of_races - 1) distance(number_of_races - 1)
	 */

	int i, races;
	scanf("%d", &races);

	int *times = calloc(sizeof(int), races);
	int *distances = calloc(sizeof(int), races);

	for(i = 0; i < races; i++){
		scanf("%d %d", &times[i], &distances[i]);
	}

	solve(races, times, distances);

	free(times);
	free(distances);	

	return 0;
}

