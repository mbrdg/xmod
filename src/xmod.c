#include <stdio.h>
#include <stdlib.h>

#include "../headers/options.h"
#include "../headers/mode.h"
#include "../headers/file.h"


int main(int argc, char *argv[], char *envp[]) {

    if (argc < 3) {
        fprintf(stderr, "Usage: xmod [OPTIONS] MODE FILE/DIR\n");
        fprintf(stderr, "Usage: xmod [OPTIONS] OCTAL-MODE FILE/DIR\n");
        exit(-1);

    } else {
        struct options opt = { .changes = false, 
                               .recursive = false, 
                               .verbose = false 
                            };
        mode_t mode = 0u;
        bool mode_parsed = false;
        bool file_found = false;
        char* file_path;

        for (size_t i = argc - 1; i >= 1; i--) {
            if(argv[i][0] == '-') {
                parse_options(&opt, argv[i]);

            } else if(!file_found) {
                file_path = parse_file(argv[i]);
                file_found = true;

            } else if (!mode_parsed) {
                mode = parse_mode(argv[i], file_path);
                mode_parsed = true;

            } else {
                fprintf(stderr, "Something"); //Alterar texto
                exit(1);  //Invalid input
            }
        }

        chmod(file_path, mode);
        free(file_path);
    }

    return 0;
}
