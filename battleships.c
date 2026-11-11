#include <stdio.h>
#include "game.h"
#include "types.h"
#include "cleanup.h"


int main(void){

	// initialize players, read matrix size and match count
	player_status player_1 = {0}, player_2 = {0};

	int matches, rows, columns;
	scanf("%d", &matches);

	if (create_statistics(&player_1, matches) == 1){
		program_abort(&player_1);
		program_abort(&player_2);
		return 1;
	}

	if (create_statistics(&player_2, matches) == 1){
		program_abort(&player_1);
		program_abort(&player_2);
		return 1;
	}


	// run matches then free memory
	while (matches){
		matches--;
		scanf("%d%d", &rows, &columns);
		if (initialize_player(&player_1, rows, columns) == 1){
			program_abort(&player_1);
			program_abort(&player_2);
			return 1;
		}
		if (initialize_player(&player_2, rows, columns) == 1){
		program_abort(&player_1);
		program_abort(&player_2);
			return 1;
		}

		play_match(&player_1, &player_2);

		free_memory(&player_1);
		free_memory(&player_2);

	}

	clear_statistics(&player_1);
	clear_statistics(&player_2);

	return 0;
}