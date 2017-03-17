#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FN_LEN 2

int main () {
	int fd;

	const char *filename[FN_LEN];
	filename[0] = "destination1.txt";
	filename[1] = "destination2.txt";

	for (int i = 0; i < 2; i++) {
		fd = open(filename[i], O_RDONLY | O_CREAT, S_IRWXU);

		if (fd < 0) {
			perror("error");
			return 1;
		}
		else {
			close(fd);
		}
	}
	printf("files %s and %s were successfully created\n", filename[0], filename[1]);
	return 0;
}
