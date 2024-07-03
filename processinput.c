#include "processinput.h"

char* get_cards(int card_count){
	char* user_input = malloc(sizeof(char)*( card_count*3));
	fgets(user_input, sizeof(char)*(card_count*3)+1, stdin);

	return user_input;	
}

int value_to_strength(char value){
	if(value >= 51 && value <= 57){
		return value-51;
	}
	else{
		switch(value){
			case 'T':
				return 8;
			case 'J':
				return 9;
			case 'Q':
				return 10;
			case 'K':
				return 11;
			case 'A':
				return 12;
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
		card_container[i]->type = suit_to_int(card_string[(3*i)+1]);
	}
}

void print_debug(Card** cards, int card_count){
	for(int i = 0; i < card_count; i++){
		print_card_information(*(cards+i));
	}
}

void print_card_information(Card* card){
	printf("| %c%c ", card->value, int_to_suit(card->type));
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
	printf("Comparing %c%c with %c%c \n", card_one->value, int_to_suit(card_one->type), card_two->value, int_to_suit(card_two->type));
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
			for(int j = 0; j < SUIT_RANGE; j++){
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

Card** concat_card_arrays(Card** card_arr1, Card** card_arr2, int arr1_len, int arr2_len){
	Card** pooled_cards = malloc(sizeof(Card*)*(arr1_len+arr2_len));
	memcpy(pooled_cards, card_arr1, sizeof(Card*)*arr1_len);
	memcpy((pooled_cards+arr1_len), card_arr2, sizeof(Card*)*arr2_len);
	return pooled_cards;
}

int compare_cards(const void* card1, const void* card2){
	//positive if card1 > card2
	int card1_str = value_to_strength((*(Card**)card1)->value);
	int card2_str = value_to_strength((*(Card**)card2)->value);

	return card1_str-card2_str;
}

bool has_no_duplicate_cards(Card** cards, int num_cards){
	for(int i = 0; i < num_cards-1; i++){
		if(cards[i]->value==cards[i+1]->value){
			#ifdef DEBUG
			printf("\nduplicates found\n");
			#endif
			return false;
		}
	}
	return true;
}

bool is_flush(Card** cards, int num_cards){
	for(int i = 0; i < num_cards-1; i++){
		if(cards[i]->type != cards[i+1]->type){
			return false;
		}
	}
	return true;
}

int compare_card_values(Card* card1, Card* card2){
	int difference = value_to_strength(card1->value) - value_to_strength(card2->value);
	#ifdef DEBUG
	printf("\ndifference: %d\n", difference);
	#endif
	return difference;
}

bool is_straight(Card** cards, int num_cards){
	bool no_duplicates = has_no_duplicate_cards(cards, num_cards);
	int range1 = compare_card_values(cards[num_cards-1], cards[0]);
	#ifdef DEBUG
	printf("\n range: %d with size %d\n", range1, num_cards-1);
	#endif

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

int evaluate_hand(Card** hand_cards, Card** table_cards, int hand_card_count, int table_card_count){
	int pooled_size = hand_card_count + table_card_count;
	Card** pooled_cards = malloc(sizeof(Card*)*(pooled_size));
	pooled_cards = concat_card_arrays(hand_cards, table_cards, hand_card_count, table_card_count);
	qsort(pooled_cards, pooled_size, sizeof(Card*), compare_cards);

	if(is_flush(pooled_cards, pooled_size)){
		printf("flush!");
	}
	if(is_straight(pooled_cards, pooled_size)){
		printf("straight!");
	}
	//generate combinations of 5-cards
/*
	//pass a sorted array
	is_royal_flush(pooled_cards, pooled_size);
	is_straight_flush(pooled_cards, pooled_size);
	is_four_of_a_kind(pooled_cards, pooled_size);
	is_full_house(pooled_cards, pooled_size);
	is_flush(pooled_cards, pooled_size);
	is_straight(pooled_cards, pooled_size);
	is_set(pooled_cards, pooled_size);
	is_two_pair(pooled_cards, pooled_size);
	is_one_pair(pooled_cards, pooled_size);
*/
	return -1; //placeholder
}

bool is_winning_hand(Card** hand_cards, Card** opponent_hand, Card** table_cards, int hand_card_count, int table_card_count){
	int pooled_size = hand_card_count + table_card_count;
	Card** pooled_cards = malloc(sizeof(Card*)*(pooled_size));
	pooled_cards = concat_card_arrays(hand_cards, table_cards, hand_card_count, table_card_count);
	qsort(pooled_cards, pooled_size, sizeof(Card*), compare_cards);
	
	(void)hand_cards, (void)opponent_hand;
	return false;

}