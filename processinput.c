#include "processinput.h"

char* get_cards(int card_count){
	char* user_input = malloc(sizeof(char)*( card_count*3));
	fgets(user_input, sizeof(char)*(card_count*3)+1, stdin);

	return user_input;	
}

int suite_to_int(char suite){
	switch(suite){
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

char int_to_suite(int suite){
	switch(suite){
	case 1:
		return 'h';
	case 2:
		return 'c';
	case 3:
		return 's';
	case 4:
		return 'd';
	default:
		return -1;
	}
}

void process_cards(Card** card_container, int number_of_cards, char* card_string){
	for(int i = 0; i < number_of_cards; i++){
		*(card_container+i) = malloc(sizeof(Card));
		card_container[i]->value = card_string[3*i];
		card_container[i]->type = suite_to_int(card_string[(3*i)+1]);
	}
}

void print_debug(Card** cards, int card_count){
	for(int i = 0; i < card_count; i++){
		print_card_information(*(cards+i));
	}
}

void print_card_information(Card* card){
	printf("| %c%c ", card->value, int_to_suite(card->type));
}

void print_cards(Card** card_container, int num_cards){
	printf("\n+++++++++++\n");
	for(int i = 0; i < num_cards; i++){
		print_card_information(card_container[i]);
	}
	printf("|");
	printf("\n+++++++++++\n");
}

bool is_same_card(Card* card_one, Card* card_two){
	#ifdef DEBUG
	printf("Comparing %c%c with %c%c \n", card_one->value, int_to_suite(card_one->type), card_two->value, int_to_suite(card_two->type));
	#endif
	return ((card_one->value == card_two->value) && (card_one->type == card_two->type)) ? true : false;
}

bool is_in_use(Card* card, Card** cards_to_check, int num_cards_to_check){
	for(int i = 0; i < num_cards_to_check; i++){
		if(is_same_card(card, cards_to_check[i])){
			return true;
		}
	}
	return false;
}

void setup_unused_cards(Card** unused_card_container, int cards_remaining, Card** hand_cards, Card** table_cards, int hand_size, int table_size){
	char CARD_VALUES[13] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

	int index = 0;
	while(index < cards_remaining){
		bool added = false;
		for(int i = 0; i < CARD_VALUE_RANGE; i++){
			for(int j = 0; j < SUITE_RANGE; j++){
				Card* temp = malloc(sizeof(Card));
				temp->value = CARD_VALUES[i];
				temp->type = j+1;
				if((!is_in_use(temp, hand_cards, hand_size)) && (!is_in_use(temp, table_cards, table_size)) && (!is_in_use(temp, unused_card_container, index))){
					unused_card_container[index] = temp;
					index++;
					added = true;
					printf("added a card: \n");
					print_card_information(temp);
					printf("\n\n+++++++++");
					break;
				}
			}
			if(added){
				break;
			}
		}
	}
}

void free_card_mem(Card** card_container, int num_cards){
	for(int i = 0; i < num_cards; i++){
		free(card_container[i]);
	}
}

Card*** generate_possible_pairs(Card*** card_pair_container, Card** possible_cards, int num_possible_cards, int* pair_count){
	int number_of_pairs = *pair_count;
	for(int i = 0; i < num_possible_cards; i++){
		for(int j = i+1; j < num_possible_cards; j++){
			Card** temp_pair_arr = malloc(sizeof(Card*)*2);
			temp_pair_arr[0] = possible_cards[i];
			temp_pair_arr[1] = possible_cards[j];
			card_pair_container = realloc(card_pair_container, sizeof(Card**)*(++number_of_pairs));
			card_pair_container[number_of_pairs-1] = temp_pair_arr;
		}
	}

	*pair_count = number_of_pairs;

	return card_pair_container;
}