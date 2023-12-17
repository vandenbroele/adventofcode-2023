#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"
#define TEST_SIZE 1000

int main(){
	struct priority_queue *minimum_priority_queue = create_priority_queue();
	int i, priority, previous_number, numbers, *num;
	
	printf("Inserting...\n");
	for(i = 0; i < TEST_SIZE; i++){
		priority = ((i + 6779) * 2141) % (7919 * 43);
		num = malloc(sizeof(int));
		*num = priority;
		enqueue_priority_queue(minimum_priority_queue, num, priority);
	}
	
	printf("Removing...\n");
	numbers = 0;
	previous_number = -1;
	while(0 < minimum_priority_queue->size){
		num = dequeue_priority_queue(minimum_priority_queue);
		
		if(*num < previous_number){
			printf("Test failed, current number %d has lower priority than previous number %d\n", *num, previous_number);
		}
		
		numbers++;
		previous_number = *num;
		free(num);
	}

	if(numbers == TEST_SIZE){
		printf("Test passed, all %d items were returned in correct order\n", TEST_SIZE);
	}

	free_priority_queue(minimum_priority_queue);

	return 0;
}
