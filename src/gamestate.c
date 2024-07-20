#include "../include/gamestate.h"
#include "../include/processinput.h"
#include "../include/managecards.h"
#include "../include/cardinfo.h"

void initialize_gamestate(GameState* gamestate, Card* added_card, Card** player_hand, Card** opponent_hand, int hand_card_count, Card** unused_cards, int unused_card_count, Card** table_cards, int table_card_count, bool is_gameover){
	gamestate->game_over = is_gameover;
	gamestate->added_card = added_card;
	gamestate->player_hand = player_hand;
	gamestate->opponent_hand = opponent_hand;
	gamestate->num_hand_cards = hand_card_count;
	gamestate->num_unused_cards = unused_card_count;
	gamestate->unused_cards = malloc(sizeof(Card*)*gamestate->num_unused_cards);
	memcpy(gamestate->unused_cards, unused_cards, sizeof(Card*)*gamestate->num_unused_cards);
	gamestate->num_table_cards = table_card_count;
	gamestate->table_cards = malloc(sizeof(Card*)*gamestate->num_table_cards);
	memcpy(gamestate->table_cards, table_cards, sizeof(Card*)*gamestate->num_table_cards);
	gamestate->best_player_hand = get_best_hand(gamestate->player_hand, gamestate->table_cards, gamestate->num_hand_cards, gamestate->num_table_cards);
	gamestate->best_opponent_hand = get_best_hand(gamestate->opponent_hand, gamestate->table_cards, gamestate->num_hand_cards, gamestate->num_table_cards);
	gamestate->player_win = is_winning_hand(gamestate->best_player_hand, gamestate->best_opponent_hand);
	gamestate->num_sub_gamestates = gamestate->num_unused_cards;
	gamestate->unused_cards = remove_card(gamestate->unused_cards, gamestate->opponent_hand[0], &(gamestate->num_unused_cards), false);
	gamestate->unused_cards = remove_card(gamestate->unused_cards, gamestate->opponent_hand[1], &(gamestate->num_unused_cards), false);
	gamestate->possible_gamestates = NULL;
}
void print_gamestate_information(GameState* gamestate){
	printf("------PRINTING GAMESTATE-----\n");
	if(gamestate->added_card){
		printf("most recent card added was: ");
		print_card_information(gamestate->added_card);
	}
	printf("\nplayer hand is:\n");
	print_cards(gamestate->player_hand, gamestate->num_hand_cards);
	printf("\nopponent hand is:\n");
	print_cards(gamestate->opponent_hand, gamestate->num_hand_cards);
	printf("The %d currently unused cards are:\n", gamestate->num_unused_cards);
	print_cards(gamestate->unused_cards, gamestate->num_unused_cards);
	printf("\ncards on the table are:\n");
	print_cards(gamestate->table_cards, gamestate->num_table_cards);
	printf("\n-------\n");
	printf("\nbest player hand value is %d \n", gamestate->best_player_hand->hand_rank);
	printf("for the combination: \n");
	print_cards(gamestate->best_player_hand->cards, 5);
	printf("\n");
	printf("\nbest opponent hand value is %d \n", gamestate->best_opponent_hand->hand_rank);
	printf("for the combination: \n");
	print_cards(gamestate->best_opponent_hand->cards, 5);
	printf("\n");
	printf("The player is currently %s.\n", (gamestate->player_win ? "winning" : "losing"));
}

void copy_gamestate(GameState* gamestate1, GameState* gamestate2){
	gamestate1->added_card = gamestate2->added_card;
	gamestate1->player_hand = gamestate2->player_hand;
	gamestate1->opponent_hand = gamestate2->opponent_hand;
	gamestate1->num_hand_cards = gamestate2->num_hand_cards;
	gamestate1->unused_cards = malloc(sizeof(Card*)*gamestate2->num_unused_cards);
	memcpy(gamestate1->unused_cards, gamestate2->unused_cards, sizeof(Card*)*gamestate2->num_unused_cards);
	gamestate1->num_unused_cards = gamestate2->num_unused_cards;

	gamestate1->table_cards = malloc(sizeof(Card*)*gamestate2->num_table_cards);
	memcpy(gamestate1->table_cards, gamestate2->table_cards, sizeof(Card*)*gamestate2->num_table_cards);
	gamestate1->num_table_cards = gamestate2->num_table_cards;

	gamestate1->best_player_hand = gamestate2->best_player_hand;
	gamestate1->best_opponent_hand = gamestate2->best_opponent_hand;
	gamestate1->player_win = gamestate2->player_win;
	gamestate1->game_over = gamestate2->game_over;
	gamestate1->possible_gamestates = gamestate2->possible_gamestates;
	gamestate1->num_sub_gamestates = gamestate2->num_sub_gamestates;
}

void free_gamestate(GameState* gamestate){
	if(gamestate){
		for(int i = 0; i < gamestate->num_sub_gamestates; i++){
			free_gamestate(gamestate->possible_gamestates[i]);
		}
		free(gamestate->possible_gamestates);
		free(gamestate->unused_cards);
		free(gamestate->table_cards);
		free_hand_struct(gamestate->best_player_hand);
		free_hand_struct(gamestate->best_opponent_hand);
		free(gamestate);
	}
}

void generate_sub_gamestates(GameState* root_gamestate){
	if(root_gamestate){
		if((root_gamestate->num_hand_cards + root_gamestate->num_table_cards)==7){
			root_gamestate->possible_gamestates = NULL;
			root_gamestate->game_over = true;
			root_gamestate->num_sub_gamestates = 0;
			return;
		}else{
			root_gamestate->possible_gamestates = malloc(sizeof(GameState*)*root_gamestate->num_unused_cards);
			root_gamestate->num_sub_gamestates = root_gamestate->num_unused_cards;
			for(int i = 0; i < root_gamestate->num_unused_cards; i++){
				#ifdef DEBUG
				printf("adding card: ");
				print_card_information(root_gamestate->unused_cards[i]);
				printf("\n");
				#endif

				root_gamestate->possible_gamestates[i] = malloc(sizeof(GameState));
				copy_gamestate(root_gamestate->possible_gamestates[i], root_gamestate);

				#ifdef DEBUG
				printf("\n\nprevious unused cards at %p with new unused cards at %p\n\n", root_gamestate->unused_cards, root_gamestate->possible_gamestates[i]->unused_cards);
				printf("old unused cards: \n");
				print_cards(root_gamestate->unused_cards, root_gamestate->num_unused_cards);
				#endif

				root_gamestate->possible_gamestates[i]->added_card = root_gamestate->unused_cards[i];
				root_gamestate->possible_gamestates[i]->unused_cards = remove_card(root_gamestate->possible_gamestates[i]->unused_cards, root_gamestate->unused_cards[i], &(root_gamestate->possible_gamestates[i]->num_unused_cards), false);
				root_gamestate->possible_gamestates[i]->table_cards = add_card(root_gamestate->possible_gamestates[i]->table_cards, root_gamestate->possible_gamestates[i]->added_card, &(root_gamestate->possible_gamestates[i]->num_table_cards));

				#ifdef DEBUG
				printf("new unused cards: \n");
				print_cards(root_gamestate->possible_gamestates[i]->unused_cards, root_gamestate->possible_gamestates[i]->num_unused_cards);
				printf("\n\n");
				#endif

				root_gamestate->possible_gamestates[i]->best_player_hand = get_best_hand(root_gamestate->possible_gamestates[i]->player_hand, root_gamestate->possible_gamestates[i]->table_cards, root_gamestate->possible_gamestates[i]->num_hand_cards, root_gamestate->possible_gamestates[i]->num_table_cards);
				root_gamestate->possible_gamestates[i]->best_opponent_hand = get_best_hand(root_gamestate->possible_gamestates[i]->opponent_hand, root_gamestate->possible_gamestates[i]->table_cards, root_gamestate->possible_gamestates[i]->num_hand_cards, root_gamestate->possible_gamestates[i]->num_table_cards);
				root_gamestate->possible_gamestates[i]->player_win = (is_winning_hand(root_gamestate->possible_gamestates[i]->best_player_hand, root_gamestate->possible_gamestates[i]->best_opponent_hand) ? true : false);
				print_gamestate_information(root_gamestate->possible_gamestates[i]);
				generate_sub_gamestates(root_gamestate->possible_gamestates[i]);
				//break;
			}
		}
	}
}