#include "processinput.h"

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
	Card* unused_cards[remaining_cards]; 

	setup_unused_cards(unused_cards, remaining_cards, hand_cards, table_cards, hand_card_number, table_card_count);

	printf("Currently unused cards: \n\n\n++++++++++++++++++++++++++++++++++");
	print_cards(unused_cards, remaining_cards);


	int opponent_pair_count = 0;
	Card*** possible_opponent_pairs = malloc(sizeof(Card**)*opponent_pair_count);
	possible_opponent_pairs = generate_possible_pairs(possible_opponent_pairs, unused_cards, remaining_cards, &opponent_pair_count);

	/*
	for(int i = 0; i < opponent_pair_count; i++){
		print_cards(possible_opponent_pairs[i], 2);
	}*/

	enum Hand test_eval = evaluate_hand(hand_cards, table_cards, 2, 3);
	printf("hand evaluated as value %d \n", test_eval);
	//is_winning_hand(hand_cards, hand_cards, table_cards, 2, 3);

	free_card_mem(hand_cards, hand_card_number);
	free_card_mem(table_cards, table_card_count);
	free_card_mem(unused_cards, remaining_cards);

	return 0;
}
