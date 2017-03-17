#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DEST1_LEN 100
#define DEST2_LEN 50

int main() {
	int fd, wd1, wd2;

	char *sName = "source.txt";
	char *dName1 = "destination1.txt";
	char *dName2 = "destination2.txt";

	fd = open(sName, O_RDONLY);
   	if (fd < 0) {
        switch(errno) {
            case ENOENT:                    // no such file or directory
            case EACCES: perror("error");   // permission denied
                         break;
            default:     perror("Error");
        }
    }
    else {
        // Opens destination file for writing
        wd1 = open(dName1, O_WRONLY);
        wd2 = open(dName2, O_WRONLY);

        char buff[DEST1_LEN+1];

        size_t length, i;

		do {
			// Read 100 chars from source.txt, change 1s to As and write to destination1.txt
			if ((length = read(fd, buff, DEST1_LEN)) != 0) {
				for (i = 0; i < length; i++) {
					if (buff[i] == '1') {
						buff[i] = 'A';
					}
				}
				write(wd1, buff, length);
			}

			// Read 50 chars from source.txt, change 2s to As and write to destination2.txt
			if ((length = read(fd, buff, DEST2_LEN)) != 0) {
				for (i = 0; i < length; i++) {
					if (buff[i] == '2') {
						buff[i] = 'B';
					}
				}
				write(wd2, buff, length);
			}
		}
		while (length != 0);

        close(fd);
        close(wd1);
        close(wd2);
        printf("%s successfully copied to %s and %s\n", sName, dName1, dName2);
    }
	return 0;
}
