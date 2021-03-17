#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>
#include <wait.h>

#include "../headers/options.h"
#include "../headers/mode.h"
#include "../headers/file.h"
#include "../headers/signals.h"
#include "../headers/logs.h"

char* file_path; //Review

/* 
 * File struct that will containg LOG_FILENAME information if
 * the environment variable with the same name is set to a valid path.
 */
struct logs log_info;

/* Found Files and Modified Files counters */
uint32_t nftot = 1u;
uint32_t nfmod = 0u;

int main(int argc, char *argv[]) {
    /* LOGS Setup */
    logs_setup(&log_info, argc, argv);
    setup_signals();

    if (argc < 3) {
        /* exit error - invalid number of arguments */
        fprintf(stderr, "Usage: xmod [OPTIONS] MODE FILE/DIR\n");
        fprintf(stderr, "Usage: xmod [OPTIONS] OCTAL-MODE FILE/DIR\n");

        if (log_info.available)
            proc_exit(&log_info, getpid(), 1);

        exit(1);

    } else {

        struct options opt = { .changes = false, 
                               .recursive = false, 
                               .verbose = false 
                            };
        mode_t new_mode = 0u;
        bool mode_parsed = false;
        bool file_found = false;

        // int8_t file_index = -1;
        int8_t mode_index = -1;

        for (size_t i = argc - 1; i >= 1; --i) {
            if (argv[i][0] == '-' && argv[i][1] != 'r' && 
                argv[i][1] != 'w' && argv[i][1] != 'x') {
                /* [OPTIONS] input parsing */
                parse_options(&opt, argv[i]);

            } else if (!file_found) {
                /* FILE/DIR input parsing */
                file_path = parse_file(argv[i]);
                file_found = true;
                // file_index = i;

            } else if (!mode_parsed) {
                /* MODE input parsing */
                new_mode = parse_mode(argv[i], file_path);
                mode_parsed = true;
                mode_index = i;

            } else {
                /* exit error - invalid number of arguments */
                fprintf(stderr, "xmod: invalid argument\n");
                fprintf(stderr, "Usage: xmod [OPTIONS] MODE FILE/DIR\n");
                fprintf(stderr, "Usage: xmod [OPTIONS] OCTAL-MODE FILE/DIR\n");

                if (log_info.available)
                    proc_exit(&log_info, getpid(), 1);

                exit(1);
            }
        }

        /* output stuff */
        mode_t old_mode = get_current_file_mode(file_path);

        chmod(file_path, new_mode);

        if (log_info.available)
            file_modf(&log_info, file_path, old_mode, new_mode, getpid());

        options_output(&opt, file_path, &old_mode, &new_mode, false);

        nfmod = (new_mode == old_mode) ? nfmod : nfmod + 1;

        if (opt.recursive) {
            DIR* directory;
            struct stat stat_buf;
            struct dirent* dir;

            if ((directory = opendir(file_path)) != NULL) {
                while ((dir = readdir(directory)) != NULL) {
                    char* temp_file_path = process_node(file_path, dir->d_name);
                    stat(temp_file_path, &stat_buf);

                    if ((strcmp(dir->d_name, "..") != 0) &&
                        (strcmp(dir->d_name, ".") != 0)) {
                        new_mode = parse_mode(argv[mode_index], temp_file_path);
                        old_mode = get_current_file_mode(temp_file_path);

                        if (S_ISDIR(stat_buf.st_mode)) {
                            pid_t pid = fork();
                            
                            switch (pid) {
                                case 0:
                                    
                                    /* Child must not count parent's files */
                                    nftot = 1u;
                                    nfmod = 0u;

                                    /* 5 is the max allowed for OPTION args */
                                    char opt_str[5];
                                    get_options_str(&opt, opt_str);
                                    
                                    /* Program image Replacing */
                                    execl("./xmod", opt_str, argv[mode_index], 
                                         temp_file_path, NULL);
                                    break;

                                case -1:
                                    perror("Fork()");
                                    if (log_info.available)
                                        proc_exit(&log_info, getpid(), 1);
                                    exit(1);
                                
                                default: 
                                    wait(&pid);
                                    break;
                            }

                        } else {
                            nftot++;
                            chmod(temp_file_path, new_mode);

                            if (log_info.available)
                                file_modf(&log_info, temp_file_path, old_mode, new_mode, getpid());
                            options_output(&opt, temp_file_path, &old_mode, &new_mode, false);

                            nfmod = (new_mode == old_mode) ? nfmod : nfmod + 1;
                        }
                        
                    }
    
                    free(temp_file_path);
                }

                closedir(directory);

            } else {
                fprintf(stderr, "xmod: cannot read directory '%s': %s\n",
                        file_path, strerror(errno));
                options_output(&opt, file_path, &old_mode, &new_mode, true);
            }
        }

        free(file_path);
    }

    if (log_info.available)
        proc_exit(&log_info, getpid(), 0);
    return 0;
}
