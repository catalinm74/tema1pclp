#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "encryption_algorithm.h"

int comparator_descendant(const void *a, const void *b){
	const double first = *(double *)a;
	const double second = *(double *)b;
	return (first < second) - (first > second);
}

int comparator_ascendant(const void *a, const void *b){
	const double first = *(double *)a;
	const double second = *(double *)b;
	return (first > second) - (first < second);
}

int calculate_multiple(player_status *player, double target_accuracy, int req_type){
	//logic for tasks 1 and 3

	int total_ships_placed = 0;
	for (int i = 0; i < player->matches_played; i++)
		total_ships_placed += player->ships_placed[i];

	double target_number = (target_accuracy * total_ships_placed) / 100.0;
	//this target number needs to be larger than my hypothetical
	// pondered sum for task 1
	//we need to create a potential gains array to see which matches are the most
	//impactful if we change their accuracies to 100%

	double *gains = NULL;
	double *losses = NULL;
	if (req_type == 0){
		gains = calloc(player->matches_played, sizeof(double));
		//calculating task 1, gains array going to be sorted by biggest impact
		for (int i = 0; i < player->matches_played; i++){
			//potential gain if i change this respective match to 100% accuracy
			gains[i] = player->ships_placed[i] - player->match_accuracies_ponderated[i];
		}
		qsort(gains, player->matches_played, sizeof(double), comparator_descendant);

		//now we will add as many gains as we need until we reach target number
		//since we already subtracted the actual ponderated accuracy in gains, we
		//don't need to worry about making sure if we removed the old accuracy

		double current_sum = 0;

		for (int i = 0; i < player->matches_played; i++){
			current_sum += player->match_accuracies_ponderated[i];
		}

		int rez = 0;

		//check if accuracy is already good enough
		if (current_sum >= target_number){
			free(gains);
			return rez;
		}

		for (int i = 0; i < player->matches_played; i++){
			current_sum += gains[i];
			rez++;
			if (current_sum >= target_number){
				free(gains);
				return rez;
			}
		}
	} else {
		losses = calloc(player->matches_played, sizeof(double));
		//for task 3, our array will count the losses we'd
		// have if we have 0% accuracy in the match
		for (int i = 0; i < player->matches_played; i++){
			losses[i] = player->match_accuracies_ponderated[i];
		}

		qsort(losses, player->matches_played, sizeof(double), comparator_ascendant);

		double current_sum = 0;

		for (int i = 0; i < player->matches_played; i++){
			current_sum += player->match_accuracies_ponderated[i];
		}

		int rez = 0;

		for (int i = 0; i < player->matches_played; i++){
			current_sum -= losses[i];
			rez++;
			if (current_sum < target_number){
				rez--;
				free(losses);
				return rez;
			}
		}
	}
	free(gains);
	free(losses);
	return 0;
}

int calculate_single (player_status *player, int match_index, double target_accuracy, int req_type){

	//logic for tasks 2 and 4, calculate for task 4 first
	int hits = player->landed_hits[match_index];
	int total_hits = player->total_hits[match_index];
	int surplus = (int)floor(hits - (target_accuracy * total_hits / 100.0) + 1e-9);
	//task 4 is just a reversed task 2, if we calculate for task 4 then task 2 is -task4
	//task 4 calculated how many hits we could've removed, task 2 calculates how many hits we needed to add
	int rez = (req_type == 0) ? -surplus : surplus;
	if (rez < 0) rez = 0;

	return rez;
}

int parse_data(player_status *player, char *requirement, char *result){
	
	int match_index = (requirement[2] - '0') * 100
                    + (requirement[3] - '0') * 10
                    + (requirement[4] - '0') - 1;

	if (match_index < 0 || match_index >= player->matches_played){
		printf("Nu exista date pentru meciul cerut.\n");
		return 0;
	}

	double target_accuracy = (requirement[5] - '0') * 100.0
                           + (requirement[6] - '0') * 10.0
                           + (requirement[7] - '0')
                           + (requirement[9] - '0') * 0.1
                           + (requirement[10] - '0') * 0.01;

	if (requirement[1] == 'U'){
		int acc_single = (int)(player->match_accuracies[match_index] * 10000.0 + 1e-9);

		if (acc_single / 100 < 100){
			result[0] = '0';
			result[1] = (acc_single / 100) / 10 + '0';
			result[2] = (acc_single / 100) % 10 + '0';
		} else {
			result[0] = '1';
			result[1] = '0';
			result[2] = '0';
		}
		result[3] = '.';

		if (acc_single % 100 == 0 || acc_single % 100 < 10){
			result[4] = '0';
			if (acc_single % 100 < 10)
				result[5] = acc_single % 100 + '0';
		} else {
			result[4] = (acc_single % 100 / 10) + '0';
			result[5] = acc_single % 10 + '0';
		}

	} else {
		double acc_ponderated = 0;
		for (int i = 0; i < player->matches_played; i++){
			acc_ponderated += player->match_accuracies_ponderated[i];
		}
		int total_ships_placed = 0;
			for (int i = 0; i < player->matches_played; i++)
				total_ships_placed += player->ships_placed[i];
		acc_ponderated /= total_ships_placed * 1.0;

		int acc_ponderated_int = (int)(acc_ponderated * 10000.0 + 1e-9);

		if (acc_ponderated_int / 100 < 100){
			result[0] = '0';
			result[1] = (acc_ponderated_int / 100) / 10 + '0';
			result[2] = (acc_ponderated_int / 100) % 10 + '0';
		} else {
			result[0] = '1';
			result[1] = '0';
			result[2] = '0';
		}
		result[3] = '.';

		if (acc_ponderated_int % 100 == 0 || acc_ponderated_int % 100 < 10){
			result[4] = '0';
			if (acc_ponderated_int % 100 < 10)
				result[5] = acc_ponderated_int % 100 + '0';
		} else {
			result[4] = (acc_ponderated_int % 100 / 10) + '0';
			result[5] = acc_ponderated_int % 10 + '0';
		}
	}

	result[6] = '.';

	int rez = 0;

	if (strncmp(requirement + 11, "MIN", 3) == 0){
		if (requirement[1] == 'U'){
			// task 2
			rez = calculate_single(player, match_index, target_accuracy, 0);
		} else {
			//task 1
			rez = calculate_multiple(player, target_accuracy, 0);
		}
	} else {
		if (requirement[1] == 'U'){
			//task 4
			rez = calculate_single(player, match_index, target_accuracy, 1);
		} else {
			//task 3
			rez = calculate_multiple(player, target_accuracy, 1);
		}
	}

	snprintf(result + 7, 13, "%d", rez);
	return 1;
}

void manage_statistics(player_status *player_1, player_status *player_2){
	char *key = NULL;
	char *requirement = NULL;

	size_t size = 0;
	size_t size_req = 0;

	//removes stray '\n's from previous scanf
	int character;
	while ((character = getchar()) != '\n' && character != EOF)
    ;

	int len = getline(&key, &size, stdin);
	if (len == -1) {
		free(key);
		return;
	}

	if (key[len - 1] == '\n')
    	key[len - 1] = '\0';

	for (int i = 0; i < player_1->matches_played; i++){
		player_1->match_accuracies[i] = 
		1.0 * player_1->landed_hits[i] / player_1->total_hits[i] * 1.0;
		player_2->match_accuracies[i] = 
		1.0 * player_2->landed_hits[i] / player_2->total_hits[i] * 1.0;

		player_1->match_accuracies_ponderated[i] =
		player_1->match_accuracies[i] * player_1->ships_placed[i];

		player_2->match_accuracies_ponderated[i] =
		player_2->match_accuracies[i] * player_2->ships_placed[i];
	}

	if (key[0] == 'Q'){
		free(key);
		free(requirement);
		return;
	}

	while ((len = getline(&requirement, &size_req, stdin)) != -1){

		char *result = malloc(20 * sizeof(char));

		if (requirement[len - 1] == '\n')
        	requirement[len - 1] = '\0';

		if (requirement[0] == 'Q' && requirement[1] == '\0')
        	break;

		if (key[0] != '-')
			decrypt(requirement, key);
		
		player_status *player;

		if (requirement[0] == 'O')
			player = player_1;
		else
			player = player_2;

		if (parse_data(player, requirement, result)){
			if (key[0] != '-'){
				encrypt(result, key);
			}

			printf("%s\n", result);
			free(result);
		}
	}

	free(key);
	free(requirement);
}
