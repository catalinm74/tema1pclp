#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "types.h"
#include "validation.h"
#include "printing.h"
#include "cleanup.h"

int place_ship(player_status *player){

	// reads a ship description and returns whether it was placed successfully

	ship temporary_ship;

	scanf(" %c %c %d %d",
		&temporary_ship.type,
		&temporary_ship.orientation,
		&temporary_ship.base_x,
		&temporary_ship.base_y
	);

	temporary_ship.base_x--;
	temporary_ship.base_y--;
	temporary_ship.size = get_ship_size(temporary_ship.type);

	// check if ship can be placed or if player ran out of ships of that size
	if (!player->remaining_ships[temporary_ship.size] || 
		!is_valid_placement(player, temporary_ship)){

		print_messages(temporary_ship.base_x,
			temporary_ship.base_y, 0, 0, 0, 'I',
			temporary_ship.size,
			temporary_ship.orientation
		);

		return 0;
	}

	player->remaining_ships[temporary_ship.size]--;
	player->ships[player->ship_count++] = temporary_ship;

	// place ship on matrix, respective cell is the ship ID
	if (temporary_ship.orientation == 'H') {
		for (int i = temporary_ship.base_y; i <= temporary_ship.base_y + temporary_ship.size; i++)
			player->board[temporary_ship.base_x][i].id = player->ship_count - 1;
	} else if (temporary_ship.orientation == 'V') {
		for (int i = temporary_ship.base_x; i >= temporary_ship.base_x - temporary_ship.size; i--)
			player->board[i][temporary_ship.base_y].id = player->ship_count - 1;
	}

	return 1;
}

char start_attack(int x, int y, player_status *victim){

	// determine the attack result from the target cell and update its state
	int id = victim->board[x][y].id;
	if (victim->board[x][y].hit){
		return 'A'; 
	} else if (id == -1){
		victim->board[x][y].hit = 1;
		return 'M';
	} else {
		if (x == victim->ships[id].base_x &&
		y == victim->ships[id].base_y && victim->board[x][y].hit == 0) {
			victim->board[x][y].hit = 1;
			mark_ship_destroyed(id, victim);
			return 'D';
		}
		victim->board[x][y].hit = 1;
		int result = check_ship_status(victim->board[x][y].id, victim);
		if (result == 1)
			return 'H';
		else {
			mark_ship_destroyed(victim->board[x][y].id, victim);
			return 'D';
		}
	}

}

void play_match(player_status *player_1, player_status *player_2){

	// alternate between players while both players place all their ships
	int match_id = player_1->matches_played - 1;
	int current_player = 1;
	int total_ships = 2 * player_1->remaining_ship_count;
	while (total_ships){

		player_status *current_status = current_player == 1 ? player_1 : player_2;
		while (!place_ship(current_status)) {
		}

		current_status->ships_placed[match_id]++;

		current_player = current_player == 1 ? 2 : 1;

		total_ships--;

	}

	//print game boards
	print_game_matrix(player_1);
	printf("\n");
	print_game_matrix(player_2);

	// alternate attacks until the current victim has no remaining ships
	current_player = 1;
	player_status *victim = player_2;

	while (victim->remaining_ship_count){

		player_status *current_status =
		 current_player == 1 ? player_1 : player_2;
		// scan the attack coordinates and identify the ship at that location
		int x, y;
		scanf(" %d %d", &x, &y);
		x--;
		y--;
		int id = victim->board[x][y].id;
		int type = 0;
		char orientation = ' ';

		int base_x = 0;
		int base_y = 0;

		// initialize the message parameters for the attack result
		if (id != -1) {
			type = victim->ships[id].size;
			orientation = victim->ships[id].orientation;
			base_x = victim->ships[id].base_x;
			base_y = victim->ships[id].base_y;
		}

		// launch the attack and print the corresponding message
		char attack_result = start_attack(x, y, victim);
		print_messages(x,
			y,
			base_x,
			base_y,
			current_player,
			attack_result,
			type,
			orientation
		);

		current_status->total_hits[match_id]++;

		if (attack_result != 'M' && attack_result != 'A')
			current_status->landed_hits[match_id]++;

		// stop the match when the last ship has been destroyed
		if (attack_result == 'D' && victim->remaining_ship_count == 0){
			print_messages(
			x,
			y,
			base_x,
			base_y,
			current_player,
			'W',
			type,
			orientation
			);
			break;
		}
		current_player = current_player == 1 ? 2 : 1;
		victim = victim == player_2 ? player_1 : player_2;
	}

	if (player_1->total_hits[match_id]){
		player_1->match_accuracies[match_id] = 
		1.0 * player_1->landed_hits[match_id] / player_1->total_hits[match_id];
	}

	if (player_2->total_hits[match_id]){
		player_2->match_accuracies[match_id] =
		1.0 * player_2->landed_hits[match_id] / player_2->total_hits[match_id];
	}

	player_1->match_accuracies_ponderated[match_id] =
	player_1->match_accuracies[match_id] * player_1->ships_placed[match_id];

	player_2->match_accuracies_ponderated[match_id] =
	player_2->match_accuracies[match_id] * player_2->ships_placed[match_id];

}

int initialize_player(player_status *player, int rows, int columns){

	// allocate the player's ship counters and board
	player->rows = rows;
	player->columns = columns;

	player->remaining_ships = malloc(5 * sizeof(int));
	if (player->remaining_ships == NULL)
		return 1;

	player->remaining_ship_count = 0;
	player->matches_played++;

	int match = player->matches_played - 1;
	player->landed_hits[match] = 0;
	player->total_hits[match] = 0;
	player->ships_placed[match] = 0;
	player->match_accuracies[match] = 0.0;
	player->match_accuracies_ponderated[match] = 0.0;

	player->board = calloc(rows, sizeof(cell *));
	if (player->board == NULL)
		return 1;

	for (int i = 0; i < rows; i++){

		// empty cells have no ship id and have not been hit
		player->board[i] = malloc(columns * sizeof(cell));
		if(player->board[i] == NULL)
			return 1;

		for (int j = 0; j < columns; j++){
			player->board[i][j].id = -1;
			player->board[i][j].hit = 0;
		}

	}

	const long long values[] = {20, 30, 40, 55, 70};
	
	// calculate how many ships of each type the board can contain
	for (int i = 0; i <= 4; i++){
		player->remaining_ships[i] = rows * columns / values[i];
		player->remaining_ship_count += player->remaining_ships[i];
	}
	player->ship_count = 0;
	player->ships = malloc(player->remaining_ship_count * sizeof(ship));
	if (player->ships == NULL)
		return 1;
	return 0;
}

int create_statistics(player_status *player, int matches){

	player->matches_played = 0;
	player->match_accuracies = malloc(matches * sizeof(float));
	if (player->match_accuracies == NULL)
		return 1;
	player->match_accuracies_ponderated = malloc(matches * sizeof(float));
	if (player->match_accuracies_ponderated == NULL)
		return 1;
	player->total_hits = malloc(matches * sizeof(int));
	if (player->total_hits == NULL)
		return 1;
	player->landed_hits = malloc(matches * sizeof(int));
	if (player->landed_hits == NULL)
		return 1;
	player->ships_placed = malloc(matches * sizeof(int));
	if (player->ships_placed == NULL)
		return 1;

	return 0;
}