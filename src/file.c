#include "../include/file.h"

extern struct logs log_info;

/* FILE/DIR parser function */
char* parse_file(const char* arg) {
    char* ret_file_path;

    if (access(arg, F_OK) != -1) {
        ret_file_path = strdup(arg);

    } else {
        /* exit error - cannot access error */
        fprintf(stderr, "xmod: cannot access '%s': %s\n", arg, strerror(errno));

        char mode_str[17] = "0000 (---------)";
        fprintf(stderr, "failed to change mode of '%s' from %s to %s\n",
                mode_str, mode_str, arg);

        if (log_info.available)
            proc_exit(getpid(), 1);
        exit(1);
    }

    return ret_file_path;
}

/* Appends to the current DIR the name of the child FILE/DIR */
char* process_node(const char* parent, const char* child) {
    bool append_sep = (parent[strlen(parent) - 1] == '/');
    char* new_node;

    if (append_sep) {
        if (asprintf(&new_node, "%s%s", parent, child) < 0)
            goto invalid_allocation;

    } else {
        if (asprintf(&new_node, "%s/%s", parent, child) < 0)
            goto invalid_allocation;
    }

    return new_node;

invalid_allocation:
    fprintf(stderr, "xmod: unable to process directory tree\n");

    if (log_info.available)
        proc_exit(getpid(), 1);
    exit(1);
}
