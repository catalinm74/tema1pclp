#ifndef VALIDATION_H
#define VALIDATION_H

#include "types.h"

// converts a ship letter to its internal size index
int get_ship_size(char ship_type);

// removes the ship from the board while preserving already hit cells
void mark_ship_destroyed(int id, player_status *player);

// validates the position and orientation of a ship
int is_valid_placement(player_status *player, ship temporary_ship);

// returns whether a coordinate is inside the board
int in_bounds(long long rows, long long columns, int x, int y);

// checks if a ship is destroyed or alive
int check_ship_status(int id, player_status *player);

#endif