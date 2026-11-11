#ifndef TYPES_H
#define TYPES_H

typedef struct {
	// stores the ship occupying a board cell and whether that cell was hit
	int id;
	int hit;

} cell;

typedef struct {
	// stores the properties and current state of one ship
	char type;
	int size;
	char orientation;
	int base_x;
	int base_y;

} ship;

typedef struct {
	// stores player info

	int rows, columns;

	int matches_played;
	int *ships_placed;

	int *total_hits;
	int *landed_hits;

	float *match_accuracies;
	float *match_accuracies_ponderated;
	

	ship *ships;
	int ship_count;
	int *remaining_ships;
	int remaining_ship_count;
	
	cell **board;

} player_status;

#endif