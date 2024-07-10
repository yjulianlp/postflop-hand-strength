#include "../include/managecards.h"
#include "../include/cardinfo.h"

int find_card(Card** sorted_card_container, Card* card, int container_size){
	int low = 0, high = container_size-1;
	while(low <= high){
		int middle = (low+high)/2;
		if(is_same_card(sorted_card_container[middle], card)){
			return middle;
		}else{
			int difference = value_difference(sorted_card_container[middle], card);
			if(difference == 0){ //correct value but wrong suit
				difference = suit_difference(sorted_card_container[middle], card);
			}
			
			if(difference > 0){ 
				//card has a larger value
				high = middle-1;
			}else{ 
				//card has a smaller value
				low = middle+1;
			}
		}
	}

	return -1; //not found
}

Card** remove_card(Card** cards, Card* card_to_remove, int* num_cards){
	int index = find_card(cards, card_to_remove, *num_cards);

	if(index == -1){ //card not found
		return cards;
	}

	swap_cards(cards, index, (*num_cards)-1);
	cards = realloc(cards, sizeof(Card*)*((*num_cards)-1));
	(*num_cards)--;
	return cards;
}

Card*** remove_pairs_with_card(Card*** card_pair_array, Card* card, int* num_pairs){
	int index = 0;
	while(index < (*num_pairs)){
		if(is_same_card(card_pair_array[index][0], card) || is_same_card(card_pair_array[index][1], card)){
			if(index == (*num_pairs-1)){
				card_pair_array = realloc(card_pair_array, sizeof(Card**)*((*num_pairs)-1));
				(*num_pairs)--;
				index++;
			}else{
				swap_pairs(card_pair_array, index, (*num_pairs)-1);
				card_pair_array = realloc(card_pair_array, sizeof(Card**)*((*num_pairs)-1));
				(*num_pairs)--;		
			}
		}else{
			index++;
		}
	}

	return card_pair_array;
}

Card*** swap_pairs(Card*** cards, int index_of_pair1, int index_of_pair2){
	Card** temp_pair = cards[index_of_pair1];
	cards[index_of_pair1] = cards[index_of_pair2];
	cards[index_of_pair2] = temp_pair;

	return cards;
}

Card** swap_cards(Card** cards, int index_of_card1, int index_of_card2){
	Card* temp = cards[index_of_card1];
	cards[index_of_card1] = cards[index_of_card2];
	cards[index_of_card2] = temp;

	return cards;
}

Card** add_card(Card** hand, Card* card, int* hand_size){
	hand = realloc(hand, sizeof(Card*)*((*hand_size)+1));
	hand[(*hand_size)] = card;
	(*hand_size)++;
	return hand;
}
