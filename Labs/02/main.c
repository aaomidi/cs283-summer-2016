#include <time.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/stat.h>
#include <unistd.h>
// My fast implementation of malloc, checks the output of malloc.
#define MY_MALLOC(ptr, count, lbl)\
do { \
 if (NULL == (ptr = malloc(count))) { \
    fprintf(stderr, "%s: Failed to allocate memory.\n",lbl); \
    exit(EXIT_FAILURE); \
 } \
} while(0)

/**
 * Opens the directory and does error management
 * @param path Path to directory.
 * @return DIR type.
 */
DIR *open_directory(char *path) {
    DIR *dir1 = opendir(path);
    if (dir1 == NULL) {
        fprintf(stderr, "Error: %s\nCode: %d\n", strerror(errno), errno);
        exit(EXIT_FAILURE);
    }
    return dir1;
}

/**
 * Makes a directory with a specific permission set.
 * @param path Path to make directory.
 */
void make_directory(char *path) {
    mkdir(path, 0770);
    if (errno > 0) {
        fprintf(stderr, "Error: %s\nCode: %d\n", strerror(errno), errno);
        exit(EXIT_FAILURE);
    }
}

/**
 * Appends a file dirent to a path.
 * @param oldPath Old path to append
 * @param appended the dirent
 * @return The appended char*
 */
char *getAppendedPath(char *oldPath, struct dirent *appended) {
    char *reconstructed;
    MY_MALLOC(reconstructed, sizeof(char) * 256, "mkdir reconstruct");
    bzero(reconstructed, 256);
    strcpy(reconstructed, oldPath);
    strcat(reconstructed, appended->d_name);
    if (appended->d_type == DT_DIR) {
        strcat(reconstructed, "/");
    }
    return reconstructed;
}

/**
 * Appends the path and calls make_directory.
 * @param where Where to make the path
 * @param toMake What we're making
 * @return the reconstructed path.
 */
char *makeDir(char *where, struct dirent *toMake) {
    char *reconstructed = getAppendedPath(where, toMake);
    printf("Making directory: %s\n\n", reconstructed);
    make_directory(reconstructed);

    return reconstructed;
}

/**
 * Makes a file
 * @param from source of file
 * @param where dest of file
 * @param toMake file dirent
 * @return reconstructed path for file
 */
char *makeFile(char *from, char *where, struct dirent *toMake) {
    char *reconstructed = getAppendedPath(where, toMake);
    printf("Making file: %s\n", reconstructed);

    FILE *src_fd, *dst_fd;
    char *buffer;

    src_fd = fopen(from, "r");
    dst_fd = fopen(reconstructed, "w");

    fseek(src_fd, 0, SEEK_END);
    long lSize = ftell(src_fd);
    rewind(src_fd);
    MY_MALLOC(buffer, sizeof(char) * lSize, "file");

    size_t result = fread(buffer, 1, (size_t) lSize, src_fd);
    fwrite(buffer, sizeof(char), result, dst_fd);

    free(buffer);
    fclose(src_fd);
    fclose(dst_fd);
    return reconstructed;
}

/**
 * Checks a directory to see if the dir/file exists.
 * @param dir The DIR to check.
 * @param check What to check.
 * @param path1 path of folder A
 * @param path2 path of folder B
 * @param allow allow the time based file changes.
 * @return result of the check
 */
int checkDir(DIR *dir, struct dirent *check, char *path1, char *path2, int allow) {
    struct dirent *in_file;
    int found = 0;
    while ((in_file = readdir(dir))) {
        if (!strcmp(in_file->d_name, ".")) {
            continue;
        }
        if (!strcmp(in_file->d_name, "..")) {
            continue;
        }

        if ((strcmp(check->d_name, in_file->d_name) == 0) && (check->d_type == in_file->d_type)) {
            found = 1;
            if (check->d_type == DT_REG && allow) { // If its a file.
                struct stat attr1;
                struct stat attr2;

                char *newPath1 = getAppendedPath(path1, check);
                char *newPath2 = getAppendedPath(path2, check);

                if ((stat(newPath1, &attr1)) < 0) {
                    fprintf(stderr, "Error reading stats1.");
                    exit(EXIT_FAILURE);
                }

                if ((stat(newPath2, &attr2)) < 0) {
                    fprintf(stderr, "Error reading stats2.");
                    exit(EXIT_FAILURE);
                }

                double d = difftime(attr1.st_mtime, attr2.st_mtime);
                char *str;
                if (d > 0) {
                    printf("Copying more recent file from A to B\n");
                    str = makeFile(newPath1, path2, check);
                } else {
                    printf("Copying more recent file from B to A\n");
                    str = makeFile(newPath2, path1, check);
                }
                free(str);
                free(newPath1);
                free(newPath2);
            }
            break;
        }
    }
    return found;
}

/**
 * Recursive directory removal.
 * @param path
 */
void removeDirectory(char *path) {
    DIR *dir1 = open_directory(path);
    struct dirent *in_file;

    while ((in_file = readdir(dir1))) {
        if (!strcmp(in_file->d_name, ".")) {
            continue;
        }
        if (!strcmp(in_file->d_name, "..")) {
            continue;
        }
        char *reconstruct = getAppendedPath(path, in_file);
        switch (in_file->d_type) {
            case DT_DIR: {
                removeDirectory(reconstruct);
                rmdir(reconstruct);
                break;
            }
            case DT_REG: {
                remove(reconstruct);
                break;
            }
            default:
                fprintf(stderr, "File type not supported.\n\tFile name: %s", in_file->d_name);
        }
    }
}

/**
 * Main syncing method. Has calls to all methods above.
 * @param path1 Dir A
 * @param path2 Dir B
 */
void syncFiles(char *path1, char *path2) {

    DIR *dir1 = open_directory(path1);
    DIR *dir2 = open_directory(path2);

    struct dirent *in_file;
    while ((in_file = readdir(dir1))) {
        rewinddir(dir2);

        // Ignores files that point to current and parent directories
        if (!strcmp(in_file->d_name, ".")) {
            continue;
        }
        if (!strcmp(in_file->d_name, "..")) {
            continue;
        }
        int res = checkDir(dir2, in_file, path1, path2, 1);
        switch (in_file->d_type) {
            case DT_DIR: {
                if (res == 0) {
                    char *newPath2 = makeDir(path2, in_file);
                    char *newPath1 = getAppendedPath(path1, in_file);
                    syncFiles(newPath1, newPath2);
                    free(newPath1);
                    free(newPath2);
                }
                break;
            }
            case DT_REG: {
                if (res == 0) {
                    char *newPath1 = getAppendedPath(path1, in_file);
                    char *newPath2 = makeFile(newPath1, path2, in_file);
                    free(newPath1);
                    free(newPath2);
                }

                break;
            }
            default:
                fprintf(stderr, "File type not supported.\n\tFile name: %s", in_file->d_name);
        }
    }
    rewinddir(dir2);
    rewinddir(dir1);
    printf("Started reverse traversal for deleting files.\n");
    while ((in_file = readdir(dir2))) {
        rewinddir(dir1);

        // Ignores files that point to current and parent directories
        if (!strcmp(in_file->d_name, ".")) {
            continue;
        }
        if (!strcmp(in_file->d_name, "..")) {
            continue;
        }
        int res = checkDir(dir1, in_file, path2, path1, 0);
        char *newPath2 = getAppendedPath(path2, in_file);

        switch (in_file->d_type) {
            case DT_DIR: {
                if (res == 0) {
                    removeDirectory(newPath2);
                    printf("Deleting directory from B: %s\n", newPath2);
                    rmdir(newPath2);
                }
                break;
            }
            case DT_REG: {
                if (res == 0) {
                    printf("Deleting file from B: %s\n", newPath2);
                    remove(newPath2);
                }
                break;

            }
            default:
                fprintf(stderr, "File type not supported.\n\tFile name: %s", in_file->d_name);
        }
        free(newPath2);
    }
    closedir(dir1);
    closedir(dir2);
    free(in_file);
}
/**
 * Main method of program.
 * @param argc number of args.
 * @param argv the actual args.
 * @return result of prog.
 */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Not enough arguments.\n");
        return 0;
    }
    char *path1 = argv[1];
    char *path2 = argv[2];

    printf("Starting the sync...\n");
    syncFiles(path1, path2);
    printf("\tFinished.\n");
    return 0;
}