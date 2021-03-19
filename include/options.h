#ifndef  OPTIONS_H_
#define  OPTIONS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>

#include "../include/logs.h"


/**
 * @brief Options is a set of flags for program output.
 * 
 */
struct options {
    bool recursive;     /* Option for Recursive Strategy */
    bool verbose;       /* Option for verbose - prints all the ocorrences */
    bool changes;       /* Option for changes - prints only the changes */
};

/**
 * @brief Options handler, checks the validation of the options and activates 
 *        the struct flags passed in argument.
 * 
 * @param options Struct to activate certain options.
 * @param arg Options passed through command line.
 * @return int 0 if successful.
 */
int parse_options(struct options* options, char* arg);

/**
 * @brief Output handler, prints the information required to stdout based
 *        on the status of the flags from struct options.
 * 
 * @param opt Set of flags passed as input by the command line.
 * @param file_path String with the location of FILE/DIR modified or not.
 * @param old_mode Previous set of file premissions.
 * @param new_mode New set of file premissions.
 */
void options_output(const struct options* opt, const char* file_path,
                    mode_t* old_mode, mode_t* new_mode, bool failure);

/**
 * @brief Get the OPTIONS string
 * 
 * @param opt Set of flags passed as input by the command line.
 * @param arg string filled with the set of options
 */
void get_options_str(const struct options* opt, char* arg);

#endif  // OPTIONS_H_
