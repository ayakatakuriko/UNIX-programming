#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "utility.h"

#define NAME_LEN 200

void main(int argc, char *argv[])
{
    char crr_direct[NAME_LEN];
    DIR *dp;
    struct dirent *dr;
    struct stat fstat;
    int f_error;
    struct passwd *p;
    struct group *grp;

    // Get path of current directory
    getcwd(crr_direct, NAME_LEN);

    // Open current directory
    if ((dp = opendir(crr_direct)) == NULL) {
        perror("open directory");
        exit(19);
    }

    // Get directory entry
    while ((dr = readdir(dp)) != NULL) {
        if ((f_error = stat(dr->d_name, &fstat)) == -1) {
            perror("stat");
            exit(12);
        }
        //TODO
        struct tm *mtime;
        if ((p = getpwuid(fstat.st_uid)) == NULL) {
            perror("getpwuid()");
            exit(2);
        }
        if ((grp = getgrgid(fstat.st_gid)) == NULL) {
            perror("getgrgit()");
            exit(2);
        }

        if (S_ISDIR(fstat.st_mode))
            printf("d");
        else if (S_ISCHR(fstat.st_mode))
            printf("c");
        else if (S_ISLNK(fstat.st_mode))
            printf("s");
        else
            printf("-");
        printf((fstat.st_mode & S_IRUSR) ? "r" : "-");
        printf((fstat.st_mode & S_IWUSR) ? "w" : "-");
        printf((fstat.st_mode & S_IXUSR) ? "x" : "-");
        printf((fstat.st_mode & S_IRGRP) ? "r" : "-");
        printf((fstat.st_mode & S_IWGRP) ? "w" : "-");
        printf((fstat.st_mode & S_IXGRP) ? "x" : "-");
        printf((fstat.st_mode & S_IROTH) ? "r" : "-");
        printf((fstat.st_mode & S_IWOTH) ? "w" : "-");
        printf((fstat.st_mode & S_IXOTH) ? "x" : "-");
        printf("  %d", fstat.st_nlink);
        printf(" %s", p->pw_name);
        printf(" %s", grp->gr_name);
        printf(" %5d", fstat.st_size);
        mtime = localtime(&fstat.st_mtime);
        printf(" %2d月 %2d %2d:%2d", mtime->tm_mon + 1, mtime->tm_mday,
               mtime->tm_hour, mtime->tm_min);
        printf(" %s\n", dr->d_name);
    }
    closedir(dp);
}
