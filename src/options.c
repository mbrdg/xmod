#include "../headers/options.h"

/* [Options] parser function */
int parse_options(struct options* opt, char* arg) {
    for (size_t i = 1; i < strlen(arg); ++i) {
        switch (arg[i]) {
            case 'R':
                opt->recursive = true;
                break;
            case 'v':
                opt->verbose = true;
                opt->changes = false;
                break;
            case 'c':
                opt->verbose = false;
                opt->changes = true;
                break;
            
            default:
                /* exit error - invalid option */
                fprintf(stderr, "xmod: invalid option -- '%s'\n", arg);
                exit(1);
        }
    }

    return 0;
}
