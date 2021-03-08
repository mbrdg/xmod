#ifndef  HEADERS_MODE_H_
#define  HEADERS_MODE_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * @brief Processes the argument string passed through the command line
 *        into a proper mask mode_t for calling chmod().
 *        It handles both OCTAL-MODE and MODE. 
 * 
 * @param mode passed by the user through the command line
 * @param file_path String containing the path of the file/directory 
 *                  whose premitions will change.
 * @param old_mode current file mode
 * @return mode_t new mode set of premissions for the input file(s).
 */
mode_t parse_mode(char* mode, char* file_path);

mode_t get_current_file_mode(const char* file_path);

#endif  // HEADERS_MODE_H_
