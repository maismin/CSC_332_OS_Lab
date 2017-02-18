#include <stdio.h>
#include <string.h>
#include <errno.h>
// for open command
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>     // for read/write/close syscall

#define BUFF_LENGTH 128

int main (int argc, char* argv[]) {
    int fd;
    
    char *filename = argv[1];

    fd = open(filename, O_RDONLY);

    if (fd < 0) { 
        perror("open");
        return 1;
    }   
    
    char buff[BUFF_LENGTH];

    ssize_t length;

    while ((length = read(fd, buff, BUFF_LENGTH)) != 0) {
        write(1, buff, length);
    }

    close(fd);

    return 0;
}
