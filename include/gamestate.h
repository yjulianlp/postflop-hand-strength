#ifndef GAMESTATES_H
#define GAMESTATES_H
#include "gameinfo.h"

void initialize_gamestate(GameState* gamestate, Card* added_card, Card** player_hand, Card** opponent_hand, int hand_card_count, Card** unused_cards, int unused_card_count, Card** table_cards, int table_card_count, bool is_gameover);

void copy_gamestate(GameState* gamestate1, GameState* gamestate2);

void print_gamestate_information(GameState* gamestate);

void free_gamestate(GameState* gamestate);

void generate_sub_gamestates(GameState* root_gamestate);

#endif