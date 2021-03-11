#ifndef  HEADERS_FILE_H_
#define  HEADERS_FILE_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>

/**
 * @brief Tests the file or directory for opening checking if it is
 *         accessible to the process group.
 * 
 * @param arg String containg file/directory path.
 * @return const char* String containg file/directory path.
 */
char* parse_file(const char* arg);


#endif  // HEADERS_FILE_H_
