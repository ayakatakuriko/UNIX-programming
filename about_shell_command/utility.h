#ifndef UTILITY_H
#define UTILITY_H


#define mem_alloc(ptr, type, size, err) \
    do { \
        if ((ptr = (type *)malloc(sizeof(type) * (size))) == NULL) { \
            perror("Error\n"); \
            exit(err); \
        } \
    } while (0)

#define file_open(fp, fname, mode, err) \
    do { \
        if ((fp = fopen(fname, mode)) == NULL) { \
            fprintf(stderr, "Error: Failed to open file: %s\n", fname); \
            exit(err); \
        } \
    } while (0)

#define file_open_sys(fd, path, flag, err) \
    do { \
        if ((fd = open(path, flag)) == -1) { \
            fprintf(stderr, "Error: Failed to open file: %s\n", path); \
            exit(err); \
        } \
    } while (0)

#endif
