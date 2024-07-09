#ifndef CARDINFO_H
#define CARDINFO_H
#include "gameinfo.h"

bool has_same_value(Card* card1, Card* card2);

int value_to_strength(char value);

int suit_to_int(char suit);

char int_to_suit(int suit);

int value_difference(Card* card1, Card* card2);

int suit_difference(Card* card1, Card* card2);

bool has_no_duplicate_cards(Card** cards, int num_cards);

bool is_same_card(Card* card_one, Card* card_two);

void print_card_information(Card* card);

void print_cards(Card** card_container, int num_cards);

#endif