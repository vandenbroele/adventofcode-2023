#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define bool unsigned char
#define false 0
#define true 1
#define MAX_NUMBER 100

void solve(){
	bool winning_numbers[MAX_NUMBER], passed_separator;
	char word[5];
	int point_sum, number, winning_numbers_on_card;

	passed_separator = false;
	point_sum = winning_numbers_on_card = 0;
	while(scanf("%s", word) != EOF){
		if(strcmp(word, "Card") == 0){
			// add card points to sum
			if(0 < winning_numbers_on_card){
				point_sum += 1 << (winning_numbers_on_card - 1);
			}
			
			// skip the next word because it is just the card number
			scanf("%*s"); 

			// reset card state
			passed_separator = false;
			memset(winning_numbers, false, sizeof(winning_numbers));
			winning_numbers_on_card = 0;
		} else if(strcmp(word, "|") == 0){
			passed_separator = true;
		} else {
			number = atoi(word);

			if(passed_separator){
				winning_numbers_on_card += winning_numbers[number];
			} else {
				winning_numbers[number] = true;
			}
		}
	}

	printf("%d\n", point_sum);
}

int main(){
	solve();

	return 0;
}
