#ifndef  FILE_H_
#define  FILE_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#include "../include/logs.h"

/**
 * @brief Tests the file or directory for opening checking if it is
 *         accessible to the process group.
 * 
 * @param arg String containg file/directory path.
 * @return const char* String containg file/directory path.
 */
char* parse_file(const char* arg);

/**
 * @brief Concatenates strings with the propuse of iterating to the next
 *        FILE/DIR with their corresponding token and calloc'd
 *        Input: parent: "folder1", child: "file1"
 *        Output: "folder1/file1"
 * 
 * @param parent current iterated DIR
 * @param child name of the nodes inside DIR
 * @return char* concatenated string with node value
 */
char* process_node(const char* parent, const char* child);

#endif  // FILE_H_
