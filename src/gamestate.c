#include "../include/gamestate.h"
#include "../include/processinput.h"
#include "../include/managecards.h"
#include "../include/cardinfo.h"
#define TESTING_GAMESTATE_COUNT 2

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
	gamestate->num_sub_gamestates = gamestate->num_unused_cards-2;
	gamestate->unused_cards = remove_card(gamestate->unused_cards, gamestate->opponent_hand[0], &(gamestate->num_unused_cards), false);
	gamestate->unused_cards = remove_card(gamestate->unused_cards, gamestate->opponent_hand[1], &(gamestate->num_unused_cards), false);
	gamestate->possible_gamestates = NULL;
	gamestate->num_winning_sub_gamestates = 0;
	gamestate->num_losing_sub_gamestates = 0;
	gamestate->total_winning_sub_gamestates = 0;
	gamestate->total_losing_sub_gamestates = 0;
}
void print_gamestate_information(GameState* gamestate){
	printf("\n------PRINTING GAMESTATE-----\n");
	if(gamestate->added_card){
		printf("most recent card added was: ");
		print_card_information(gamestate->added_card);
		printf("|");
	}
	printf("\n(Player Hand): ");
	print_cards(gamestate->player_hand, gamestate->num_hand_cards);
	printf(" VS (Opponent Hand): ");
	print_cards(gamestate->opponent_hand, gamestate->num_hand_cards);
	#ifdef DEBUG
	printf("\nThe %d currently unused cards are:\n", gamestate->num_unused_cards);
	print_cards(gamestate->unused_cards, gamestate->num_unused_cards);
	printf("\nTable cards are: ");
	print_cards(gamestate->table_cards, gamestate->num_table_cards);
	printf("\n-------\n");
	#else
	printf("\nTable cards are: ");
	print_cards(gamestate->table_cards, gamestate->num_table_cards);
	printf("\nThere are %d unused cards\n", gamestate->num_unused_cards);
	#endif

	printf("\nbest player hand value is %d", gamestate->best_player_hand->hand_rank);
	printf(" for the combination: ");
	print_cards(gamestate->best_player_hand->cards, 5);
	printf("\n");
	printf("\nbest opponent hand value is %d ", gamestate->best_opponent_hand->hand_rank);
	printf("for the combination: ");
	print_cards(gamestate->best_opponent_hand->cards, 5);
	printf("\n\nThe player is currently %s.\n-------\n", (gamestate->player_win ? "winning" : "losing"));
	printf("The current gamestate has %d possible immediate continuation(s).\n", gamestate->num_sub_gamestates);
	printf("From the current gamestate, %ld card continuations will result in the player winning, %ld result in the player losing.\n", gamestate->num_winning_sub_gamestates, gamestate->num_losing_sub_gamestates);
	#ifdef DEBUG
	printf("\nFrom the current gamestate, there are %ld total final outcomes", (gamestate->total_winning_sub_gamestates+gamestate->total_losing_sub_gamestates));
	printf("\n%ld final outcomes result in player victory, %ld result in player defeat.\n", gamestate->total_winning_sub_gamestates, gamestate->total_losing_sub_gamestates);
	#endif
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
			root_gamestate->num_winning_sub_gamestates = 0;
			root_gamestate->num_losing_sub_gamestates = 0;
			root_gamestate->total_winning_sub_gamestates = 0;
			root_gamestate->total_losing_sub_gamestates = 0;
			return;
		}else{
			root_gamestate->possible_gamestates = malloc(sizeof(GameState*)*root_gamestate->num_unused_cards);
			root_gamestate->num_sub_gamestates = root_gamestate->num_unused_cards;

			#ifdef TESTING
			printf("assigning values to old values of %d and %d\n\n", root_gamestate->num_unused_cards, root_gamestate->num_sub_gamestates);
			root_gamestate->num_sub_gamestates = TESTING_GAMESTATE_COUNT;
			root_gamestate->num_unused_cards = TESTING_GAMESTATE_COUNT;
			printf("new vals are %d and %d\n\n", root_gamestate->num_unused_cards, root_gamestate->num_sub_gamestates);
			#endif

			for(int i = 0; i < root_gamestate->num_unused_cards; i++){

				#ifdef DEBUG
				printf("adding card: ");
				print_card_information(root_gamestate->unused_cards[i]);
				printf("\n");
				#endif

				root_gamestate->possible_gamestates[i] = malloc(sizeof(GameState));
				copy_gamestate(root_gamestate->possible_gamestates[i], root_gamestate);
				root_gamestate->possible_gamestates[i]->num_winning_sub_gamestates = 0;
				root_gamestate->possible_gamestates[i]->num_losing_sub_gamestates = 0;
				root_gamestate->possible_gamestates[i]->total_losing_sub_gamestates = 0;
				root_gamestate->possible_gamestates[i]->total_winning_sub_gamestates = 0;
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
				generate_sub_gamestates(root_gamestate->possible_gamestates[i]);

				if((root_gamestate->possible_gamestates[i]->player_win)){
					root_gamestate->num_winning_sub_gamestates++;
				}else{
					root_gamestate->num_losing_sub_gamestates++;
				}
				root_gamestate->total_losing_sub_gamestates = root_gamestate->num_losing_sub_gamestates;
				root_gamestate->total_winning_sub_gamestates = root_gamestate->num_winning_sub_gamestates;
			}			
		}
	}
}

void update_total_outcomes(GameState* root_gamestate){
	root_gamestate->total_losing_sub_gamestates = 0;
	root_gamestate->total_winning_sub_gamestates = 0;
	for(int i = 0; i < root_gamestate->num_unused_cards; i++){
		root_gamestate->total_losing_sub_gamestates+=root_gamestate->possible_gamestates[i]->num_losing_sub_gamestates;
		root_gamestate->total_winning_sub_gamestates+=root_gamestate->possible_gamestates[i]->num_winning_sub_gamestates;
	}
}

void print_gamestate_tree(GameState* root_gamestate){
	print_gamestate_information(root_gamestate);

		for(int i = 0; i < root_gamestate->num_sub_gamestates; i++){
			print_gamestate_tree(root_gamestate->possible_gamestates[i]);
		}	

	printf("\n");
}