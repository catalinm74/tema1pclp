#ifndef STATISTICS_H
#define STATISTICS_H

#include "types.h"

void manage_statistics(player_status *player_1, player_status *player_2);

void parse_data(player_status *player, char *requirement, char *result);

int calculate_single (player_status *player, int match_index, double target_accuracy, int req_type);

int calculate_multiple(player_status *player, double target_accuracy, int req_type);

#endif