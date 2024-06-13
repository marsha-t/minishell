#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {

	char *filename = "/workspaces/minishell/explore/sym.txt";
    struct stat fileStat;

    // Get file status
    if (stat(filename, &fileStat) == -1) {
        perror("Error getting file status");
        return 1;
    }

    // Check file type
    if (S_ISLNK(fileStat.st_mode)) {
        printf("%s is a symbolic link.\n", filename);
    }
    else if (S_ISREG(fileStat.st_mode)) {
        printf("%s is a regular file.\n", filename);
    } else if (S_ISDIR(fileStat.st_mode)) {
        printf("%s is a directory.\n", filename);
    } else if (S_ISCHR(fileStat.st_mode)) {
        printf("%s is a character special file.\n", filename);
    } else if (S_ISBLK(fileStat.st_mode)) {
        printf("%s is a block special file.\n", filename);
    } else if (S_ISFIFO(fileStat.st_mode)) {
        printf("%s is a FIFO (named pipe).\n", filename);
    } else if (S_ISLNK(fileStat.st_mode)) {
        printf("%s is a symbolic link.\n", filename);
    } else if (S_ISSOCK(fileStat.st_mode)) {
        printf("%s is a socket.\n", filename);
    } else {
        printf("%s is of unknown type.\n", filename);
    }

    return 0;
}
