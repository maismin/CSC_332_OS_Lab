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
            w1 = waitpid(child1, &status, 0);
            w2 = waitpid(child2, &status, 0);
//            printf("w1 is %d, %w2 is %d\n", w1, w2);
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
