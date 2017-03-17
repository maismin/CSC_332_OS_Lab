#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	int child = fork();

	if (!child) {
		printf("Child was successfully forked, PID: %d\n", getpid());
		char *options = "-l";
		char * const opt = options;
		execvp("ls", &opt);
		printf("EXECVP Failed\n");
	}
	return 0;
}
