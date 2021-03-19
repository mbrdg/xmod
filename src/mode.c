#include "../include/mode.h"

extern struct logs log_info;

/**
 * @brief Creates a mini-mask to apply to the different types of users
 *        (Example) Input: "a-rx", Output: "0b00000101".
 * 
 * @param mode Input string.
 * @return u_int8_t Mini-mask with the correspondig bits set to 1.
 */
static uint8_t permissions_set(const char* mode) {
    uint8_t mode_mask = 0u;

    for (size_t i = 2; i < strlen(mode); ++i) {
        switch (mode[i]) {
            case 'r':
                mode_mask |= 1<<2;
                break;
            case 'w':
                mode_mask |= 1<<1;
                break;
            case 'x':
                mode_mask |= 1<<0;
                break;

            default:
                /* exit error - invalid premission */
                fprintf(stderr, "xmod: invalid premission '%c'\n", mode[i]);
                if (log_info.available)
                    proc_exit(getpid(), 1);
                exit(1);
        }
    }

    return mode_mask;
}

#undef MAX_STR_LEN
#define MAX_STR_LEN 5

#define MIN_STR_LEN 3
/**
 * @brief Parses the new mode for file/directory passed as a normal string 
 *        format.
 * 
 * @param mode Input mode string.
 * @param file_path String containg file/directory path to obtain its previous
 *                  premissions.
 * @return mode_t new mode set of premissions for the input file(s).
 */
static mode_t parse_mode_str(const char* mode, char* file_path) {
    if (strlen(mode) > MAX_STR_LEN || strlen(mode) < MIN_STR_LEN) {
        /* exit error - invalid MODE string size */
        fprintf(stderr, "xmod: invalid MODE string size\n");

        if (log_info.available)
            proc_exit(getpid(), 1);
        exit(1);
    }
    if (mode[1] != '-' && mode[1] != '=' && mode[1] != '+') {
        /* exit error - invalid MODE operand */
        fprintf(stderr, "xmod: invalid MODE operand '%c'\n", mode[1]);

        if (log_info.available)
            proc_exit(getpid(), 1);
        exit(1);
    }

    uint16_t mode_mask = permissions_set(mode);

    struct stat file_mode;
    stat(file_path, &file_mode);

    switch (mode[0]) {
        case 'o':
            /* 
             *  'Other' bit field come already set from permissions_set().
             *  No need to make changes!
             */
            break;
        case 'g':
            /* 'Group' bit field set according to the new mask */
            mode_mask = (mode_mask << 3);
            break;
        case 'u':
            /* 'User' bit field set according to the new mask */
            mode_mask = (mode_mask << 6);
            break;
        case 'a':
            /* U, G, and O bit fields set according to the new mask */
            mode_mask = (mode_mask << 6) | (mode_mask << 3) | mode_mask;
            break;
    }

    if (mode[1] == '+') {
        mode_mask |= file_mode.st_mode;

    } else if (mode[1] == '-') {
        mode_mask = (~mode_mask) & file_mode.st_mode;

    } else if (mode[1] == '=') {
        /* 
         * Copies the unchanged bits from the file_mode.st_mode
         * to mode_mask - the mask that will be set to FILE/DIR input
         */
        for (int8_t i = 2; i >= 0; --i) {
            mode_t temp = 07u;

            if ((mode_mask & (7u << i*3)) == 0) {
                temp <<= i*3;
                temp &= file_mode.st_mode;
                mode_mask |= temp;
            }
        }
    }
    return (mode_t) mode_mask;
}

#define OCT_LEN 4
/**
 * @brief Parses the new mode for FILE/DIR passed as an octal string format.
 * 
 * @param mode Input mode string.
 * @return mode_t new mode set of premissions for the input FILE/DIR.
 */
static mode_t parse_mode_octal(const char* mode) {
    if (strlen(mode) != OCT_LEN) {
        goto invalid_octal_format;
    }

    for (size_t i = 1; i < OCT_LEN; ++i) {
        if (!isdigit(mode[i]) || mode[i] >= '8') {
            goto invalid_octal_format;
        }
    }

    return (mode_t) strtol(mode, NULL, 8);

invalid_octal_format:
    /* Invalid MODE-OCTAL string format */
    fprintf(stderr, "xmod: invalid MODE-OCTAL input format\n");

    if (log_info.available)
        proc_exit(getpid(), 1);
    exit(1);
}

/* MODE Parser Entry Function */
mode_t parse_mode(const char* mode, char* file_path) {
    switch (mode[0]) {
        case '0':
            return parse_mode_octal(mode);
        case 'a':
        case 'u':
        case 'g':
        case 'o':
            return parse_mode_str(mode, file_path);
        case '+':
        case '-':
        case '=': {}
            char temp[MAX_STR_LEN + 1] = "a";
            strncat(temp, mode, MAX_STR_LEN);
            return parse_mode_str(temp, file_path);

        default:
            /* Invalid MODE string format */
            fprintf(stderr, "xmod: invalid MODE input format\n");

            if (log_info.available)
                proc_exit(getpid(), 1);
            exit(1);
    }
}

/* Utility to get FILE/DIR current premissions */
mode_t get_current_file_mode(const char* file_path) {
    struct stat file_mode;

    if (stat(file_path, &file_mode) == -1) {
        /* exit error - cannot access FILE/DIR status error */
        fprintf(stderr, "xmod: cannot access '%s' status: %s\n",
                file_path, strerror(errno));
    }

    return file_mode.st_mode;
}
