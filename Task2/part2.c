#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
            
    // parent P
    int a = 10, b = 25, fq = 0, fr = 0;
    
    fq = fork();    // fork a child - call it Process Q
    
    if (fq == 0) {  // Child successfully forked
        a = a + b;     
        printf("Process Q p1 | a: %d, b: %d, pid: %d\n", a, b, getpid());

        fr = fork();// fork another child - call it Process R
        if (fr != 0) {
            b = b + 20;
            printf("Process Q p2 | a: %d, b: %d, pid: %d\n", a, b, getpid());
        }
        else {
            a = (a * b) + 30;
            printf("Process R | a: %d, b: %d, pid: %d\n", a, b, getpid());
        }
    } 
    else {
        b = a + b - 5;
        printf("Process P | a: %d, b: %d, pid: %d\n", a, b, getpid());
    }
    
    return 0;
}
