#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define bool unsigned char
#define true 1
#define false 0
#define HANDS 1000

char* cards = "AKQT98765432";

enum EScore {
	Five_Of_A_Kind = 6,
	Four_Of_A_Kind = 5,
	Full_House = 4,
	Three_Of_A_Kind = 3,
	Two_Pair = 2,
	One_Pair = 1,
	High_Card = 0
};

enum EScore get_hand_score(char *hand){
	short character_frequency[256] = { 0 };
	int i;

	for(i = 0; i < strlen(hand); i++){
		character_frequency[hand[i]]++;
	}
	
	int pairs = 0;
	bool contains_three_of_a_kind = false;
	for(i = 0; i < strlen(cards); i++){
		if(character_frequency[cards[i]] + character_frequency['J'] == 5){
			return Five_Of_A_Kind;
		}

		if(character_frequency[cards[i]] == 4){
			return Four_Of_A_Kind;
		}

		if(character_frequency[cards[i]] == 3){
			contains_three_of_a_kind = true;
		}

		if(character_frequency[cards[i]] == 2){
			pairs++;
		}
	}
	
	if((contains_three_of_a_kind && character_frequency['J'] == 1) || (pairs == 1 && character_frequency['J'] == 2)){
		return Four_Of_A_Kind;
	} if((pairs == 2 && character_frequency['J'] == 1) || (contains_three_of_a_kind && pairs == 1)){
		return Full_House;
	} else if((pairs == 1 && character_frequency['J'] == 1) || character_frequency['J'] == 2 || contains_three_of_a_kind){
		return Three_Of_A_Kind;
	} else if(pairs == 2){
		return Two_Pair;
	} else if(pairs == 1 || character_frequency['J'] == 1){
		return One_Pair;
	} else {
		return High_Card;
	}
}

int get_card_score(char card){
	int i;

	for(i = 0; i < strlen(cards); i++){
		if(card == cards[i]){
			return strlen(cards) - i;
		}
	}

	return 0;
}

bool is_lower_than(char *hand1, char *hand2){
	if(get_hand_score(hand1) < get_hand_score(hand2)){
		return true;
	}

	if(get_hand_score(hand1) > get_hand_score(hand2)){
		return false;
	}

	int i;

	for(i = 0; i < strlen(hand1); i++){
		if(get_card_score(hand1[i]) < get_card_score(hand2[i])){
			return true;
		}

		if(get_card_score(hand1[i]) > get_card_score(hand2[i])){
			return false;
		}
	}

	return false;
}

int main(){
	char hands[HANDS][6];
	int hand_count, bids[HANDS];

	for(hand_count = 0; scanf("%s %d", hands[hand_count], &bids[hand_count]) != EOF; hand_count++);

	int i, j, rank, total_winnings;
	
	total_winnings = 0;
	for(i = 0; i < hand_count; i++){
		rank = 1;
		for(j = 0; j < hand_count; j++){
			if(j != i && is_lower_than(hands[j], hands[i])){
				rank++;
			}
		}

		total_winnings += bids[i] * rank;
	}

	printf("Total winnings: %d\n", total_winnings);

	return 0;
}
