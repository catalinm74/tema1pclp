#include <stdio.h>
#include "types.h"

void print_game_matrix(player_status *player){
	// convert each board cell to its displayed ship size or zero
	cell **matrix = player->board;

	int rows = player->rows;
	int columns = player->columns;

	for (int i = 0; i < rows; i++){
		for (int j = 0; j < columns; j++){
			if (matrix[i][j].id != -1){
				printf("%d", player->ships[matrix[i][j].id].size + 1);
			} else {
				printf("0");
			}
			if (j != columns - 1)
				printf(" ");
		}
		printf("\n");
	}
}

void print_messages(
	int x, int y, int base_x, int base_y, int player_number, char message, int type, char orientation
){

	static const char *const ship_names[] = {
		"Albacore",
		 "Laffey",
		 "Belfast",
		 "Yamato",
		 "Shinano"
	};

	// select the message based on the result code returned by the game logic
	switch(message){
		//M = missed, H = hit, D = destroyed, I = impossible, A = already, W = win
		case 'M':

			printf("Jucatorul %d a ratat o lovitura la coordonatele (%d, %d).\n",
				 player_number, x + 1, y + 1);
			break;

		case 'H':

			printf("Jucatorul %d a lovit o nava %s la coordonatele (%d, %d).\n",
				 player_number, ship_names[type], x + 1, y + 1);
			break;

		case 'D':

			printf("Jucatorul %d a distrus o nava %s plasata intre coordonatele (%d, %d)",
				 player_number, ship_names[type], base_x + 1, base_y + 1);

			if (orientation == 'H'){
				printf(" si (%d, %d).\n", base_x + 1, base_y + type + 1);
			} else {
				printf(" si (%d, %d).\n", base_x - type + 1, base_y + 1);
			}

			break;

		case 'I': {

			const char *direction = orientation == 'H' ? "orizontal": orientation == 'V' ? "vertical": "necunoscuta";

			printf("Nava %s nu poate fi amplasata %s la coordonatele (%d, %d).\n",
				 ship_names[type], direction, x + 1, y + 1);
			break;

		}
		case 'A':

			printf("Coordonatele (%d, %d) au fost deja atacate de jucatorul %d.\n",
				 x + 1, y + 1, player_number);
			break;

		case 'W':

			printf("Jucatorul %d a castigat.\n", player_number);
			break;
		
	}
}
