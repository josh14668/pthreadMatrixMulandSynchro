#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
float **A,**B, **C; //global variables for two-dimensional arrays
int M, K, N; //matrix dimensions
int num_thrds; //number of threads to be created

//thread routine for matrix multiplication.
void *mat_mult_thrds(void *threadarg)
{
	//local variables declaration
	... please fill in if any.

	//retrieve my thread id from *threadarg.
	... please fill in.

	//calculate my task i.e., matrix C columns using N, num_thrds and my thread id.
	... please fill in.

	//perform my task for matrix multiplication.
	... please fill in.

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t *threads;
	int *t; //user defined thread ids
	int rc, i, j;
	void *status;
	//... add more if necessary.

	// ask for matrices dimensions M, K, N for the user.
	... please fill in.

	//ask for the number of threads num_thrds to be created from the user.
	... please fill in.

	//create matrices A, B and C using malloc().
	... please fill in.

	//initialize matrices A and B using srand() and rand()
	//the range of float values should be between 0 and 1.0.
	//initialize matrix C.
	... please fill in.

	//allocate memory for thread ids.
	... please fill in.

	//create threads.
	for(i=0; i<num_thrds; i++) {
		t[i] = i; //user defined thread id to be passed to the thread routine
		rc = pthread_create(&threads[i], NULL, mat_mult_thrds, (void *) &t[i]);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	//join threads.
	for(i=0;i<num_thrds;i++) {
	... please fill in.

	//check correctness -
	//perform a sequential computation using the same A and B and
	//compare the results.
	... please fill in.

 	printf("\n Output Matrix C:\n");
	for(i=0; i<M; i++){
		for(j=0; j<N; j++){
			printf("%f  ", C[i][j]);
			if(j == N - 1)
				printf("\n\n");
		}
	}

	pthread_exit(NULL);
}
