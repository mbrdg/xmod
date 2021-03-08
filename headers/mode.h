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

typedef struct {
    bool r;
    bool w;
    bool x;
} permitions_t;

typedef struct {
    bool is_octal;
    mode_t mode;
    char signal;
} mode_result_t;

mode_result_t parse_mode(char mode[]);

int update_mode(char file_path[],mode_result_t *mode);

#endif  // HEADERS_MODE_H_
