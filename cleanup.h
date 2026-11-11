#ifndef CLEANUP_H
#define CLEANUP_H

#include "types.h"

//deallocates memory for players
void free_memory(player_status *player);
void clear_statistics(player_status *player);
void program_abort(player_status *player);

#endif