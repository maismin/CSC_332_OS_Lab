#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define INPUT_SIZE 1024
#define BUFF_SIZE 32

int main() { 
	pid_t child, wait;
	int status;

	char input[INPUT_SIZE];
	char *arg[BUFF_SIZE];

	char delimit[] = " \t\r\n\v\f";
	char *exit = "quit";

	size_t i;

	while(1) {
		i = 0;
		printf("[SHELL]$ ");
		if(fgets(input, sizeof(input), stdin)) {
			// check for new line char to prevent segmentation fault
			if (input[0] == '\n')
				continue;	
			arg[i]=strtok(input, delimit);
			while(arg[i] != NULL) {
//				printf("arg[%d] = %s\n", i, arg[i]);
				i++;
				arg[i]=strtok(NULL, delimit);
			}
		}
		if(strcmp(arg[0], exit) != 0) {
			child = fork();
			if (!child) {
				execvp(*arg, arg);
				printf("EXECVP Failed\n");
			}
			else {
				wait = waitpid(child, &status, 0);
			}
		}
		else
			break;
	}
	return 0;
}
