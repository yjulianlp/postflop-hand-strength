#include "../include/managecards.h"
#include "../include/cardinfo.h"

int find_card(Card** sorted_card_container, Card* card, int container_size){
	printf("finding card");
	int low = 0, high = container_size-1;
	while(low <= high){
		int middle = (low+high)/2;
		printf("low: %d high: %d middle: %d\n", low, high, middle);
		printf("looking at card: ");
		print_card_information(sorted_card_container[middle]);
		printf("\n");
		if(is_same_card(sorted_card_container[middle], card)){
			return middle;
		}else{
			int difference = value_difference(sorted_card_container[middle], card);
			printf("difference of %d \n", difference);
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
	swap_cards(cards, index, (*num_cards)-1);
	cards = realloc(cards, sizeof(Card*)*((*num_cards)-1));
	(*num_cards)--;
	return cards;
}

Card** swap_cards(Card** cards, int index_of_card1, int index_of_card2){
	Card* temp = cards[index_of_card1];
	cards[index_of_card1] = cards[index_of_card2];
	cards[index_of_card2] = temp;

	return cards;
}
