#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
    // Parent p
    pid_t child1, child2, w1, w2;
    int status;

    if (child1 = fork()) {      // prevents child1 from forking
        if (child2 = fork()) {
            w1 = waitpid(child1, &status, 0); // parent process waits until child1 is done 
            w2 = waitpid(child2, &status, 0); // parent process waits until child2 is done 
			printf("Prcs_P1 and PrcsP2 finished executing\n");
        }
        else {
			execl("./Prcs_P1", 0);
			printf("Prcs_P1 failed to execute\n");
        }
    }
    else {
		execl("./Prcs_P2", 0);
		printf("Prcs_P2 failed to execute\n");
    }
            
    return 0;
}
