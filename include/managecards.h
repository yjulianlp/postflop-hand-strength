#ifndef MANAGECARDS_H
#define MANAGECARDS_H
#include "gameinfo.h"

int find_card(Card** sorted_card_container, Card* card, int container_size);

Card** remove_card(Card** cards, Card* card_to_remove, int* num_cards);

Card** swap_cards(Card** cards, int index_of_card1, int index_of_card2);

Card*** remove_pairs_with_card(Card*** card_pair_array, Card* card, int* num_pairs);

Card*** swap_pairs(Card*** cards, int index_of_pair1, int index_of_pair2);

Card** add_card(Card** hand, Card* card, int* hand_size);

void free_pair_mem(Card** pair);

void free_hand_struct(Hand* hand);

#endif