#include "../headers/file.h"

/* FILE/DIR parser function */
char* parse_file(const char* arg) {
    char* ret_file_path;

    if (access(arg, F_OK) != -1) {
        ret_file_path = strdup(arg);

    } else {
        /* exit error - cannot access error */
        fprintf(stderr, "xmod: cannot access '%s': %s\n",
                                    arg, strerror(errno));
        exit(1);
    }

    return ret_file_path;
}
