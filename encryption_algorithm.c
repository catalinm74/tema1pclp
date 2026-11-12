#include <string.h>
#include <stdlib.h>

int get_matrix_id(char ch){
	if (ch >= 'A' && ch <= 'Z')
			return ch - 'A' + 10;
	else if (ch >= '0' && ch <= '9'){
		return ch - '0';
	} else if (ch == '.')
		return 36;
	return -1;
}

char get_matrix_char(int id){
	if (id >= 10 && id <= 35){
		return (id - 10) + 'A';
	} else if (id == 36)
		return '.';
	return id + '0';
}

void encrypt_block(char *result, char *key, int chunk_size){
	int **mat_key = calloc(chunk_size, sizeof(int *));
	for (int i = 0; i < chunk_size; i++)
		mat_key[i] = calloc(chunk_size, sizeof(int));

	int total_index = 0;
	for (int i = 0; i < chunk_size; i++){
		for (int j = 0; j < chunk_size; j++){
			mat_key[i][j] = get_matrix_id(key[total_index]);
			total_index++;
		}
	}


	int *matrix_message = calloc(chunk_size, sizeof(int));

	for (int i = 0; i < chunk_size; i++)
		matrix_message[i] = get_matrix_id(result[i]);

	int *encryption_result = calloc(chunk_size, sizeof(int));
	for (int i = 0; i < chunk_size; i++){
		for (int j = 0; j < chunk_size; j++){
			encryption_result[i] += mat_key[i][j] * matrix_message[j];
		}
		encryption_result[i] %= 37;
		result[i] = get_matrix_char(encryption_result[i]);
	}

	for (int i = 0; i < chunk_size; i++)
		free(mat_key[i]);
	free(mat_key);
	free(matrix_message);
	free(encryption_result);

}

void encrypt(char *result, char *key){
	int chunk_size = 0;
	int len_key = strlen(key);
	for (int i = 1; i <= 3; i++){
		if (i * i <= len_key)
			chunk_size = i;
	}

	int result_length = strlen(result);

	for (int i = 0; i < result_length; i += chunk_size){
		int remaining = result_length - i;
		if (remaining < chunk_size){
			chunk_size = remaining;
		}
		encrypt_block(result + i, key, chunk_size);
	}	
}

int modular_inverse(int x){
	for (int i = 1; i < 37; i++){
		if ((x * i) % 37 == 1)
			return i;
	}
	return -1;
}

int calculate_determinant(int **mat_key, int chunk_size){
	int determinant = 0;
	if (chunk_size == 1)
		determinant = mat_key[0][0];
	else if (chunk_size == 2)
		determinant = mat_key[0][0] * mat_key[1][1] - mat_key[0][1] * mat_key[1][0];
	else
		determinant = mat_key[0][0] * mat_key[1][1] * mat_key[2][2] +
				mat_key[1][0] * mat_key[2][1] * mat_key[0][2] +
				mat_key[2][0] * mat_key[0][1] * mat_key[1][2] -
				mat_key[0][2] * mat_key[1][1] * mat_key[2][0] -
				mat_key[1][2] * mat_key[2][1] * mat_key[0][0] - 
				mat_key[2][2] * mat_key[0][1] * mat_key[1][0];

	while (determinant < 0)
		determinant += 37;
	
	determinant = determinant % 37;
	return determinant;
}

void create_adjunct_matrix(int **mat, int chunk_size) {
    int **temp = calloc(chunk_size, sizeof(int *));
    for (int i = 0; i < chunk_size; i++) {
        temp[i] = calloc(chunk_size, sizeof(int));
        for (int j = 0; j < chunk_size; j++) {
            temp[i][j] = mat[i][j];
        }
    }

    if (chunk_size == 1) {
        mat[0][0] = 1;
    } else if (chunk_size == 2) {
        mat[0][0] =  temp[1][1];
        mat[0][1] = -temp[0][1];
        mat[1][0] = -temp[1][0];
        mat[1][1] =  temp[0][0];
    } else {
        mat[0][0] =  (temp[1][1] * temp[2][2] - temp[1][2] * temp[2][1]);
        mat[0][1] = -(temp[0][1] * temp[2][2] - temp[0][2] * temp[2][1]);
        mat[0][2] =  (temp[0][1] * temp[1][2] - temp[0][2] * temp[1][1]);

        mat[1][0] = -(temp[1][0] * temp[2][2] - temp[1][2] * temp[2][0]);
        mat[1][1] =  (temp[0][0] * temp[2][2] - temp[0][2] * temp[2][0]);
        mat[1][2] = -(temp[0][0] * temp[1][2] - temp[0][2] * temp[1][0]);

        mat[2][0] =  (temp[1][0] * temp[2][1] - temp[1][1] * temp[2][0]);
        mat[2][1] = -(temp[0][0] * temp[2][1] - temp[0][1] * temp[2][0]);
        mat[2][2] =  (temp[0][0] * temp[1][1] - temp[0][1] * temp[1][0]);
    }

    for (int i = 0; i < chunk_size; i++)
        free(temp[i]);
    free(temp);
}

void decrypt_block(char *encrypted_message, char *key, int chunk_size){
	int **mat_key = calloc(chunk_size, sizeof(int *));
	for (int i = 0; i < chunk_size; i++)
		mat_key[i] = calloc(chunk_size, sizeof(int));

	int total_index = 0;
	for (int i = 0; i < chunk_size; i++){
		for (int j = 0; j < chunk_size; j++){
			mat_key[i][j] = get_matrix_id(key[total_index]);
			total_index++;
		}
	}

	int determinant = 0;

	determinant = calculate_determinant(mat_key, chunk_size);
	int multi_inverse = modular_inverse(determinant);

	create_adjunct_matrix(mat_key, chunk_size);

	for (int i = 0; i < chunk_size; i++){
		for (int j = 0; j < chunk_size; j++)
			mat_key[i][j] = (((mat_key[i][j] * multi_inverse) % 37) + 37) % 37;
	}

	int *decrypted_message = calloc(chunk_size, sizeof(int));

	for (int i = 0; i < chunk_size; i++){
		for (int j = 0; j < chunk_size; j++)
			decrypted_message[i] += mat_key[i][j] * get_matrix_id(encrypted_message[j]);
		decrypted_message[i] %= 37;
	}

	for (int i = 0; i < chunk_size; i++){
		encrypted_message[i] = get_matrix_char(decrypted_message[i]);
	}

	for (int i = 0; i < chunk_size; i++)
		free(mat_key[i]);
	free(mat_key);

	free(decrypted_message);
}

void decrypt(char *encrypted_message, char *key){
	int chunk_size = 0;
	int len_key = strlen(key);
	for (int i = 1; i <= 3; i++){
		if (i * i <= len_key)
			chunk_size = i;
	}

	int result_length = strlen(encrypted_message);
	
	for (int i = 0; i < result_length; i += chunk_size){
		int remaining = result_length - i;
		if (remaining < chunk_size){
			chunk_size = remaining;
		}
		decrypt_block(encrypted_message + i, key, chunk_size);
	}
}