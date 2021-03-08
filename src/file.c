#include "../headers/file.h"

// FILE/DIR parser function
char* parse_file(const char* arg) {
    char* file_path;

    if (access(arg, F_OK) != -1) {
        file_path = (char*) malloc(sizeof(char) * (strlen(arg) + 1));
        strncpy(file_path, arg, strlen(arg) + 1);

    } else {
        fprintf(stderr, "Something"); //Alterar texto
        exit(1);  //Invalid path
    }

    return file_path;
}
