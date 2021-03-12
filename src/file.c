#include "../headers/file.h"

extern char *log_path;
extern clock_t begin;

/* FILE/DIR parser function */
char* parse_file(const char* arg) {
    char* ret_file_path;
    if (access(arg, F_OK) != -1) {
        ret_file_path = strdup(arg);

    } else {
        /* exit error - cannot access error */
        fprintf(stderr, "xmod: cannot access '%s': %s\n",
                                    arg, strerror(errno));
        prog_exit(getpid(), 1);
        exit(1);
    }

    return ret_file_path;
}

void proc_creat(FILE* log_file, char** argv, size_t n, clock_t begin){
    if(log_file!=NULL){
        char* temp;
        clock_t end=clock();
        asprintf(&temp,"%f ; %d ; PROC_CREAT ;", (double)((end-begin)/(CLOCKS_PER_SEC*1000)), getpid());
        for (size_t i = 1; i < n; i++)
        {
            asprintf(&temp, "%s %s",temp, argv[i]);
        }
        fwrite(temp, sizeof(char), strlen(temp), log_file);
        fputc('\n', log_file);
    }
}

void prog_exit(int pid, int status) {
    if(log_path!=NULL){
        FILE* log_file = fopen(log_path,"a");
        char* temp;
        clock_t end = clock();
        asprintf(&temp,"%f ; %d ; PROC EXIT ; %d\n", (double)((end-begin)/(CLOCKS_PER_SEC*1000)), pid, status);
        fwrite(temp, sizeof(char), strlen(temp), log_file);
        fclose(log_file);
    }
}

void file_modf(char* file_path, mode_t old_mode, mode_t new_mode, int pid){
    if(log_path!=NULL){
        FILE* log_file = fopen(log_path,"a");
        char* temp;
        old_mode &= 0777;
        clock_t end = clock();
        asprintf(&temp,"%f ; %d ; FILE_MODF ; %s : %04o : %04o\n", (double)((end-begin)/(CLOCKS_PER_SEC*1000)), pid, file_path, old_mode, new_mode);
        fwrite(temp, sizeof(char), strlen(temp), log_file);
        fclose(log_file);
    }
}
