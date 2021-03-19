#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#include "../include/options.h"
#include "../include/mode.h"
#include "../include/file.h"
#include "../include/signals.h"
#include "../include/logs.h"

/* GLOBALS */

/* 
 * File struct that will containg LOG_FILENAME information if
 * the environment variable with the same name is set to a valid path.
 */
struct logs log_info;
char* file_path;  /* Path to the current processed file */

uint32_t nftot = 1u;  /* No. Found Files */
uint32_t nfmod = 0u;  /* No. Modified Files */

/* Flag that ensures that the group leader hasn`t any child */
bool last_child_dead = false;

int main(int argc, char *argv[]) {
    /* LOGS Setup */
    logs_setup(argc, argv);
    setup_signals();

    if (argc < 3) {
        /* exit error - invalid number of arguments */
        fprintf(stderr, "Usage: xmod [OPTIONS] MODE FILE/DIR\n");
        fprintf(stderr, "Usage: xmod [OPTIONS] OCTAL-MODE FILE/DIR\n");

        if (log_info.available)
            proc_exit(getpid(), 1);

        exit(1);

    } else {
        struct options opt = { .changes = false,
                               .recursive = false,
                               .verbose = false
                            };
        mode_t new_mode = 0u;
        bool mode_parsed = false;
        bool file_found = false;

        int8_t md_ind = -1;

        for (size_t i = argc - 1; i >= 1; --i) {
            if (argv[i][0] == '-' && argv[i][1] != 'r' &&
                argv[i][1] != 'w' && argv[i][1] != 'x') {
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
                md_ind = i;

            } else {
                /* exit error - invalid number of arguments */
                fprintf(stderr, "xmod: invalid argument\n");
                fprintf(stderr, "Usage: xmod [OPTIONS] MODE FILE/DIR\n");
                fprintf(stderr, "Usage: xmod [OPTIONS] OCTAL-MODE FILE/DIR\n");

                if (log_info.available)
                    proc_exit(getpid(), 1);

                exit(1);
            }
        }

        /* 
         * Whenever a FILE/DIR is found, we assume - by looking at chmod's
         * output - that chmod changes the FILE/DIR before calling fork()
         * to handle sub-folders within the recursive approach.
        */
        mode_t old_mode = get_current_file_mode(file_path);
        chmod(file_path, new_mode);

        if (log_info.available)
            file_modf(file_path, old_mode, new_mode, getpid());
        options_output(&opt, file_path, &old_mode, &new_mode, false);

        nfmod = (new_mode == old_mode) ? nfmod : nfmod + 1;

        if (opt.recursive) {
            DIR* directory;
            struct dirent* dir;
            struct stat stat_buf;

            if ((directory = opendir(file_path)) != NULL) {
                while ((dir = readdir(directory)) != NULL) {
                    if ((strcmp(dir->d_name, "..") == 0) ||
                        (strcmp(dir->d_name, ".") == 0))
                            continue;

                    char* tmp_fl_path = process_node(file_path, dir->d_name);
                    lstat(tmp_fl_path, &stat_buf);

                    new_mode = parse_mode(argv[md_ind], tmp_fl_path);
                    old_mode = get_current_file_mode(tmp_fl_path);

                    if (S_ISDIR(stat_buf.st_mode)) {
                        pid_t pid = fork();

                        switch (pid) {
                            case -1:
                                if (log_info.available)
                                    proc_exit(getpid(), 1);
                                exit(1);

                            case 0:
                            /*
                             * Perhaps not the best practice but the new
                             * process must have its own directory so we
                             * assign file_path to point to the same string
                             * built by the parent and that is a sub-folder
                             */  
                                file_path = tmp_fl_path;

                            /* Child must not count parent's files */
                                nftot = 1u;
                                nfmod = 0u;

                            /* 5 is the max allowed for OPTION args */
                                char opt_str[5];
                                get_options_str(&opt, opt_str);

                           if (log_info.available) {
                                char* args[4];
                                args[0] = argv[0];
                                args[1] = opt_str;
                                args[2] = argv[md_ind];
                                args[3] = file_path;

                            /* Hard Coded due to possible repeated args */
                                proc_creat(4, args);
                            }

                            /* Program image Replacing */
                                execl(argv[0], argv[0], opt_str,
                                      argv[md_ind], file_path, NULL);

                            /* Only if something goes wrong with exec */
                                if (log_info.available)
                                    proc_exit(getpid(), 127);
                                exit(127);

                            default:
                                wait(&pid);
                            /* No more children are alive */
                                last_child_dead = GROUP_LEADER;
                        }

                    } else if (S_ISREG(stat_buf.st_mode)) {
                        chmod(tmp_fl_path, new_mode);

                        if (log_info.available)
                            file_modf(tmp_fl_path, old_mode,
                                        new_mode, getpid());

                        options_output(&opt, tmp_fl_path,
                                        &old_mode, &new_mode, false);

                        nfmod = (new_mode == old_mode) ? nfmod : nfmod + 1;

                    } else if (S_ISLNK(stat_buf.st_mode)) {
                        /* Symbolic link has been found */
                        fprintf(stdout,
                "neither symbolic link '%s' nor referent has been changed\n",
                                tmp_fl_path);
                    }

                    nftot += 1;
                    free(tmp_fl_path);
                }
                closedir(directory);

            } else {
                /* 
                 * In case we set the recursive mode and we just pass a FILE
                 * every error message should be according to chmod but ENOTDIR
                 */
                if (errno != ENOTDIR)
                    /* something failed in this point */
                    options_output(&opt, file_path, &old_mode, &new_mode, true);
            }
        }

        free(file_path);
    }

    if (log_info.available)
        proc_exit(getpid(), 0);
    return 0;
}
