#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define bool unsigned char
#define false 0
#define true 1
#define MAX_NUMBER 100
#define MAX_CARDS 256

int count_scratch_cards(int* winning_numbers_on_card, int cards){
	int* scratch_cards = calloc(sizeof(int), cards + 1);
	int i, j, total_scratch_cards;

	total_scratch_cards = 0;
	for(i = cards; 0 < i; i--){
		scratch_cards[i] = 1;

		for(j = 1; j <= winning_numbers_on_card[i]; j++){
			scratch_cards[i] += scratch_cards[i + j];
		}
		
		total_scratch_cards += scratch_cards[i];
	}

	free(scratch_cards);

	return total_scratch_cards;
}

void solve(){
	bool winning_numbers[MAX_NUMBER], passed_separator;
	char word[5];
	int number, card_number;
        int winning_numbers_on_card[MAX_CARDS] = { 0 };

	passed_separator = false;
	card_number = 0;
	while(scanf("%s", word) != EOF){
		if(strcmp(word, "Card") == 0){
			card_number++;
			
			// skip the next word because it is just the card number
			scanf("%*s"); 

			// reset card state
			passed_separator = false;
			memset(winning_numbers, false, sizeof(winning_numbers));
		} else if(strcmp(word, "|") == 0){
			passed_separator = true;
		} else {
			number = atoi(word);

			if(passed_separator){
				if(winning_numbers[number]){
					winning_numbers_on_card[card_number]++;
				}
			} else {
				winning_numbers[number] = true;
			}
		}
	}

	printf("%d\n", count_scratch_cards(winning_numbers_on_card, card_number));
}

int main(){
	solve();

	return 0;
}
