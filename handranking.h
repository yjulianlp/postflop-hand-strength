#ifndef HANDRANKING_H
#define HANDRANKING_H
#include "gameinfo.h"
#include <stdbool.h>
#include "cardinfo.h"

int find_pairs(Card** cards, int num_cards);

bool is_flush(Card** cards, int num_cards);

bool is_straight(Card** cards, int num_cards);

bool is_straight_flush(Card** cards, int num_cards);

bool is_royal_flush(Card** cards, int num_cards);

bool is_four_of_a_kind(Card** cards, int num_cards);

bool is_full_house(Card** cards, int num_cards);

bool is_set(Card** cards, int num_cards);

bool is_wheel(Card** hand, int num_cards);

int tiebreak_full_house(Card** hand1, Card** hand2);

int tiebreak_four_of_a_kind(Card** hand1, Card** hand2);

int tiebreak_flush(Card** hand1, Card** hand2, int num_cards);

int tiebreak_straight(Card** hand1, Card** hand2, int num_cards);

int tiebreak_set(Card** hand1, Card** hand2, int num_cards);

int tiebreak_two_pair(Card** hand1, Card** hand2, int num_cards);

int tiebreak_one_pair(Card** hand1, Card** hand2, int num_cards);

int tiebreak_highcard(Card** hand1, Card** hand2, int num_cards);

int tiebreaker(Card** hand1, Card** hand2, int num_cards, enum Hand_Ranking hand_rank);
#endif