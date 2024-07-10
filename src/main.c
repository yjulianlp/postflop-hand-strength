#include "../include/processinput.h"
#include "../include/managecards.h"
#include "../include/cardinfo.h"

int main(int argc, char* argv[]){
	if(argc<2){
		return 1;
	}

	//get card information
	int hand_card_number = atoi(argv[1]);
	printf("Enter the cards in your hand: (e.g. As-Jc)\n");
	char *hand_card_input = get_cards(hand_card_number);
	Card* hand_cards[hand_card_number];

	//store card information
	process_cards(hand_cards, hand_card_number, hand_card_input);

	printf("Enter the cards on the table: (e.g. Kc-Th-3s)");
	fflush(stdout);

	int table_card_count = 3;
	char *table_card_input = get_cards(table_card_count);
	Card** table_cards = malloc(sizeof(Card*)*table_card_count);
	process_cards(table_cards, table_card_count, table_card_input);

	//cards not in hand and not on table
	int remaining_cards = DECK_SIZE-hand_card_number-table_card_count;
	Card** unused_cards = malloc(sizeof(Card*)*remaining_cards); 

	unused_cards = setup_unused_cards(unused_cards, remaining_cards, hand_cards, table_cards, hand_card_number, table_card_count);
	qsort(unused_cards, remaining_cards, sizeof(Card*), compare_cards);

	int opponent_pair_count = 0;
	printf("allocating memory\n");
	Card*** possible_opponent_pairs = malloc(sizeof(Card**)*(opponent_pair_count));
	printf("running");
	possible_opponent_pairs = generate_possible_pairs(unused_cards, remaining_cards, &opponent_pair_count);

	printf("pair count: %d\n", opponent_pair_count);
	for(int i = 0; i < (opponent_pair_count); i++){
		print_cards(possible_opponent_pairs[i], 2);
	}
	Card** cards = concat_card_arrays(hand_cards, table_cards, 2, 3);
	qsort(cards, 5, sizeof(Card*), compare_cards);
	enum Hand_Ranking test_eval = evaluate_hand(cards, 5);
	printf("hand evaluated as value %d \n", test_eval);
	//is_winning_hand(hand_cards, hand_cards, table_cards, 2, 3);

	Hand* best = get_best_hand(hand_cards, table_cards, 2, 3);

	printf("\n\ntesting multiple combinations\n\n");
	printf("Enter the turn card: ");
	Card* temp_card = get_card();
	table_cards = add_card(table_cards, temp_card, table_card_count);
	best = get_best_hand(hand_cards, table_cards, 2, 4);


	printf("\nbest hand value is %d \n", best->hand_rank);
	printf("for the combination: \n");
	print_cards(best->cards, best->num_cards);
	printf("\n");

	free_card_mem(hand_cards, hand_card_number);
	free_card_mem(table_cards, table_card_count);
	free_card_mem(unused_cards, remaining_cards);

	return 0;
}
