#ifndef  HEADERS_MODE_H_
#define  HEADERS_MODE_H_

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>

mode_t parse_mode(char mode[],char file_path[]);

#endif  // HEADERS_MODE_H_
