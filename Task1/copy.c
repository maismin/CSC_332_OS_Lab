#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFF_LENGTH 128

int main(int argc, char *argv[]) {
    /* fd holds the file descriptor of the source file
     * wd holds the file descriptor of the destination file
     */
    int fd, wd;

   /* checks if there are exactly 2 arguments: the source and destination file
    * otherwise function ends
    */ 
    if (argc != 3) {
        printf("Usage: ./copy SOURCE DEST\n");
        return 0;
    }

    char *sName = argv[1];
    char *dName = argv[2];

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
        // Opens destination file for writing, creates it if it doesn't exist
        wd = open(dName, O_WRONLY | O_CREAT, S_IRWXU);

        char buff[BUFF_LENGTH];

        ssize_t length;

        while ((length = read(fd, buff, BUFF_LENGTH)) != 0) {
            write(wd, buff, length);
        }

        close(fd);
        close(wd);
        printf("%s successfully copied to %s\n", sName, dName);
    }
}
