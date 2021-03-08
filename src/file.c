#include "../headers/file.h"

/* FILE/DIR parser function */
char* parse_file(const char* arg) {
    char* ret_file_path;

    if (access(arg, F_OK) != -1) {
        ret_file_path = (char*) malloc(sizeof(char) * (strlen(arg) + 1));
        strncpy(ret_file_path, arg, strlen(arg) + 1);

    } else {
        /* exit error - cannot access error */
        fprintf(stderr, "xmod: cannot access '%s': %s\n", 
                                    arg, strerror(errno));
        exit(1);
    }

    return ret_file_path;
}
