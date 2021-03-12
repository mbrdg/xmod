#include "../headers/file.h"

/* FILE/DIR parser function */
char* parse_file(const char* arg) {
    char* ret_file_path;
    if (access(arg, F_OK) != -1) {
        ret_file_path = strdup(arg);

    } else {
        /* exit error - cannot access error */
        fprintf(stderr, "xmod: cannot access '%s': %s\n",
                                    arg, strerror(errno));
        exit(1);
    }

    return ret_file_path;
}

void proc_creat(FILE* log_file, char** argv, size_t n, clock_t begin){
    if(log_file!=NULL){
        char* temp;
        clock_t end=clock();
        asprintf(&temp,"%f ; %d ; PROC_CREAT ; ", (double)((end-begin)/(CLOCKS_PER_SEC*1000)), getpid());
        for (size_t i = 1; i < n; i++)
        {
            asprintf(&temp, "%s %s",temp, argv[i]);
        }
        fwrite(temp, sizeof(char), strlen(temp), log_file);
        fputc('\n', log_file);
    }
}

