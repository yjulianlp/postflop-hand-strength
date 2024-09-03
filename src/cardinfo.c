#include "../include/cardinfo.h"

int value_difference(Card* card1, Card* card2){
	//positive if card1 > card2
	return value_to_strength(card1->value) - value_to_strength(card2->value);
}

int suit_difference(Card* card1, Card* card2){
	//positive if card1 > card2
	return (card1->type - card2->type);
}

int card_difference(Card* card1, Card* card2){
	//positive if card1 > card2
	int difference = value_difference(card1, card2);

	if(difference == 0){
		difference = suit_difference(card1, card2);
	}

	return difference;
}

int pair_difference(Card** pair1, Card** pair2){
	int difference = card_difference(pair1[0], pair2[0]);
	if(difference==0){
		difference = card_difference(pair1[1], pair2[1]);
	}

	return difference;
}

bool is_same_card(Card* card_one, Card* card_two){
	return ((card_one->value == card_two->value) && (card_one->type == card_two->type)) ? true : false;
}

bool is_same_pair(Card** pair1, Card** pair2){
	return (is_same_card(pair1[0], pair2[0]) && is_same_card(pair1[1], pair2[1]));
}

bool has_same_value(Card* card1, Card* card2){
	return (card1->value == card2->value) ? true : false;
}

bool has_no_duplicate_cards(Card** cards, int num_cards){
	for(int i = 0; i < num_cards-1; i++){
		if(cards[i]->value==cards[i+1]->value){
			return false;
		}
	}
	return true;
}

int value_to_strength(char value){
	if(value >= 50 && value <= 57){
		return value-48;
	}
	else{
		switch(value){
			case 'T':
				return 10;
			case 'J':
				return 11;
			case 'Q':
				return 12;
			case 'K':
				return 13;
			case 'A':
				return 14;
		}
	}
	return 0;
}

int suit_to_int(char suit){
	switch(suit){
	case 'h':
		return 1;
	case 'c':
		return 2;
	case 's':
		return 3;
	case 'd':
		return 4;
	default:
		return -1;
	}
}

char int_to_suit(int suit){
	switch(suit){
	case HEART:
		return 'h';
	case CLUB:
		return 'c';
	case SPADE:
		return 's';
	case DIAMOND:
		return 'd';
	default:
		return -1;
	}
}

void print_card_information(Card* card){
	printf("| %c%c ", card->value, int_to_suit(card->type));
}

void print_cards(Card** card_container, int num_cards){
	for(int i = 0; i < num_cards; i++){
		print_card_information(card_container[i]);
	}
	printf("|");
}