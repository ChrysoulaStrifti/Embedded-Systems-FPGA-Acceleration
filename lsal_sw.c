// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>



const short GAP_i = -1;
const short GAP_d = -1;
const short MATCH = 2;
const short MISS_MATCH = -1;
const short CENTER = 0;
const short NORTH = 1;
const short NORTH_WEST = 2;
const short WEST = 3;


void compute_matrices(
	char *string1, char *string2,
	int *max_index, int *similarity_matrix, short *direction_matrix, int N, int M)
{

	int index = 0;
	int i = 0;
	int j = 0;

	// Following values are used for the N, W, and NW values wrt. similarity_matrix[i]

	int north = 0;
	int west = 0;
	int northwest = 0;

	int max_value = 0;
	int max_cell = 0;//maximum for the whole array
	int max_cell_index = 0;//starting point

	int match;
	int test_val;
	int dir;

	// Main computation
	for (index = 0; index < N*M; index++) {

		i = index % N; // column index
		j = index / N; // row index

		if ((i == 0) && (j == 0)) {
			north = 0;
			northwest = 0;
			west = 0;
		}

		else if (j == 0) {
			// first row 
			north = 0;
			northwest = 0;
			west = similarity_matrix[index - 1] -1;
		}


		else if (i == 0) {
			// first column. 
			west = 0;
			northwest = 0;
			north = similarity_matrix[index - N] - 1;
		}

		else {//not first column       
			north = similarity_matrix[index - N] - 1;
			west = similarity_matrix[index - 1] - 1;
			northwest = similarity_matrix[index - 1 - N];
		}

		match = (string1[i] == string2[j]) ? MATCH : MISS_MATCH; //2 or -1
		test_val = northwest + match;
		max_value = test_val;
		dir = NORTH_WEST;
		
		//search for maximum value[test_val, north, west, 0]

		//north
		if (max_value < north) {
			max_value = north;
			dir = NORTH;
		}
		//west
		if (max_value < west) {
			max_value = west;
			dir = WEST;
		}
		//max=0-> default
		if (max_value <= 0) {
			max_value = 0;
			dir = CENTER;
		}
		//write
		similarity_matrix[index] = max_value;
		direction_matrix[index] = dir;

		if (max_cell < max_value) {
			max_cell = max_value;
			max_index[0] = index;
		}

	}   // end of for-loop
}  // end of function

/************************************************************************/

/*
 return a random number in [0, limit].
 */
int rand_lim(int limit) {

	int divisor = RAND_MAX / (limit + 1);
	int retval;

	do {
		retval = rand() / divisor;
	} while (retval > limit);

	return retval;
}

/*
 Fill the string with random values
 */
void fillRandom(char* string, int dimension) {
	//fill the string with random letters..
	static const char possibleLetters[] = "ATCG";

	string[0] = '-';

	int i;
	for (i = 0; i < dimension; i++) {
		int randomNum = rand_lim(3);
		string[i] = possibleLetters[randomNum];
	}

}
int main(int argc, char** argv) {

	clock_t t1, t2;

	if (argc != 3) {
		printf("%s <Query Size N> <DataBase Size M>\n", argv[0]);
		return EXIT_FAILURE;
	}

	printf("Starting Local Alignment Code \n");
	fflush(stdout);

	/* Typically, M >> N */
	atoi(argv[1]);
	atoi(argv[2]);

	char *query = (char*)malloc(sizeof(char) * N);
	char *database = (char*)malloc(sizeof(char) * M);
	int *similarity_matrix = (int*)malloc(sizeof(int) * N * M);
	short *direction_matrix = (short*)malloc(sizeof(short) * N * M);
	int *max_index = (int *)malloc(sizeof(int));

	/* Create the two input strings by calling a random number generator */
	fillRandom(query, N);
	fillRandom(database, M);

	memset(similarity_matrix, 0, sizeof(int) * N * M);
	memset(direction_matrix, 0, sizeof(short) * N * M);

	t1 = clock();
	compute_matrices(query, database, max_index, similarity_matrix, direction_matrix, N, M);
	t2 = clock();

	printf("query %s\n", query);
	printf("database %s\n", database);

	printf(" max index is in position (%d, %d) \n", max_index[0] / N, max_index[0] % N);
	printf(" execution time of Smith Waterman SW algorithm is %f sec \n", (double)(t2 - t1) / CLOCKS_PER_SEC);


	for(int j=0; j<M; j++){
		for(int i=0; i<N; i++){//row
			printf("% 4d", similarity_matrix[j*N +i]);

		}
		printf("\n");
	}

	printf("\n");
	free(similarity_matrix);
	free(direction_matrix);
	free(max_index);

	return EXIT_SUCCESS;
}
