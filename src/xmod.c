#include <stdio.h>
#include <stdlib.h>

#include "../headers/options.h"
#include "../headers/mode.h"
#include "../headers/file.h"


int main(int argc, char *argv[], char *envp[]) {

    if (argc < 3) {
        /* exit error - invalid number of arguments */
        fprintf(stderr, "Usage: xmod [OPTIONS] MODE FILE/DIR\n");
        fprintf(stderr, "Usage: xmod [OPTIONS] OCTAL-MODE FILE/DIR\n");
        exit(1);

    } else {
        struct options opt = { .changes = false, 
                               .recursive = false, 
                               .verbose = false 
                            };
        mode_t new_mode = 0u;
        bool mode_parsed = false;
        bool file_found = false;
        char* file_path;

        for (size_t i = argc - 1; i >= 1; --i) {
            if (argv[i][0] == '-') {
                /* [OPTIONS] input parsing */
                parse_options(&opt, argv[i]);

            } else if (!file_found) {
                /* FILE/DIR input parsing */
                file_path = parse_file(argv[i]);
                file_found = true;

            } else if (!mode_parsed) {
                /* MODE input parsing */
                new_mode = parse_mode(argv[i], file_path);
                mode_parsed = true;

            } else {
                /* exit error - invalid number of arguments */
                fprintf(stderr, "xmod: invalid argument\n");
                fprintf(stderr, "Usage: xmod [OPTIONS] MODE FILE/DIR\n");
                fprintf(stderr, "Usage: xmod [OPTIONS] OCTAL-MODE FILE/DIR\n");
                exit(1);
            }
        }

        /* output stuff */
        mode_t old_mode = get_current_file_mode(file_path);
        options_output(&opt, file_path, &old_mode, &new_mode);

        chmod(file_path, new_mode);
        free(file_path);
    }

    return 0;
}
