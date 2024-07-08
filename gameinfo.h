#ifndef GAMEINFO_H
#define GAMEINFO_H

#define DECK_SIZE 52
#define CARD_VALUE_RANGE 13
#define SUIT_RANGE 4

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum Hand_Ranking {
	HIGHCARD,
	ONE_PAIR,
	TWO_PAIR,
	SET,
	STRAIGHT,
	FLUSH,
	FULL_HOUSE,
	FOUR_OF_A_KIND,
	STRAIGHT_FLUSH,
	ROYAL_FLUSH
};

typedef struct Card{
	char value; //1-10 (number cards), 'J', 'Q', 'K', 'A'
	int type; //hearts: 1, clubs: 2, spades: 3, diamonds: 4
} Card;

typedef struct Combinations{
	Card*** combinations;
	int num_combinations;
} Combinations;

typedef struct Hand{
	Card** cards;
	int num_cards;
	enum Hand_Ranking hand_rank;
} Hand;
#endif