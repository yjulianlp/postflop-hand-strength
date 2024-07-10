#ifndef MANAGECARDS_H
#define MANAGECARDS_H
#include "gameinfo.h"

int find_card(Card** sorted_card_container, Card* card, int container_size);

Card** remove_card(Card** cards, Card* card_to_remove, int* num_cards);

Card** swap_cards(Card** cards, int index_of_card1, int index_of_card2);

#endif