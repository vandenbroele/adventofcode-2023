#include <stdio.h>
#include <stdlib.h>

char *read_next_step(int *length){
	int c, size;
	char *step;

	size = 1;
	*length = 0;
	step = malloc(sizeof(char));
	while((c = getchar()) != EOF && c != ',' && c != '\n'){
		if(*length == size){
			size *= 2;
			step = realloc(step, sizeof(char) * size);
		}	
		step[(*length)++] = c;
	}

	return step;
}

int hash(char *string, int length){
	int i, hash;

	hash = 0;
	for(i = 0; i < length; i++){
		hash = ((hash + string[i]) * 17) % 256;
	}

	return hash;
}

void solve(){
	int i, length, hash_sum;
	char *step;

	hash_sum = 0;
	step = read_next_step(&length);
	while(0 < length){
		hash_sum += hash(step, length);
		free(step);
		step = read_next_step(&length);
	}
	free(step);

	printf("%d\n", hash_sum);
}

int main(){
	solve();

	return 0;
}
