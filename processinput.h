#ifndef PROCESSINPUT_H

#define PROCESSINPUT_H
#define DECK_SIZE 52
#define CARD_VALUE_RANGE 13
#define SUIT_RANGE 4

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Card{
	char value; //1-10 (number cards), 'J', 'Q', 'K', 'A'
	int type; //hearts: 1, clubs: 2, spades: 3, diamonds: 4
} Card;

char* get_cards(int hand_card_count);

void print_hand_cards(int hand_card_count, char* hand_cards);

void process_cards(Card** card_container, int number_of_cards, char* card_string);

void print_card_information(Card* card);

int suit_to_int(char suit);

char int_to_suit(int suit);

void print_debug(Card** hand_cards, int hand_card_number);

bool is_same_card(Card* card_one, Card* card_two);

void setup_unused_cards(Card** unused_card_container, int cards_remaining, Card** hand_cards, Card** table_cards, int hand_size, int table_size);

void print_cards(Card** card_container, int num_cards);

bool is_in_use(Card* card, Card** cards_to_check, int num_cards_to_check);

void free_card_mem(Card** card_container, int num_cards);

Card*** generate_possible_pairs(Card*** card_pair_container, Card** possible_cards, int num_possible_cards, int* pair_count);

bool is_winning_hand(Card** hand_cards, Card** opponent_hand, Card** table_cards, int hand_card_count, int table_card_count);

int evaluate_hand(Card** hand_cards, Card** table_cards, int hand_card_count, int table_card_count);

Card** concat_card_arrays(Card** card_arr1, Card** card_arr2, int arr1_len, int arr2_len);

int value_to_strength(char value);

int compare_cards(const void* card1, const void* card2);

bool is_flush(Card** cards, int num_cards);

bool is_straight(Card** cards, int num_cards);

bool has_no_duplicate_cards(Card** cards, int num_cards);

int get_card_value_difference(Card* card1, Card* card2);

bool has_same_value(Card* card1, Card* card2);

bool is_straight_flush(Card** cards, int num_cards);

bool is_royal_flush(Card** cards, int num_cards);

bool is_four_of_a_kind(Card** cards, int num_cards);

bool is_full_house(Card** cards, int num_cards);

bool is_set(Card** cards, int num_cards);

bool is_two_pair(Card** cards, int num_cards);

bool is_one_pair(Card** cards, int num_cards);

#endif