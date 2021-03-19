#ifndef  MODE_H_
#define  MODE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "../include/logs.h"

/**
 * @brief Processes the argument string passed through the command line
 *        into a proper mask mode_t for calling chmod().
 *        It handles both OCTAL-MODE and MODE. 
 * 
 * @param mode passed by the user through the command line.
 * @param file_path String containing the path of the FILE/DIR
 *                  whose premitions will change.
 * @param old_mode current file mode.
 * @return mode_t new mode set of premissions for the input file(s).
 */
mode_t parse_mode(const char* mode, char* file_path);

/**
 * @brief Gets the current FILE/DIR set of premission.
 * 
 * @param file_path String with the location of FILE/DIR.
 * @return mode_t set of premissions of input FILE/DIR path.
 */
mode_t get_current_file_mode(const char* file_path);

#endif  // MODE_H_
