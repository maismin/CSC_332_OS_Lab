#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main (int argc, char* argv[]) {
    int fd;
    if (argc != 2) {
        printf("\n Usage: \n");
        return 1;
    }

    char* filename = argv[1];

    errno = 0;

    fd = open(filename, O_RDONLY | O_CREAT, S_IRWXU);

    if (fd < 0) {
        printf("Could not open %s\n", filename);
        perror("error");
        return 1;
    }
    else {
        printf("\nOpen() Successful\n");
        close(fd);
        printf("file is now closed\n");
        /* open() succeeded, now one can do read operations on the file opened since we opened it in read-only mode. Also once done with processing, the file needs to be closed. */
    }
    return 0;
}
