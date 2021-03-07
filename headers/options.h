#ifndef  HEADERS_OPTIONS_H_
#define  HEADERS_OPTIONS_H_

#include <stdbool.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    bool recursive;
    bool verbose;
    bool changes;
} options_t;

/**
 * @brief Options handler, checks the validation of the options and activates the struct flags passed in argument.
 * 
 * @param options Struct to activate certain options
 * @param arg Options passed through command line
 * @return int 0 if successful 
 */
int parse_options(options_t* options, char arg[]);

#endif  // HEADERS_OPTIONS_H_
