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

enum Suit {
	HEART = 1,
	CLUB = 2,
	SPADE = 3,
	DIAMOND = 4
};

typedef struct Card{
	char value; //'2'-'9', 'T', 'J', 'Q', 'K', 'A'
	enum Suit type; 
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

typedef struct GameState{
	Card* added_card;
	Card** player_hand;
	Card** opponent_hand;
	int num_hand_cards;
	Card** unused_cards;
	int num_unused_cards;
	Card** table_cards;
	int num_table_cards;
	Hand* best_player_hand;
	Hand* best_opponent_hand;
	bool player_win;
	bool game_over;
	struct GameState** possible_gamestates;
	int num_sub_gamestates;
	long num_winning_sub_gamestates;
	long num_losing_sub_gamestates;
} GameState;

#endif