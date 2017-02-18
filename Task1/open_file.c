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

    fd = open(filename, O_RDONLY | O_CREAT, S_IRWXU);

    if (fd < 0) {
        perror("error");
        return 1;
    }
    else {
        printf("\nOpen() Successful\n");
        close(fd);
        printf("file is now closed\n");
    }
    return 0;
}
