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
            printf("I am the parent\n");
        }
        else {
            printf("I am child two, my pid is: %d\n", getpid()); 
        }
    }
    else {
        printf("I am child one, my pid is: %d\n", getpid());
    }
            
    return 0;
}
