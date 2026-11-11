#include "types.h"

int check_ship_status(int id, player_status *player){

	// check if ship is alive by seeing if any of its cells have not been attacked yet
	ship temporary_ship = player->ships[id];
	int x = temporary_ship.base_x;
	int y = temporary_ship.base_y;
	int orientation = temporary_ship.orientation;
	int size = temporary_ship.size;

	if (orientation == 'H'){
		for (int i = y; i <= y + size; i++){
			if (player->board[x][i].hit == 0)
				return 1;
		}
	} else {
		for (int i = x; i >= x - size; i--){
			if (player->board[i][y].hit == 0)
				return 1;
		}
	}
	return 0;
}


int get_ship_size(char ship_type){

	// Ship letters map to indexes used for sizes and remaining counts.
	switch(ship_type){

		case 'S':

			return 4;

		case 'Y':

			return 3;

		case 'B':

			return 2;

		case 'L':

			return 1;

		case 'A':

			return 0;

		default:

			return 0;
	}

}

void mark_ship_destroyed(int id, player_status *player){
	// remove the ship id from the board
	ship ship_to_destroy = player->ships[id];
	int x = ship_to_destroy.base_x;
	int y = ship_to_destroy.base_y;
	int orientation = ship_to_destroy.orientation;
	int size = ship_to_destroy.size;

	if (orientation == 'H'){
		for (int i = y; i <= y + size; i++){
			player->board[x][i].id = -1;
		}
	} else {
		for (int i = x; i >= x - size; i--){			
			player->board[i][y].id = -1;
		}
	}
	player->remaining_ship_count--;
}

int in_bounds(long long rows, long long columns, int x, int y){
	return (x >= 0 && x < rows && y >= 0 && y < columns);
}

int is_valid_placement(player_status *player, ship temporary_ship){

	// Validate the starting coordinate before checking the ship's full length.
	cell **board = player->board;
	int rows = player->rows;
	int columns = player->columns;
	int x = temporary_ship.base_x;
	int y = temporary_ship.base_y;
	int size = temporary_ship.size;
	char orientation = temporary_ship.orientation;

	if (!in_bounds(rows, columns, x, y))
    	return 0;

	if (orientation == 'H'){
		// Horizontal ships extend toward increasing column indexes.
		if (!in_bounds(rows, columns, x, y + size))
			return 0;

		for (int i = y; i <= y + size; i++)
			if (board[x][i].id != -1)
				return 0;	
	} else if (orientation == 'V'){
		// Vertical ships extend toward decreasing row indexes.
		if (!in_bounds(rows, columns, x - size, y))
			return 0;
		
		for (int i = x; i >= x - size; i--)
			if (board[i][y].id != -1)
				return 0;	
	} else {
		return 0;
	}
	
	return 1;
}