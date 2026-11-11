#ifndef GAME_H
#define GAME_H
#include "types.h"

// reads and places one ship for a player
int place_ship(player_status *player);

// runs the placement and attack rounds of a match
void play_match(player_status *player_1, player_status *player_2);

// allocates and initializes a player's board and ships
int initialize_player(player_status *player, int rows, int columns);

//initializes statistics for players before the first match
int create_statistics(player_status *player, int matches);

#endif
