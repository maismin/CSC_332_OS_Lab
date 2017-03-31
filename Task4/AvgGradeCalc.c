#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define LEN 256
#define ROW 10
#define COL 4

int main() {
    // Parent p
    pid_t man1, man2, work1, work2, work3, work4, m1, m2, w1, w2, w3, w4;
    int status;
	int grades[ROW][COL];

	// File descriptor
	FILE *fp;

	char *mode = "r";
	// Source name
	char *sName = "quiz_grades.txt";

	char buff[LEN];

	fp = fopen(sName,mode);

	int row = 0;

	while(row < ROW) {
		fscanf(fp, "%d %d %d %d", &grades[row][0], &grades[row][1], &grades[row][2], &grades[row][3]);
		row++;
	}

    if (man1 = fork()) {      // prevents child1 from forking
        if (man2 = fork()) {
            m1 = waitpid(man1, &status, 0); // parent process waits until child1 is done 
            m2 = waitpid(man2, &status, 0); // parent process waits until child2 is done 
//			printf("Manger 1 and Manager 2 Finished\n");
        }
        else {
			if (work1 = fork()) {
				if (work2 = fork()) {
					w1 = waitpid(work1, &status, 0); // parent process waits until child1 is done 
					w2 = waitpid(work2, &status, 0); // parent process waits until child2 is done 
//					printf("Manager 1 finished\n");
				}
				else  {
					// Worker1
					float avg_hw1 = 0;
					for (int i = 0; i < ROW; i++) {
						avg_hw1 += grades[i][0];
					}
					avg_hw1 /= ROW;
					printf("HW1 Avg: %f\n", avg_hw1);
				}
			}
			else  {
				// Worker2
				float avg_hw2 = 0;
				for (int i = 0; i < ROW; i++) {
					avg_hw2 += grades[i][1];
				}
				avg_hw2 /= ROW;
				printf("HW2 Avg: %f\n", avg_hw2);
			}
        }
    }
    else {
		if (work3 = fork()) {
			if (work4 = fork()) {
				w3 = waitpid(work3, &status, 0); // parent process waits until child1 is done 
				w4 = waitpid(work4, &status, 0); // parent process waits until child2 is done 
//				printf("Manager 2 finished\n");
			}
			else  {
				// Worker3
				float avg_hw3 = 0;
				for (int i = 0; i < ROW; i++) {
					avg_hw3 += grades[i][2];
				}
				avg_hw3 /= ROW;
				printf("HW3 Avg: %f\n", avg_hw3);
			}
		}
		else  {
			// Worker4
			float avg_hw4 = 0;
			for (int i = 0; i < ROW; i++) {
				avg_hw4 += grades[i][1];
			}
			avg_hw4 /= ROW;
			printf("HW4 Avg: %f\n", avg_hw4);
		}
    }
    return 0;
}
