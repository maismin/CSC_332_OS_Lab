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
#define M 5 				// Max attempts by any process to access critical section before printing out
#define BUFFSIZE 3

int *buffer;
FILE *fp1, *fp2, *fp3, *fp4;			/* File Pointers */
int mutexBalance, readAttempt; 		/* semaphore variables 
								   	   mutexBalance - for withrdawing and depositing account
									   readAttempt - for children reading number of attempts */

main()
{
	int pid;						// Process ID after fork call
	int i;							// Loop index
	int N;							// Number of times dad does update
	int N_Att;						// Number of time sons allowed to do update
	int status;						// Exit status of child process
	int bal1, bal2;					// Balance read by processes
	int flag, flag1;				// End of loop variables
	int shmid;

	// Semaphore Creation
	if((mutexBalance=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
	{
		printf("\n can't create mutexBalance semaphore");
		exit(1);
	}

	if((readAttempt=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
	{
		printf("\n can't create readAttempt semaphore");
		exit(1);
	}

	// Initialize semaphore
	sem_create(mutexBalance, 1);
	sem_create(readAttempt, 1);

	// For keeping track of T(P)
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
	// Index 0 = Dad, 1 = Son1, 2 = Son2
	for (int i = 0; i < BUFFSIZE; i++){
		*(buffer + i) = 0;
		printf("Buffer[%d]: %d\n", i, buffer[i]);
	}
	
	//Initialize the file balance to be $100
	fp1 = fopen("balance","w");
	bal1 = 100;
	fprintf(fp1, "%d\n", bal1);
	fclose(fp1);
	
	//Initialize the number of attempts to be 20
	fp4 = fopen("attempt", "w");
	N_Att = 20;
	fprintf(fp4, "%d\n", N_Att);
	fclose(fp4);
	
	//Create child processes that will do the updates
	if ((pid = fork()) == -1) 
	{
		//fork failed!
		perror("fork");
		exit(1);
	}
	
	if (pid == CHILD){
	//First Child Process. Dear old dad tries to do some updates.
	
		N=5;
		for(i=1;i<=N; i++)
		{
			P(mutexBalance);
			buffer[1]++;
			buffer[2]++;
			if (buffer[0] >= M) {
				printf("Dad waited %d or more turn(s) before going into CS\n", M);
				buffer[0] = 0;
			}
			printf("Dad: %d | Son1: %d | Son2: %d\n", buffer[0], buffer[1], buffer[2]);
			printf("Dear old dad is trying to do update.\n");
			fp1 = fopen("balance", "r+");
			fscanf(fp1, "%d", &bal2);
			printf("Dear old dad reads balance = %d \n", bal2);
			
			//Dad has to think (0-14 sec) if his son is really worth it
			sleep(rand()%15);
			fseek(fp1,0L,0);
			bal2 += 60;
			printf("Dear old dad writes new balance = %d \n", bal2);
			fprintf(fp1, "%d \n", bal2);
			fclose(fp1);
			printf("Dear old dad is done doing update. \n");
			V(mutexBalance);
			sleep(rand()%5);	/* Go have coffee for 0-4 sec. */
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
			printf("First Son's Pid: %d\n",getpid());
			//Second child process. First poor son tries to do updates.
			flag = FALSE;
			while(flag == FALSE) 
			{
				P(readAttempt);
				fp3 = fopen("attempt" , "r+");
				fscanf(fp3, "%d", &N_Att);
				if(N_Att == 0)
				{
					fclose(fp3);
					flag = TRUE;
					V(readAttempt);
				}
				else
				{
					P(mutexBalance);
					buffer[0]++;
					buffer[2]++;
					if (buffer[1] >= M) {
						printf("Son1 waited %d or more turn(s) before going into CS\n", M);
						buffer[1] = 0;
					}
					printf("Dad: %d | Son1: %d | Son2: %d\n", buffer[0], buffer[1], buffer[2]);
					printf("Poor SON_1 wants to withdraw money.\n");
					fp2 = fopen("balance", "r+");
					fscanf(fp2,"%d", &bal2);
					printf("Poor SON_1 reads balance. Available Balance: %d \n", bal2);
					if (bal2 == 0)
					{
						fclose(fp2);
						fclose(fp3);
						V(mutexBalance);
					}
					else
					{
						sleep(rand()%5);
						fseek(fp2,0L, 0);
						bal2 -=20;
						printf("Poor SON_1 write new balance: %d \n", bal2);
						fprintf(fp2,"%d\n", bal2);
						fclose(fp2);
						printf("poor SON_1 done doing update.\n");
						V(mutexBalance);
						fseek(fp3,0L, 0);
						N_Att -=1;
						printf("Attempt: %d\n", N_Att);
						fprintf(fp3, "%d\n", N_Att);
						fclose(fp3);
						V(readAttempt);
					}
				}
			}
		}
		else
		{
		//Parent Process. Fork off one more child process.
			if ((pid = fork()) == -1) 
			{
				//fork failed!
				perror("fork");
				exit(1);
			}
			if (pid == CHILD)
			{
				printf("Second Son's Pid: %d\n",getpid());
				//Third child process. Second poor son tries to do updates.
				flag1 = FALSE;
				while(flag1 == FALSE) 
				{
					P(readAttempt);
					fp3 = fopen("attempt" , "r+");
					fscanf(fp3, "%d", &N_Att);
					if(N_Att == 0)
					{
						fclose(fp3);
						flag1 = TRUE;
						V(readAttempt);
					}
					else
					{
						P(mutexBalance);
						buffer[0]++;
						buffer[1]++;
						if (buffer[2] >= M) {
							printf("Son2 waited %d or more turn(s) before going into CS\n", M);
							buffer[2] = 0;
						}
						printf("Dad: %d | Son1: %d | Son2: %d\n", buffer[0], buffer[1], buffer[2]);
						printf("Poor SON_2 wants to withdraw money.\n");
						fp2 = fopen("balance", "r+");
						fscanf(fp2,"%d", &bal2);
						printf("Poor SON_2 reads balance. Available Balance: %d \n", bal2);
						if (bal2 == 0)
						{
							fclose(fp2);
							fclose(fp3);
							V(mutexBalance);
						}
						else
						{
							sleep(rand()%5);
							fseek(fp2,0L, 0);
							bal2 -=20;
							printf("Poor SON_2 write new balance: %d \n", bal2);
							fprintf(fp2,"%d\n", bal2);
							fclose(fp2);

							printf("poor SON_2 done doing update.\n");
							V(mutexBalance);
							fseek(fp3,0L, 0);
							N_Att -=1;
							printf("Attempt: %d\n", N_Att);
							fprintf(fp3, "%d\n", N_Att);
							fclose(fp3);
							V(readAttempt);
						}
					}
				}
			}
			else
			{
				//Now parent process waits for the child processes to finish
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
