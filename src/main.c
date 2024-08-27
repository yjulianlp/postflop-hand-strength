#include "../include/processinput.h"
#include "../include/managecards.h"
#include "../include/cardinfo.h"
#include "../include/gamestate.h"

int main(int argc, char* argv[]){
	int hand_card_number;
	if(argc<2){
		hand_card_number = 2;
	}else{
		hand_card_number = atoi(argv[1]);
	}

	//get card information
	printf("Enter the cards in your hand: (e.g. As-Jc)\n");
	char *hand_card_input = get_cards(hand_card_number);

	//store card information
	Card** hand_cards = process_cards(hand_card_number, hand_card_input);
	free(hand_card_input);

	printf("Enter the cards on the table: (e.g. Kc-Th-3s)");

	int table_card_count = 3;
	char *table_card_input = get_cards(table_card_count);
	Card** table_cards = process_cards(table_card_count, table_card_input);
	free(table_card_input);

	//cards not in hand and not on table
	int remaining_cards = DECK_SIZE-hand_card_number-table_card_count;
	Card** unused_cards = malloc(sizeof(Card*)*remaining_cards); 

	unused_cards = setup_unused_cards(unused_cards, remaining_cards, hand_cards, table_cards, hand_card_number, table_card_count);
	qsort(unused_cards, remaining_cards, sizeof(Card*), compare_cards);

	int opponent_pair_count = 0;
	Card*** possible_opponent_pairs = NULL;
	possible_opponent_pairs = generate_possible_pairs(unused_cards, remaining_cards, &opponent_pair_count);

	Card** cards = concat_card_arrays(hand_cards, table_cards, 2, 3);
	qsort(cards, 5, sizeof(Card*), compare_cards);
	enum Hand_Ranking test_eval = evaluate_hand(cards, 5);
	printf("hand evaluated as value %d \n", test_eval);

	Hand* best = get_best_hand(hand_cards, table_cards, 2, 3);
	printf("\nbest hand value is %d \n", best->hand_rank);
	printf("for the combination: \n");
	print_cards(best->cards, best->num_cards);
	printf("\n");

	//testing gamestates
	GameState* flop_gamestate = malloc(sizeof(GameState));
	initialize_gamestate(flop_gamestate, NULL, hand_cards, possible_opponent_pairs[0], hand_card_number, unused_cards, remaining_cards, table_cards, table_card_count, false);
	//print_gamestate_information(flop_gamestate);
	generate_sub_gamestates(flop_gamestate);
	print_gamestate_tree(flop_gamestate);

	free_gamestate(flop_gamestate);

	free_card_mem(hand_cards, hand_card_number);
	free_card_mem(table_cards, table_card_count);
	free_card_mem(unused_cards, remaining_cards);
	for(int i = 0; i < opponent_pair_count; i++){
		free(possible_opponent_pairs[i]);
	}
	free(possible_opponent_pairs);
	free(cards);
	free_hand_struct(best);
	return 0;
}
