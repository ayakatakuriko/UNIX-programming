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
    if (argc < 2) {
        fprintf(stderr, "Please input argument.\n");
        fprintf(stderr, "Usage: mycp <src> <dst>\n");
        exit(100);
    }
    if (argc > 3) {
        fprintf(stderr, "Too many argument\n");
        fprintf(stderr, "Usage: mycp <src> <dst>\n");
        exit(200);
    }

    int src, dst;
    int rbyte = 0;
    int wflag;
    int cflag;
    char decide;
    argv++;
    char buf[BUF_SIZE];

    //open dst file
    file_open_sys(src, argv[0], O_RDONLY, 300);

    if ((dst = open(argv[1], O_WRONLY|O_CREAT|O_EXCL, 0644)) < 0) {
        if (errno != EEXIST) {
            perror("open");
            exit(1);
        }
        // File already exists.
        do {
            // ask overwrite or not.
            fprintf(stderr, "overwrite (y/n)?: ");
            fscanf(stdin, "%c", &decide);
            if (decide == 'y')
                break;
            else if (decide == 'n') {
                fprintf(stdout, "Abold\n");
                return;
            }
            else {
                fprintf(stderr, "Please input y or n.\n");
            }
        } while (1);

        if ((dst = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0) {
            perror("open");
            exit(1);
        }
    }

    while ((rbyte = read(src, buf, sizeof(buf))) != 0) {
        if ((wflag = write(dst, buf, rbyte)) == -1) {
            perror("Cannot print!");
            exit(200);
        }
        memset(buf, 0, sizeof(buf));
    }

    if ((cflag = close(src)) == -1) {
        perror("Cannot close.");
        exit(300);
    }

    if ((cflag = close(dst)) == -1) {
        perror("Cannot close.");
        exit(300);
    }
}
