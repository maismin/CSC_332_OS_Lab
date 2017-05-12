#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "sem.h"

#define PERMS 0666 //0666 - To grant read and write permissions 
#define ITERATION 10

pthread_mutex_t lock_mutex = PTHREAD_MUTEX_INITIALIZER;

int agent_s, smokerPaper_s, smokerTobacco_s, smokerMatches_s; 	/* semaphore variables 
																   hold - used to access
																   agent - puts the ingredients for the smokers
																   smoker1 - has paper
																   smoker2 - has tobacco
																   smoker3 - has matches
																*/

void *chooseIngredients();
void *smokePaper();
void *smokeTobacco();
void *smokeMatches();
int count = 0;
int randNum;

main() {
	pthread_t agent_t, smokerPaper_t, smokerTobacco_t, smokerMatches_t;
	int rc1, rc2, rc3, rc4;

	// Semaphore Creation
	if((agent_s=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
	{
		printf("\n can't create agent semaphore");
		exit(1);
	}

	if((smokerPaper_s=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
	{
		printf("\n can't create smokerPaper_s semaphore");
		exit(1);
	}

	if((smokerTobacco_s=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
	{
		printf("\n can't create smokerTobacco_s semaphore");
		exit(1);
	}

	if((smokerMatches_s=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
	{
		printf("\n can't create smokerMatches_s semaphore");
		exit(1);
	}

	// Initialize semaphore
	sem_create(agent_s, 1);
	sem_create(smokerPaper_s, 0);
	sem_create(smokerTobacco_s, 0);
	sem_create(smokerMatches_s, 0);
		
	// Create independent threads each of which will execute function
	if (rc1 = pthread_create(&agent_t, NULL, &chooseIngredients, NULL)) {
		printf("Thread creation failed: %d\n", rc1);
	}

	if (rc2 = pthread_create(&smokerPaper_t, NULL, &smokePaper, NULL)) {
		printf("Thread creation failed: %d\n", rc2);
	}

	if (rc3 = pthread_create(&smokerTobacco_t, NULL, &smokeTobacco, NULL)) {
		printf("Thread creation failed: %d\n", rc3);
	}

	if (rc4 = pthread_create(&smokerMatches_t, NULL, &smokeMatches, NULL)) {
		printf("Thread creation failed: %d\n", rc4);
	}

	// Wait til threads are complete before main continues
	pthread_join(agent_t, NULL);
	pthread_join(smokerPaper_t, NULL);
	pthread_join(smokerMatches_t, NULL);
	pthread_join(smokerTobacco_t, NULL);

	exit(EXIT_SUCCESS);
}

void *chooseIngredients(void *ptr) {
	
	while (1) {
		pthread_mutex_lock(&lock_mutex);
		randNum = rand()%3 + 1;
		if (randNum == 1) {
			// Put tobacco on table
			// Put paper on table
			V(smokerMatches_s); // Wake up smoker with match
		} else if (randNum == 2) {
			// Put tobacco on table
			// Put match on table
			V(smokerPaper_s); 	// Wake up smoker with paper
		} else {
			// Put match on table
			// Put paper on table
			V(smokerTobacco_s); // Wake up smoker with tobacco
		}
		pthread_mutex_unlock(&lock_mutex);
		P(agent_s);
	}
}

void *smokePaper(void *ptr) {
	while (1) {
		P(smokerPaper_s);
		pthread_mutex_lock(&lock_mutex);
		// Pick up tobacco
		// Pick up match
		V(agent_s);
		printf("Smoker with paper made a cigar\n");
		pthread_mutex_unlock(&lock_mutex);
	}
}

void *smokeTobacco(void *ptr) {
	while (1) {
		P(smokerTobacco_s);
		pthread_mutex_lock(&lock_mutex);
		// Pick up match
		// Pick up paper
		V(agent_s);
		printf("Smoker with tobacco made a cigar\n");
		pthread_mutex_unlock(&lock_mutex);
	}	
}

void *smokeMatches(void *ptr) {
	while (1) {
		P(smokerMatches_s);
		pthread_mutex_lock(&lock_mutex);
		// Pick up tobacco
		// Pick up paper
		V(agent_s);
		printf("Smoker with matches made a cigar\n");
		pthread_mutex_unlock(&lock_mutex);
	}
}
