#include "../include/processinput.h"
#include "../include/handranking.h"
#include "../include/cardinfo.h"
#include "../include/gameinfo.h"
#include <string.h>

char* get_cards(int card_count){
	char* user_input = malloc(sizeof(char)*(card_count*3)+1);
	fgets(user_input, sizeof(char)*(card_count*3)+2, stdin);

	return user_input;	
}

Card* get_card(void){
	char* user_input = malloc(sizeof(char)*4);
	fgets(user_input, 4, stdin);
	Card* temp_card = malloc(sizeof(Card));
	temp_card->value = user_input[0];
	temp_card->type = suit_to_int(user_input[1]);
	free(user_input);

	return temp_card;
}

Card** process_cards(int num_cards, char* card_string){
	Card** card_container = malloc(sizeof(Card*)*num_cards);
	for(int i = 0; i < num_cards; i++){
		*(card_container+i) = malloc(sizeof(Card));
		card_container[i]->value = card_string[3*i];
		card_container[i]->type = suit_to_int(card_string[(3*i)+1]);
	}

	return card_container;
}

void print_debug(Card** cards, int card_count){
	for(int i = 0; i < card_count; i++){
		print_card_information(*(cards+i));
	}
}

bool is_in_use(Card* card, Card** cards_to_check, int num_cards_to_check){
	for(int i = 0; i < num_cards_to_check; i++){
		if(is_same_card(card, cards_to_check[i])){
			return true;
		}
	}
	return false;
}

Card** setup_unused_cards(Card** unused_card_container, int cards_remaining, Card** hand_cards, Card** table_cards, int hand_size, int table_size){
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
					break;
				}else{
					free(temp);
				}
			}
			if(added){
				break;
			}
		}
	}

	return unused_card_container;
}

void free_card_mem(Card** card_container, int num_cards){
	for(int i = 0; i < num_cards; i++){
		free(card_container[i]);
	}

	free(card_container);
}

Card*** generate_possible_pairs(Card** possible_cards, int num_possible_cards, int* pair_count){
	printf("generating pairs\n");
	Card*** temp_container = NULL;
	for(int i = 0; i < num_possible_cards; i++){
		for(int j = i+1; j < num_possible_cards; j++){
			Card** temp_pair_arr = malloc(sizeof(Card*)*2);
			temp_pair_arr[0] = possible_cards[i];
			temp_pair_arr[1] = possible_cards[j];
			temp_container = realloc(temp_container, sizeof(Card**)*(++(*pair_count)));
			temp_container[(*pair_count)-1] = temp_pair_arr;
		}
	}

	return temp_container;
}


Card** concat_card_arrays(Card** card_arr1, Card** card_arr2, int arr1_len, int arr2_len){
	Card** pooled_cards = malloc(sizeof(Card*)*(arr1_len+arr2_len));
	memcpy(pooled_cards, card_arr1, sizeof(Card*)*arr1_len);
	memcpy((pooled_cards+arr1_len), card_arr2, sizeof(Card*)*arr2_len);
	return pooled_cards;
}

void free_combo_struct(Combinations* combo, int num_combinations){
	for(int i = 0; i < num_combinations; i++){
		free(combo->combinations[i]);
	}
	free(combo->combinations);
	free(combo);
}

Combinations* generate_hand_combinations(Card** cards, int num_cards){
	//generate 5-card combinations from 7 cards
	int combination_count = 0;

	int num_possible_combinations = (num_cards == 5) ? 1 : ((num_cards == 6) ? 6 : 21); // 5 choose 5, 6 choose 5, 7 choose 5;

	Combinations* combos = malloc(sizeof(struct Combinations));
	combos->combinations = malloc(sizeof(Card**)*num_possible_combinations);
	combos->num_combinations = num_possible_combinations;

	if(num_cards == 5){
		combos->combinations[0] = malloc(sizeof(Card*)*COMBINATION_SIZE);
		memcpy(combos->combinations[0], cards, sizeof(Card*)*COMBINATION_SIZE);
		//possible_combinations[0] = cards;

		return combos;
	}

	if(num_cards == 6){
		for(int i = 0; i < num_cards; i++){
			combos->combinations[combination_count] = malloc(sizeof(Card*)*COMBINATION_SIZE);

			//cards before currently selected card
			memcpy(combos->combinations[combination_count], cards, sizeof(Card*)*i);
			//cards after currently selected card
			memcpy(combos->combinations[combination_count]+i, cards+(i+1), sizeof(Card*)*(num_cards-(i+1)));

			combination_count++;
		}

		return combos;
	}

	//for 7 cards
	for(int i = 0; i < num_cards; i++){
		for(int j = i+1; j < num_cards; j++){

			combos->combinations[combination_count] = malloc(sizeof(Card*)*COMBINATION_SIZE);
			int before_cards = i, between_cards = j-(i+1), after_cards = num_cards-(j+1);
			//cards before the first selected card
			memcpy(combos->combinations[combination_count], cards, sizeof(Card*)*before_cards);
			//cards between the two selected cards
			memcpy(combos->combinations[combination_count]+i, cards+(i+1), sizeof(Card*)*between_cards);
			//cards after the second selected card
			memcpy(combos->combinations[combination_count]+before_cards+between_cards, cards+(j+1), sizeof(Card*)*after_cards);
			
			combination_count++;
		}
	}

	return combos;
}

Hand* get_best_hand(Card** hand_cards, Card** table_cards, int num_hand_cards, int num_table_cards){
	int pooled_size = num_hand_cards + num_table_cards;
	Card** pooled_cards = concat_card_arrays(hand_cards, table_cards, num_hand_cards, num_table_cards);
	qsort(pooled_cards, pooled_size, sizeof(Card*), compare_cards);

	#ifdef DEBUG
	printf("generating combinations for the cards: \n");
	print_cards(pooled_cards, pooled_size);
	printf("\n");
	#endif

	//generate 5-card combinations
	Combinations* combos = generate_hand_combinations(pooled_cards, pooled_size);
	free(pooled_cards);
	enum Hand_Ranking strongest_combination_ranking = HIGHCARD;
	Card** strongest_combination = NULL;

	for(int i = 0; i < combos->num_combinations; i++){
		enum Hand_Ranking temp_strength = evaluate_hand(combos->combinations[i], COMBINATION_SIZE);
		if(temp_strength > strongest_combination_ranking){
			strongest_combination_ranking = temp_strength;
			strongest_combination = combos->combinations[i];
		}else{
			if(temp_strength == strongest_combination_ranking){
				//tiebreaker
				if(strongest_combination){
					int tiebreak =  tiebreaker(combos->combinations[i], strongest_combination, COMBINATION_SIZE, strongest_combination_ranking);
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
	best_hand->cards = malloc(sizeof(Card*)*COMBINATION_SIZE);
	memcpy(best_hand->cards, strongest_combination, sizeof(Card*)*COMBINATION_SIZE);
	best_hand->num_cards = COMBINATION_SIZE;
	best_hand->hand_rank = strongest_combination_ranking;
	free_combo_struct(combos, combos->num_combinations);
	return best_hand;
}

enum Hand_Ranking evaluate_hand(Card** cards, int num_cards){
	//returns a Hand_Ranking for a 5 card hand (cards must be in sorted order)
	#ifdef DEBUG
	printf("evaluating the hand: ");
	print_cards(cards, num_cards);
	#endif

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

	#ifdef DEBUG
	switch(strongest_combination){
	case ROYAL_FLUSH:
		printf("royal flush!\n");
		break;
	case STRAIGHT_FLUSH:
		printf("straight flush!\n");
		break;
	case FOUR_OF_A_KIND:
		printf("four of a kind!\n");
		break;
	case FULL_HOUSE:
		printf("full house!\n");
		break;
	case FLUSH:
		printf("flush!\n");
		break;
	case STRAIGHT:
		printf("straight!\n");
		break;
	case SET:
		printf("set!\n");
		break;
	case TWO_PAIR:
		printf("two pair!\n");
		break;
	case ONE_PAIR:
		printf("one pair!\n");
		break;
	default:
		printf("high card!\n");
	}
	#endif

	return strongest_combination;
}

bool is_winning_hand(Hand* hand1, Hand* hand2){
	int difference;
	if(hand1->hand_rank == hand2->hand_rank){
		#ifdef DEBUG
		printf("tiebreaking\n");
		#endif
		difference = tiebreaker(hand1->cards, hand2->cards, hand1->num_cards, hand1->hand_rank);
		#ifdef DEBUG
		printf("difference in hand value of %d\n", difference);
		#endif
		return (difference > 0);
	}
	difference = hand1->hand_rank - hand2->hand_rank;
	#ifdef DEBUG
	printf("\ndifference in hand value of %d\n", difference);
	#endif
	return (difference > 0);
}

int compare_cards(const void* card1, const void* card2){
	//positive if card1 > card2
	int card1_str = value_to_strength((*(Card**)card1)->value);
	int card2_str = value_to_strength((*(Card**)card2)->value);
	if(card1_str == card2_str){ //if same value
		return suit_difference((*(Card**)card1), (*(Card**)card2));
	}

	return card1_str-card2_str;
}

int compare_pairs(const void* pair1, const void* pair2){
	Card** hand1 = (*(Card***)pair1);
	Card** hand2 = (*(Card***)pair2);
	int card_diff = card_difference(hand1[0], hand2[0]);
	if(card_diff == 0){
		card_diff = card_difference(hand1[1], hand2[1]);
	}

	return card_diff;
}