#include "../include/processinput.h"
#include "../include/managecards.h"
#include "../include/cardinfo.h"
#include "../include/gamestate.h"

int main(void){

	//get card information
	printf("Enter the cards in your hand: (e.g. As-Jc)\n");
	char *hand_card_input = get_cards(NUM_HAND_CARDS);

	//store card information
	Card** hand_cards = process_cards(NUM_HAND_CARDS, hand_card_input);
	free(hand_card_input);
	qsort(hand_cards, NUM_HAND_CARDS, sizeof(Card*), compare_cards);

	//get opponent hand
	printf("Enter the cards of your opponent:");
	char *opponent_hand_input = get_cards(NUM_HAND_CARDS);
	Card** opponent_hand = process_cards(NUM_HAND_CARDS, opponent_hand_input);
	free(opponent_hand_input);

	printf("Enter the cards on the table: (e.g. Kc-Th-3s)");

	char *table_card_input = get_cards(NUM_TABLE_CARDS);
	Card** table_cards = process_cards(NUM_TABLE_CARDS, table_card_input);
	free(table_card_input);

	//cards not in hand and not on table
	int remaining_cards = DECK_SIZE-NUM_HAND_CARDS-NUM_TABLE_CARDS;
	Card** unused_cards = malloc(sizeof(Card*)*remaining_cards); 

	unused_cards = setup_unused_cards(unused_cards, remaining_cards, hand_cards, table_cards, NUM_HAND_CARDS, NUM_TABLE_CARDS);
	qsort(unused_cards, remaining_cards, sizeof(Card*), compare_cards);

	int opponent_pair_count = 0;
	Card*** possible_opponent_pairs = generate_possible_pairs(unused_cards, remaining_cards, &opponent_pair_count);

	Card** cards = concat_card_arrays(hand_cards, table_cards, NUM_HAND_CARDS, NUM_TABLE_CARDS);
	qsort(cards, COMBINATION_SIZE, sizeof(Card*), compare_cards);
	enum Hand_Ranking test_eval = evaluate_hand(cards, COMBINATION_SIZE);
	printf("hand evaluated as value %d \n", test_eval);

	
	Hand* best = get_best_hand(hand_cards, table_cards, NUM_HAND_CARDS, NUM_TABLE_CARDS);
	/*
	printf("\nbest hand value is %d \n", best->hand_rank);
	printf("for the combination: \n");
	print_cards(best->cards, best->num_cards);
	printf("\n");
*/
	qsort(possible_opponent_pairs, opponent_pair_count, sizeof(Card**), compare_pairs);
	for(int i = 0; i < opponent_pair_count; i++){
		print_cards(possible_opponent_pairs[i], NUM_HAND_CARDS);
	}


	//find opponent pair position
	qsort(opponent_hand, NUM_HAND_CARDS, sizeof(Card*), compare_cards);
	int opponent_pair_position = find_pair(possible_opponent_pairs, opponent_hand, opponent_pair_count);
	printf("\n");
	//testing gamestates
	GameState* flop_gamestate = malloc(sizeof(GameState));
	initialize_gamestate(flop_gamestate, NULL, hand_cards, possible_opponent_pairs[opponent_pair_position], NUM_HAND_CARDS, unused_cards, remaining_cards, table_cards, NUM_TABLE_CARDS, false);
	//print_gamestate_information(flop_gamestate);
	generate_sub_gamestates(flop_gamestate);
	update_total_outcomes(flop_gamestate);
	print_gamestate_tree(flop_gamestate);

	free_gamestate(flop_gamestate);

	free_card_mem(hand_cards, NUM_HAND_CARDS);
	free_card_mem(opponent_hand, NUM_HAND_CARDS);
	free_card_mem(table_cards, NUM_TABLE_CARDS);
	free_card_mem(unused_cards, remaining_cards);
	for(int i = 0; i < opponent_pair_count; i++){
		free(possible_opponent_pairs[i]);
	}
	free(possible_opponent_pairs);
	free(cards);
	free_hand_struct(best);
	return 0;
}
