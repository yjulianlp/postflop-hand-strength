#include "handranking.h"
#include "cardinfo.h"
#include <assert.h>

bool is_flush(Card** cards, int num_cards){
	for(int i = 0; i < num_cards-1; i++){
		if(cards[i]->type != cards[i+1]->type){
			return false;
		}
	}
	return true;
}

int tiebreak_flush(Card** hand1, Card** hand2, int num_cards){
	//positive if hand1 > hand2
	int difference = 0;
	for(int i = num_cards-1; i >= 0; i--){
		difference = value_difference(hand1[i], hand2[i]);
		if(difference != 0){
			break;
		}
	}
	return difference;
}

bool is_straight(Card** cards, int num_cards){
	bool no_duplicates = has_no_duplicate_cards(cards, num_cards);
	int range1 = value_difference(cards[num_cards-1], cards[0]);

	if(cards[num_cards-1]->value == 'A'){
		int range2 = value_to_strength(cards[num_cards-2]->value)-1;
		if(range2 == (num_cards-1) && no_duplicates){
			return true;
		}
	}
	if((range1 == (num_cards-1)) && no_duplicates){
			return true;
	}
	return false;
}

int tiebreak_straight(Card** hand1, Card** hand2, int num_cards){
	Card* hand1_strongest = NULL;
	Card* hand2_strongest = NULL;
	if(hand1[num_cards-1]->value == 'A' && hand1[num_cards-2]->value=='5'){
		hand1_strongest = hand1[num_cards-2];
	}else{
		hand1_strongest = hand1[num_cards-1];
	}
	if(hand2[num_cards-1]->value == 'A' && hand2[num_cards-2]->value=='5'){
		hand2_strongest = hand2[num_cards-2];
	}else{
		hand2_strongest = hand2[num_cards-1];
	}

	return value_difference(hand1_strongest, hand2_strongest);
}

bool is_straight_flush(Card** cards, int num_cards){
	return (is_straight(cards, num_cards) && is_flush(cards, num_cards)) ? true : false;
}

bool is_royal_flush(Card** cards, int num_cards){
	if(cards[0]->value == 'T' && cards[num_cards-1]->value == 'A'){
		if(is_straight(cards, num_cards) && is_flush(cards, num_cards)){
			return true;
		}
	}
	return false;
}

bool is_four_of_a_kind(Card** cards, int num_cards){
	//since cards is sorted, if the first card value == 4th card value then it has a four of a kind
	if(has_same_value(cards[0], cards[3])){
		return cards[0];
	}
	if(has_same_value(cards[num_cards-1], cards[num_cards-4])){
		return cards[num_cards-1];
	}
	return NULL;
}

int tiebreak_four_of_a_kind(Card** hand1, Card** hand2){
	Card* hand1_card = has_same_value(hand1[0], hand1[3]) ? hand1[0] : hand1[4];
	Card* hand2_card = has_same_value(hand2[0], hand2[3]) ? hand2[0] : hand2[4];
	
	return value_difference(hand1_card, hand2_card);

}
bool is_full_house(Card** cards, int num_cards){
	if(has_same_value(cards[0], cards[1]) && has_same_value(cards[2], cards[num_cards-1])){
		return true;
	}
	if(has_same_value(cards[0], cards[2]) && has_same_value(cards[3], cards[num_cards-1])){
		return true;
	}
	return false;
}

int tiebreak_full_house(Card** hand1, Card** hand2){
	//returns 1 if hand1 > hand2, -1 if hand2 > hand1, 0 if hand1 = hand2
	Card* outer_part_of_hand1;
	Card* outer_part_of_hand2;
	Card* inner_part_of_hand1;
	Card* inner_part_of_hand2;

	if(has_same_value(hand1[0], hand1[2])){
		outer_part_of_hand1 = hand1[0];
		inner_part_of_hand1 = hand1[4];
	}else{
		outer_part_of_hand1 = hand1[2];
		inner_part_of_hand1 = hand1[0];
	}

	if(has_same_value(hand2[0], hand2[2])){
		outer_part_of_hand2 = hand2[0];
		inner_part_of_hand2 = hand2[4];
	}else{
		outer_part_of_hand2 = hand2[2];
		inner_part_of_hand2 = hand2[0];
	}
	int difference = value_difference(outer_part_of_hand1, outer_part_of_hand2);

	if(difference == 0){ //if outer cards are the same
		difference = value_difference(inner_part_of_hand1, inner_part_of_hand2);
		if(difference == 0){
			return 0;
		}
		return (difference > 0) ? 1 : -1;
	}

	return (difference > 0) ? 1 : -1;
}

bool is_set(Card** cards, int num_cards){
	for(int i = 0 ; i < num_cards-2; i++){
		if(has_same_value(cards[i], cards[i+2])){
			return true;
		}
	}
	return false;
}

int tiebreak_set(Card** hand1, Card** hand2, int num_cards){
	Card* hand1_set_card = NULL;
	Card* hand2_set_card = NULL;
	for(int i = 0; i < num_cards-2; i++){
		if(has_same_value(hand1[i], hand1[i+2])){
			hand1_set_card = hand1[i];
			break;
		}
	}
	for(int i = 0; i < num_cards-2; i++){
		if(has_same_value(hand2[i], hand2[i+2])){
			hand2_set_card = hand2[i];
			break;
		}
	}

	return value_difference(hand1_set_card, hand2_set_card);
}

bool is_wheel(Card** hand, int num_cards){
		if(hand[num_cards-1]->value == 'A' && hand[num_cards-2]->value == '5'){
			return true;
		}
		return false;
}

int find_pairs(Card** cards, int num_cards){
	int pairs_found = 0;
	int i = 0;
	while(i < num_cards-1){
		if(has_same_value(cards[i], cards[i+1])){
			pairs_found++;
			i++;
		}
		i++;
	}

	return pairs_found;
}

int tiebreak_two_pair(Card** hand1, Card** hand2, int num_cards){
	Card* hand1_pairs[2] = {NULL, NULL};
	Card* hand2_pairs[2] = {NULL, NULL};
	int i = 0, index = 0;
	while(i < num_cards-1){
		if(has_same_value(hand1[i], hand1[i+1])){
			hand1_pairs[index]=hand1[i];
			index++, i++;
		}
		i++;
	}
	i = 0, index = 0;
	while(i < num_cards-1){
		if(has_same_value(hand2[i], hand2[i+1])){
			hand2_pairs[index]=hand2[i];
			index++, i++;
		}
		i++;
	}

	int hand1_top, hand1_bottom, hand2_top, hand2_bottom;
	if(value_difference(hand1_pairs[0], hand1_pairs[1]) > 0){
		hand1_top = 0, hand1_bottom = 1;
	}else{
		hand1_top = 1, hand1_bottom = 0;
	}
	if(value_difference(hand2_pairs[0], hand2_pairs[1]) > 0){
		hand2_top = 0, hand2_bottom = 1;
	}else{
		hand2_top = 1, hand2_bottom = 0;
	}
	int difference = value_difference(hand1_pairs[hand1_top], hand2_pairs[hand2_top]);
	if(difference != 0){
		return difference;
	}else{
		difference = value_difference(hand1_pairs[hand1_bottom], hand2_pairs[hand2_bottom]);
	}

	return difference;
}

int tiebreak_one_pair(Card** hand1, Card** hand2, int num_cards){
	Card* hand1_pair_card = NULL, *hand2_pair_card = NULL;
	for(int i = 0; i < num_cards-1; i++){
		if(has_same_value(hand1[i], hand1[i+1])){
			hand1_pair_card = hand1[i];
		}
		if(has_same_value(hand2[i], hand2[i+1])){
			hand2_pair_card = hand2[i];
		}
	}
	return value_difference(hand1_pair_card, hand2_pair_card);
}

int tiebreak_highcard(Card** hand1, Card** hand2, int num_cards){
	int difference = 0;
	for(int i = num_cards-1; i >= 0; i--){
		difference = value_difference(hand1[i], hand2[i]);
		if(difference!=0){
			return difference;
		}
	}
	return 0;
}

int tiebreaker(Card** hand1, Card** hand2, int num_cards, enum Hand_Ranking hand_rank){
	//return a positive int if hand1 stronger than hand2, negative if hand2 strongest than hand1 (0 if same)
	assert(hand2);
	printf("\npassed assert\n");

	int difference = 0;
	switch(hand_rank){
	case ROYAL_FLUSH:
		return 0;
	case STRAIGHT_FLUSH:
		int hand1_offset = (is_wheel(hand1, num_cards)) ? 2 : 1;
		int hand2_offset = (is_wheel(hand2, num_cards)) ? 2 : 1;
		difference = value_difference(hand1[num_cards-hand1_offset], hand2[num_cards-hand2_offset]);
		return difference;
	case FOUR_OF_A_KIND:
		difference = tiebreak_four_of_a_kind(hand1, hand2);
		return difference;
	case FULL_HOUSE:
		return tiebreak_full_house(hand1, hand2);
	case FLUSH:
		return tiebreak_flush(hand1, hand2, num_cards);
	case STRAIGHT:
		return tiebreak_straight(hand1, hand2, num_cards);
	case SET:
		return tiebreak_set(hand1, hand2, num_cards);
	case TWO_PAIR:
		return tiebreak_two_pair(hand1, hand2, num_cards);
	case ONE_PAIR:
		return tiebreak_one_pair(hand1, hand2, num_cards);
	case HIGHCARD:
		return tiebreak_highcard(hand1, hand2, num_cards);
	}
	return 0;
}