#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include "sem.h"

#define CHILD      			0  			/* Return value of child proc from fork call */
#define TRUE       			0  
#define FALSE      			1
#define PERMS 0666 //0666 - To grant read and write permissions 
#define BUFFSIZE 1
#define ITERATION 10

int *buffer;
int randNum;
int lock, agent, smoker1, smoker2, smoker3; 	/* semaphore variables 
												   hold - used to access
												   agent - puts the ingredients for the smokers
												   smoker1 - has paper
												   smoker2 - has tobacco
												   smoker3 - has matches
												*/
main()
{
	int pid;						// Process ID after fork call
	int status;						// Exit status of child process
	int shmid;

	// Semaphore Creation
	if((lock=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
	{
		printf("\n can't create lock semaphore");
		exit(1);
	}

	if((agent=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
	{
		printf("\n can't create agent semaphore");
		exit(1);
	}

	if((smoker1=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
	{
		printf("\n can't create smoker1 semaphore");
		exit(1);
	}

	if((smoker2=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
	{
		printf("\n can't create smoker2 semaphore");
		exit(1);
	}

	if((smoker3=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
	{
		printf("\n can't create smoker3 semaphore");
		exit(1);
	}

	// Initialize semaphore
	sem_create(lock, 1);
	sem_create(agent, 1);
	sem_create(smoker1, 0);
	sem_create(smoker2, 0);
	sem_create(smoker3, 0);

	// For keeping track of number of iterations
	if((shmid=shmget(IPC_PRIVATE,BUFFSIZE,IPC_CREAT | PERMS)) < 0)
	{
		printf("\n unable to create shared memory\n shmid = %d\n", shmid);
		return;
	}
	if((buffer=(int*)shmat(shmid,(char*)0,0)) == (int*)-1)
	{
		printf("\n Shared memory allocation error\n");
		exit(1);
	}

	// Initialize Shared Memory
	// index 0 is the number of iterations to run
	for (int i = 0; i < BUFFSIZE; i++){
		*(buffer + i) = 0;
//		printf("Buffer[%d]: %d\n", i, buffer[i]);
	}
	
	//Create child processes that will do the updates
	if ((pid = fork()) == -1) 
	{
		//fork failed!
		perror("fork");
		exit(1);
	}
	
	if (pid == CHILD){
		//First Child Process. Agent Process chooses a smoker to wake up.
		printf("Agent Pid: %d\n",getpid());
		while (buffer[0] < ITERATION) {
			P(lock);
//			printf("ITERATION IS: %d\n", buffer[0]);
			randNum = rand()%3;
			if (randNum == 0) {
				// Put tobacco on table
				// Put matches on table
				V(smoker1); 	// wake up smoker with paper
			} else if (randNum == 1) {
				// Put paper on table
				// Put matches on table
				V(smoker2); 	// wake up smoker with tobacco
			} else {
				// Put paper on table
				// Put tobacco on table
				V(smoker3); 	// wake up smoker with matches
			}
			buffer[0]++;
			V(lock);
			P(agent);
		}
	}
	
	else
	{
		//Parent Process. Fork off another child process.
		if ((pid = fork()) == -1)
		{
			//Fork failed!
			perror("fork");
			exit(1);
		}
		if (pid == CHILD)
		{
			printf("Smoker1 Pid: %d\n",getpid());
			while (buffer[0] < ITERATION) {
				//Second child process. Smoker1 has the paper.
				P(smoker1);
				P(lock);
				// Pick up tobacco
				// Pick up matches
				// Smoke cigarette
				printf("Smoker1 has smoked a cigar!\n");
				V(agent);
				V(lock);
			}
		}
		else
		{
		//Parent Process. Fork off another child process.
			if ((pid = fork()) == -1) 
			{
				//fork failed!
				perror("fork");
				exit(1);
			}
			if (pid == CHILD)
			{
				printf("Smoker2's Pid: %d\n",getpid());
				while (buffer[0] < ITERATION) {
					//Third child process. Smoker2 has the tobacco.
					P(smoker2);
					P(lock);
					// Pick up paper
					// Pick up matches
					// Smoke cigarette
					printf("Smoker2 has smoked a cigar!\n");
					V(agent);
					V(lock);
				}
			}
			else
			{
				//Parent Process. Fork off another child process.
				if ((pid = fork()) == -1) 
				{
					//fork failed!
					perror("fork");
					exit(1);
				}
				if (pid == CHILD)
				{
					printf("Smoker3's Pid: %d\n",getpid());
					while (buffer[0] < ITERATION) {
						//Fourth child process. Smoker3 has the matches.
						P(smoker3);
						P(lock);
						// Pick up paper
						// Pick up tobacco
						// Smoke cigarette
						printf("Smoker3 has smoked a cigar!\n");
						V(agent);
						V(lock);
					}
				}
				else
				{
					//Now parent process waits for the child processes to finish

//					pid = wait(&status);
//					printf("Process(pid = %d) exited with the status %d. \n", pid, status);
				
					pid = wait(&status);
					printf("Process(pid = %d) exited with the status %d. \n", pid, status);
				
					pid = wait(&status);
					printf("Process(pid = %d) exited with the status %d. \n", pid, status);

					pid = wait(&status);
					printf("Process(pid = %d) exited with the status %d. \n", pid, status);
				}
				exit(0);
			}
			exit(0);
		}
		exit(0);
	}
	exit(0);
}
