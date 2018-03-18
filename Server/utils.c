#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXBUFLEN 100000

struct PAYLOAD {
    int file_type;
    char message[MAXBUFLEN];
};

int get_file_type(char *ext) {
    if (strcmp(ext, "txt") == 0)
        return 0;
    if (strcmp(ext, "jpg") == 0)
        return 1;
    return -1;
}

char *get_file_ext(char *path) {

    size_t len = strlen(path);
    char *ext = (char *) malloc(sizeof(char) * 4);
    int i, j;

    j = 2;
    for (i = (int)len - 1; i >= 0; i--) {
        ext[j] = path[i];
        j--;
    }
    ext[3] = '\0';

    return ext;
}

size_t read_file(char *path, char *source) {
    char *cpStr = malloc(sizeof(char) * MAXBUFLEN);

    FILE *fd = fopen(path, "r");
    if (fd == 0) {
        printf("Error reading file from path: %s\n", path);
        return (size_t) -1;
    }

    size_t readLen = fread(cpStr, sizeof(char), MAXBUFLEN, fd);
    if (ferror(fd) != 0) {
        perror("Error reading file\n");
        return (size_t) -1;
    }

    cpStr[readLen++] = '\0';
    strcpy(source, cpStr);

    fclose(fd);

    return readLen;
}
