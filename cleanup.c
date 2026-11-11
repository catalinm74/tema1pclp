#include "types.h"
#include <stdlib.h>

void free_memory(player_status *player){
	if (player->ships){
		free(player->ships);
		player->ships = NULL;
	}
	if (player->board)
		for (int i = 0; i < player->rows; i++){
			if (player->board[i]){
				free(player->board[i]);
				player->board[i] = NULL;
			}
		}

	free(player->board);
	player->board = NULL;
	free(player->remaining_ships);
	player->remaining_ships = NULL;
}

void clear_statistics(player_status *player){
	free(player->match_accuracies);
	player->match_accuracies = NULL;
	free(player->match_accuracies_ponderated);
	player->match_accuracies_ponderated = NULL;
	free(player->total_hits);
	player->total_hits = NULL;
	free(player->landed_hits);
	player->landed_hits = NULL;
	free(player->ships_placed);
	player->ships_placed = NULL;
}

void program_abort(player_status *player){
	free_memory(player);
	clear_statistics(player);
}

