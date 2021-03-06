#include <stdio.h>
#include <stdlib.h>

#define INVALID_NUMBER_ARGUMENTS -1

int main(int argc, char *argv[], char *envp[]) {

    if (argc != 3 && argc != 4) {
        fprintf(stderr, "Usage: xmod [OPTIONS] MODE FILE/DIR\n");
        fprintf(stderr, "Usage: xmod [OPTIONS] OCTAL-MODE FILE/DIR\n");
        exit(INVALID_NUMBER_ARGUMENTS);
    }

    printf("Hello World!\n");
    return 0;
}
