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
                fprintf( stderr, "xmod: Invalid Option [c,R,v]\n");
                exit(1); //Exit error (invalid option)
        }
    }

    return 0;
}
