#ifndef  HEADERS_OPTIONS_H_
#define  HEADERS_OPTIONS_H_

#include <stdbool.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Options is a set of flags for program output
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
 * @param options Struct to activate certain options
 * @param arg Options passed through command line
 * @return int 0 if successful 
 */
int parse_options(struct options* options, char arg[]);

#endif  // HEADERS_OPTIONS_H_
