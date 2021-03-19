#include "../include/options.h"

extern struct logs log_info;

/* [Options] parser function */
int parse_options(struct options* opt, char* arg) {
    if (strlen(arg) == 1) {
        /* exit error - invalid option */
        fprintf(stderr, "xmod: invalid option -- '%c'\n", arg[0]);

        if (log_info.available)
            proc_exit(getpid(), 1);
        exit(1);
    }

    for (size_t i = strlen(arg)-1; i >= 1; --i) {
        switch (arg[i]) {
            case 'R':
                opt->recursive = true;
                break;
            case 'v':
                if (!opt->verbose && !opt->changes)
                    opt->verbose = true;
                break;
            case 'c':
                if (!opt->verbose && !opt->changes)
                    opt->changes = true;
                break;
            default:
                /* exit error - invalid option */
                fprintf(stderr, "xmod: invalid option -- '%c'\n", arg[i]);

                if (log_info.available)
                    proc_exit(getpid(), 1);
                exit(1);
        }
    }

    return 0;
}

/**
 * @brief Sets the string passed as argument 
 *        with the according file permissions
 * 
 * @param mode input MODE whose corresponding premissions string will be filled
 * @param str string to be changed according the mask passed.
 */
static void mode_to_str(const mode_t* mode, char* str) {
    str[0] = ((*mode) & S_IRUSR) ? 'r' : '-';
    str[1] = ((*mode) & S_IWUSR) ? 'w' : '-';
    str[2] = ((*mode) & S_IXUSR) ? 'x' : '-';
    str[3] = ((*mode) & S_IRGRP) ? 'r' : '-';
    str[4] = ((*mode) & S_IWGRP) ? 'w' : '-';
    str[5] = ((*mode) & S_IXGRP) ? 'x' : '-';
    str[6] = ((*mode) & S_IROTH) ? 'r' : '-';
    str[7] = ((*mode) & S_IWOTH) ? 'w' : '-';
    str[8] = ((*mode) & S_IXOTH) ? 'x' : '-';
    str[9] = '\0';
}

/* Prints the output according to the input */
void options_output(const struct options* opt, const char* file_path,
                    mode_t* old_mode, mode_t* new_mode, bool failure) {
    /* output according to chmod only requires the last 4 digits of MODE */
    (*new_mode) = (*new_mode) & 0777;
    (*old_mode) = (*old_mode) & 0777;

    char curr_mode[10];
    char pret_mode[10];

    mode_to_str(old_mode, curr_mode);
    mode_to_str(new_mode, pret_mode);

    if (!failure) {
        /* some change was detected */
        if ((*old_mode) != (*new_mode)) {
            if (opt->changes || opt->verbose) {
                fprintf(stdout,
                        "mode of '%s' changed from %04o (%s) to %04o (%s)\n",
                    file_path, (*old_mode), curr_mode, (*new_mode), pret_mode);
            }

        } else {
            if (opt->verbose) {
                fprintf(stdout, "mode of '%s' retained as %04o (%s)\n",
                        file_path, (*old_mode), curr_mode);
            }
        }

    } else {
        fprintf(stderr, "xmod: cannot read directory '%s': %s\n",
                file_path, strerror(errno));

        if (opt->verbose) {
                fprintf(stderr,
                "failed to change mode of '%s' from %04o (%s) to %04o (%s)\n",
                    file_path, (*old_mode), curr_mode, (*new_mode), pret_mode);
        }
    }
}

/* Getter for OPTIONS string */
void get_options_str(const struct options* opt, char* arg) {
    arg[0] = '-';
    arg[1] = '\0';

    if (opt->changes) strncat(arg, "c", 2);
    if (opt->verbose) strncat(arg, "v", 2);
    if (opt->recursive) strncat(arg, "R", 2);
}
