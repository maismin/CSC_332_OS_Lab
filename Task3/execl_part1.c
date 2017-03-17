#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	int child = fork();

	if (!child) {
		printf("Child was successfully forked, PID: %d\n", getpid());
		execl("/bin/date", "-u", 0);
		printf("EXECL Failed\n");
	}
	return 0;
}
