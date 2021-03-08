#include <stdio.h>
#include <stdlib.h>
#include "../headers/options.h"
#include "../headers/mode.h"
#include "../headers/file.h"

#define INVALID_NUMBER_ARGUMENTS -1

int main(int argc, char *argv[], char *envp[]) {

    if (argc < 3) {
        fprintf(stderr, "Usage: xmod [OPTIONS] MODE FILE/DIR\n");
        fprintf(stderr, "Usage: xmod [OPTIONS] OCTAL-MODE FILE/DIR\n");
        exit(INVALID_NUMBER_ARGUMENTS);
    } else {
        options_t options = { .changes = false, .recursive = false, .verbose = false };
        mode_result_t mode;
        bool mode_parsed = false;
        bool file_found = false;
        char* file_path;
        for (size_t i = 1; i < argc; i++)
        {
            if(argv[i][0] == '-'){
                parse_options(&options,argv[i]);
            } else if ( !mode_parsed ) {
                mode = parse_mode(argv[i]);
                mode_parsed=true;
            } else if(!file_found){
                file_path=parse_file(argv[i]);
                file_found=true;
            } else{
                fprintf(stderr, "Something"); //Alterar texto
                exit(1);  //Invalid input
            }
        }
        //Ateçao a options
        if(!mode.is_octal){
            update_mode(file_path,&mode);
        }
        //NAO DESCOMENTAR
        //chmod(file_path,mode.mode);
    }

    //printf("Hello World!\n");
    return 0;
}
