#ifndef PROCESSINPUT_H
#define PROCESSINPUT_H
#include "gameinfo.h"

char* get_cards(int hand_card_count);

void print_hand_cards(int hand_card_count, char* hand_cards);

void process_cards(Card** card_container, int number_of_cards, char* card_string);

void print_debug(Card** hand_cards, int hand_card_number);

Card** setup_unused_cards(Card** unused_card_container, int cards_remaining, Card** hand_cards, Card** table_cards, int hand_size, int table_size);

bool is_in_use(Card* card, Card** cards_to_check, int num_cards_to_check);

void free_card_mem(Card** card_container, int num_cards);

Card*** generate_possible_pairs(Card** possible_cards, int num_possible_cards, int* pair_count);

bool is_winning_hand(Card** hand_cards, Card** opponent_hand, Card** table_cards, int hand_card_count, int table_card_count);

enum Hand_Ranking evaluate_hand(Card** cards, int num_cards);

Card** concat_card_arrays(Card** card_arr1, Card** card_arr2, int arr1_len, int arr2_len);

Combinations* generate_hand_combinations(Card** cards, int num_cards);

void free_combinations(Combinations* combo);

Hand* get_best_hand(Card** hand_cards, Card** table_cards, int num_hand_cards, int num_table_cards);

int compare_cards(const void* card1, const void* card2);

Card** add_card(Card** hand, Card* card, int hand_size);

Card* get_card(void);

#endif