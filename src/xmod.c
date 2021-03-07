#include <stdio.h>
#include <stdlib.h>
#include "../headers/options.h"
#include "../headers/mode.h"

#define INVALID_NUMBER_ARGUMENTS -1

int main(int argc, char *argv[], char *envp[]) {

    if (argc != 3 && argc != 4) {
        fprintf(stderr, "Usage: xmod [OPTIONS] MODE FILE/DIR\n");
        fprintf(stderr, "Usage: xmod [OPTIONS] OCTAL-MODE FILE/DIR\n");
        exit(INVALID_NUMBER_ARGUMENTS);
    } else {
        options_t options = { .changes = false, .recursive = false, .verbose = false };
        mode_t mode = 0u;
        bool file_arg_found = false;
        for (size_t i = 1; i < argc; i++)
        {
            if(argv[i][0] == '-'){
                parse_options(&options,argv[i]);
            } else if ( !file_arg_found && mode == 0u) {
                mode = parse_mode(argv[i]);
            } else {

            }
        }
        
    }

    //printf("Hello World!\n");
    return 0;
}
