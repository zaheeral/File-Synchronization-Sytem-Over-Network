#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASH_SIZE 64

void show_hash(char *hash_val, long block_size) {
    for(int i = 0; i < block_size; i++) {
        printf("%.2hhx ", hash_val[i]);
    }
    printf("\n");
}


void hash_helper(char *hash_val, long block_size, char *input){
	int hash_index = 0;
	
	int i = 0;
	//DEBUG TO REMOVE: printf("Curr input: %s\n", input);
	//DEBUG TO REMOVE: printf("Strlen of input: %d\n", strlen(input));
	//DEBUG TO REMOVE: printf("Curr input: %s", input);
	for(i=0; i<strlen(input); i++){
		// Input XOR value in hash_val
		//DEBUG TO REMOVE: printf("Curr input char: %c\n", input[i]);
printf("Current hash_val: %c\n", hash_val[i]);
		hash_val[hash_index] ^= input[i];

		// Increase hash index
		hash_index++;
		if(hash_index >= block_size){
			hash_index = 0;
		}
	}
}


char *hash(FILE *f){
printf("Beginning Hash...");
	char input[HASH_SIZE + 1];
	char *hash_val = malloc(HASH_SIZE);
	char *is_eof;
printf("Rewinding...\n");rewind(f);

	char empty_hash[HASH_SIZE+1] = {'\0'};
	//strcpy(hash_val, empty_hash);
	int i;
	for(i=0; i<=64; i++){
		printf("Emptying...");
		hash_val[i] = empty_hash[i];
	}

	while((is_eof = fgets(input, HASH_SIZE, f)) != NULL){
printf("Curr input: %s", input);
		hash_helper(hash_val, HASH_SIZE, input);
	}
	//printf("After hash, in 'hash_functions'\n");
	//show_hash(&hash_val, HASH_SIZE);
	return hash_val;
}


int check_hash(const char *hash1, const char *hash2, long block_size) {
	int hash_index;
	// Loop through hashes
	for(hash_index=0; hash_index<block_size; hash_index++){
		// Return index if non-match is found
		if(hash1[hash_index] != hash2[hash_index]){
			return hash_index;
		}
	}
	// Return block_size if no non-matches are found
	return block_size;
}

