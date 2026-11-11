#ifndef PRINTING_H
#define PRINTING_H

#include "types.h"

// prints the board using ship sizes and zeroes for empty cells
void print_game_matrix(player_status *player);

// prints the message corresponding to the result of an action
void print_messages(
	int x, int y, int base_x, int base_y, int player_number, char message, int type, char orientation
);

#endif