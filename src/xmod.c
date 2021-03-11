#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <wait.h>

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
        mode_t new_mode = 00u;
        bool mode_parsed = false;
        bool file_found = false;
        char* file_path;
        int file_index=-1;
        int mode_index=-1;

        for (size_t i = argc - 1; i >= 1; --i) {
            if (argv[i][0] == '-' && argv[i][1] != 'r' && argv[i][1] != 'w' && argv[i][1] != 'x') {
                /* [OPTIONS] input parsing */
                parse_options(&opt, argv[i]);

            } else if (!file_found) {
                /* FILE/DIR input parsing */
                file_path = parse_file(argv[i]);
                file_found = true;
                file_index=i;

            } else if (!mode_parsed) {
                /* MODE input parsing */
                new_mode = parse_mode(argv[i], file_path);
                mode_parsed = true;
                mode_index=i;

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
        chmod(file_path, new_mode);
        options_output(&opt, file_path, &old_mode, &new_mode);

        if(opt.recursive){
            DIR* directory;
            struct stat stat_buf;
            struct dirent *dir;
            if((directory=opendir(file_path))!=NULL){
                while ((dir=readdir(directory))!=NULL)
                {
                    char * temp_file_path=NULL;
                    asprintf(&temp_file_path, "%s/%s",file_path,dir->d_name);
                    stat(temp_file_path,&stat_buf);

                    if((strcmp(dir->d_name,"..")!=0) && (strcmp(dir->d_name,".")!=0)){
                        new_mode=parse_mode(argv[mode_index], temp_file_path);
                        
                        old_mode = get_current_file_mode(temp_file_path);
                        if(S_ISDIR(stat_buf.st_mode)){
                            int pid=fork();
                            switch (pid)
                            {
                                case 0:{
                                    asprintf(&argv[file_index],"%s", temp_file_path);
                                    execv("./xmod",argv);
                                    break;
                                }
                                case -1:
                                    perror("Fork()");
                                    exit(1);
                                
                                default:{
                                    wait(&pid);
                                    break;
                                }
                            }
                        }
                        else {
                            chmod(temp_file_path, new_mode);
                            options_output(&opt, temp_file_path, &old_mode, &new_mode);
                        }
                        
                    }
                }
            }
            closedir(directory);
        }

        free(file_path);
        
    }

    

    return 0;
}
