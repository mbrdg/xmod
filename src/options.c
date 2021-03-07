#include "../headers/options.h"

int parse_options(options_t* options, char arg[]){

    for (size_t i = 1; i < strlen(arg); i++)
    {
        switch (arg[i])
        {
        case 'R':
            options->recursive=true;
            break;
        case 'v':
            options->verbose=true;
            options->changes=false;
            break;
        case 'c':
            options->verbose=false;
            options->changes=true;
            break;
        
        default:
            fprintf( stderr, "xmod: Invalid Option [c,R,v]\n");
            exit(1); //Exit error (invalid option)
        }
    }
    return 0;
}

