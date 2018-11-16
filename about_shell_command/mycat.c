#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "utility.h"

#define BUF_SIZE 300

void main(int argc, char *argv[])
{
    int files[argc - 1];
    int i;
    int rbyte;
    int wflag;
    int cflag;
    argv++;
    char buf[BUF_SIZE];

    // Open files
    for (i = 0; i < argc - 1; i++) {
        file_open_sys(files[i], argv[i], O_RDONLY, 100);
    }

    for (i = 0; i <argc - 1; i++) {
        rbyte = 0;
        while ((rbyte = read(files[i], buf, sizeof(buf))) != 0) {
            if ((wflag = write(1, buf, rbyte)) == -1) {
                perror("Cannot print!");
                exit(200);
            }
            memset(buf, 0, sizeof(buf));
        }
    }

    for (i = 0; i < argc - 1; i++) {
        if ((cflag = close(files[i])) == -1) {
            perror("Cannot close.");
            exit(300);
        }
    }
}
