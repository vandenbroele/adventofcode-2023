#include <stdio.h>
#include <stdlib.h>

long calculate_number_of_ways_to_win(long time, long distance){
	long i, number_of_ways_to_win;

	number_of_ways_to_win = 0;
	for(i = 1; i < time; i++){
		if(distance < (time - i) * i){
			number_of_ways_to_win++;
		}
	}

	return number_of_ways_to_win;
}

int main(){
	/* Modified the input to the following format:
	 * time(0) distance(0)
	 */
	long time, distance;
	
	scanf("%ld %ld", &time, &distance);
	printf("%ld\n", calculate_number_of_ways_to_win(time, distance));

	return 0;
}

