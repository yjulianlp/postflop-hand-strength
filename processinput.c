#include "processinput.h"
#include "handranking.h"
#include <assert.h>

char* get_cards(int card_count){
	char* user_input = malloc(sizeof(char)*( card_count*3));
	fgets(user_input, sizeof(char)*(card_count*3)+1, stdin);

	return user_input;	
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
					/*
					printf("added a card: \n");
					print_card_information(temp);
					printf("\n\n+++++++++");*/
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

void free_combinations(Combinations* combo){
	free(combo->combinations);
	free(combo);
}

Combinations* generate_hand_combinations(Card** cards, int num_cards){
	//generate 5-card combinations from 7 cards
	int combination_count = 0;

	int num_possible_combinations = (num_cards == 5) ? 1 : ((num_cards == 6) ? 6 : 21); // 5 choose 5, 6 choose 5, 7 choose 5;
	Card*** possible_combinations = malloc(sizeof(Card**)*num_possible_combinations);

	Combinations* combos = malloc(sizeof(struct Combinations));
	combos->combinations = possible_combinations;
	combos->num_combinations = num_possible_combinations;

	if(num_cards == 5){
		possible_combinations[0] = cards;

		return combos;
	}

	if(num_cards == 6){
		for(int i = 0; i < num_cards; i++){
			possible_combinations[combination_count] = malloc(sizeof(Card*)*5);

			//cards before currently selected card
			memcpy(possible_combinations[combination_count], cards, sizeof(Card*)*i);
			//cards after currently selected card
			memcpy(possible_combinations[combination_count]+i, cards+(i+1), sizeof(Card*)*(num_cards-(i+1)));

			combination_count++;
		}

		return combos;
	}

	//for 7 cards
	for(int i = 0; i < num_cards; i++){
		for(int j = i+1; j < num_cards; j++){

			possible_combinations[combination_count] = malloc(sizeof(Card*)*5);
			int before_cards = i, between_cards = j-(i+1), after_cards = num_cards-(j+1);
			//cards before the first selected card
			memcpy(possible_combinations[combination_count], cards, sizeof(Card*)*before_cards);
			//cards between the two selected cards
			memcpy(possible_combinations[combination_count]+i, cards+(i+1), sizeof(Card*)*between_cards);
			//cards after the second selected card
			memcpy(possible_combinations[combination_count]+before_cards+between_cards, cards+(j+1), sizeof(Card*)*after_cards);
			
			combination_count++;
		}
	}

	return combos;
}
int tiebreaker(Card** hand1, Card** hand2, int num_cards, enum Hand_Ranking hand_rank){
	//return a positive int if hand1 stronger than hand2, negative if hand2 strongest than hand1 (0 if same)
	assert(hand2);
	printf("\npassed assert");

	int difference = 0;
	switch(hand_rank){
	case ROYAL_FLUSH:
		return 0;
	case STRAIGHT_FLUSH:
		int hand1_offset = (is_wheel(hand1, num_cards)) ? 2 : 1;
		int hand2_offset = (is_wheel(hand2, num_cards)) ? 2 : 1;
		difference = compare_cards(hand1[num_cards-hand1_offset], hand2[num_cards-hand2_offset]);
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

Hand* get_best_hand(Card** hand_cards, Card** table_cards, int num_hand_cards, int num_table_cards){
	int pooled_size = num_hand_cards + num_table_cards;
	Card** pooled_cards = malloc(sizeof(Card*)*pooled_size);
	pooled_cards = concat_card_arrays(hand_cards, table_cards, num_hand_cards, num_table_cards);
	qsort(pooled_cards, pooled_size, sizeof(Card*), compare_cards);

	//generate 5-card combinations
	Combinations* combos = generate_hand_combinations(pooled_cards, pooled_size);

	enum Hand_Ranking strongest_combination_ranking = HIGHCARD;
	Card** strongest_combination = NULL;
	for(int i = 0; i < combos->num_combinations; i++){
		enum Hand_Ranking temp_strength = evaluate_hand(combos->combinations[i], 5);
		if(temp_strength > strongest_combination_ranking){
			strongest_combination_ranking = temp_strength;
			strongest_combination = combos->combinations[i];
		}else{
			if(temp_strength == strongest_combination_ranking){
				//tiebreaker
				if(strongest_combination){
					int tiebreak =  tiebreaker(combos->combinations[i], strongest_combination, 5, strongest_combination_ranking);
					if(tiebreak > 0){
						strongest_combination = combos->combinations[i];
						strongest_combination_ranking = temp_strength;
					}
				}else{
					strongest_combination = combos->combinations[i];
					strongest_combination_ranking = temp_strength;
				}
			}
		}
	}

	Hand* best_hand = malloc(sizeof(Hand));
	best_hand->cards = strongest_combination;
	best_hand->num_cards = 5;
	best_hand->hand_rank = strongest_combination_ranking;

	return best_hand;
}

enum Hand_Ranking evaluate_hand(Card** cards, int num_cards){
	//returns a Hand_Ranking for a 5 card hand (cards must be in sorted order)
	print_cards(cards, num_cards);
	bool has_straight = is_straight(cards, num_cards), has_flush = is_flush(cards, num_cards);

	enum Hand_Ranking strongest_combination = HIGHCARD;
	if(has_flush){
		if(has_straight){
			//straight flush or royal flush
			if(cards[0]->value == 'T' && cards[num_cards-1]->value == 'A'){
				strongest_combination = ROYAL_FLUSH;
			}else{
				strongest_combination = STRAIGHT_FLUSH;
			}
		}else{
			//flush
			strongest_combination = FLUSH;
		}
	}else{
		if(is_four_of_a_kind(cards, num_cards)){
			//4 of a kind
			strongest_combination = FOUR_OF_A_KIND;
		}else{
			if(is_full_house(cards, num_cards)){
				strongest_combination = FULL_HOUSE;
			}
			else{
				if(has_straight){
					strongest_combination = STRAIGHT;
				}else{
					if(is_set(cards, num_cards)){
						strongest_combination = SET;
					}else{
						//check for two pair or pair
						int pairs = find_pairs(cards, num_cards);
						switch(pairs){
						case 2:
							strongest_combination = TWO_PAIR;
							break;
						case 1:
							strongest_combination = ONE_PAIR;
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}

	if(is_flush(cards, num_cards)){
		printf("flush! |");
	}
	if(is_straight(cards, num_cards)){
		printf("straight! |");
	}
	if(is_royal_flush(cards, num_cards)){
		printf("royal flush! |");
	}
	if(is_straight_flush(cards, num_cards)){
		printf("straight flush! |");
	}
	if(is_four_of_a_kind(cards, num_cards)){
		printf("four of a kind! |");
	}
	if(is_full_house(cards, num_cards)){
		printf("full house! |");
	}
	if(is_set(cards, num_cards)){
		printf("set! |");
	}
	if(find_pairs(cards, num_cards) == 2){
		printf("two pair! |");
	}
	if(find_pairs(cards, num_cards) == 1){
		printf("one pair! |");
	}

	return strongest_combination;
}

bool is_winning_hand(Card** hand_cards, Card** opponent_hand, Card** table_cards, int hand_card_count, int table_card_count){
	int pooled_size = hand_card_count + table_card_count;
	Card** pooled_cards = malloc(sizeof(Card*)*(pooled_size));
	pooled_cards = concat_card_arrays(hand_cards, table_cards, hand_card_count, table_card_count);
	qsort(pooled_cards, pooled_size, sizeof(Card*), compare_cards);
	(void)hand_cards, (void)opponent_hand;
	return false;

}

int compare_cards(const void* card1, const void* card2){
	//positive if card1 > card2
	int card1_str = value_to_strength((*(Card**)card1)->value);
	int card2_str = value_to_strength((*(Card**)card2)->value);

	return card1_str-card2_str;
}

Card** add_card(Card** hand, Card* card, int hand_size){
	hand = realloc(hand, sizeof(Card*)*(hand_size+1));
	hand[hand_size] = card;

	return hand;
}