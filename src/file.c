#include "../headers/file.h"

#include <math.h>

extern char *log_path;

static double get_proc_time(){
    //get btime
    time_t all_time = 0, proc_time = 0;
    FILE * fp= fopen("/proc/stat", "r");
    char temp[1024];
    while(fgets(temp, 1024, fp) != NULL) {
        if(strstr(temp, "btime") != NULL) {
            char * token_1;
            token_1 = strtok(temp, " ");
            token_1 = strtok(NULL, " ");
            all_time = atoll(token_1);
        }
    }
    
    char* proc_process_path;
    asprintf(&proc_process_path, "/proc/%d/stat", getpgid(getpid()));
    FILE * fp_proc= fopen(proc_process_path, "r");
    if(fgets(temp, 1024, fp_proc) != NULL){
        char * token_2;
        token_2 = strtok(temp, " ");
        for(int i = 0; i < 21; i++)
            token_2 = strtok(NULL, " ");

        proc_time = atoll(token_2);
    }

    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    //time.tvsec + time.tvnsec * 10^-9 = tempo desde 1 de Janeiro de 1970 até agora 
    //all_time = tempo desde a época até ao momento em que ligamos a máquina
    //time.tvsec - all_time = tempo desde que ligamos a máquina
    //proc_time = tempo desde que a máquina ligou até o processo inicial
    //subtração: tempo que passou desde que o processo começou
    return (double)( (time.tv_sec + (time.tv_nsec * pow(10.0, -9.0)) - all_time) - (proc_time / sysconf(_SC_CLK_TCK)) ) * 1000;
}

/* FILE/DIR parser function */
char* parse_file(const char* arg) {
    char* ret_file_path;
    if (access(arg, F_OK) != -1) {
        ret_file_path = strdup(arg);

    } else {
        /* exit error - cannot access error */
        fprintf(stderr, "xmod: cannot access '%s': %s\n",
                                    arg, strerror(errno));
        fprintf(stderr, "failed to change mode of '%s' from 0000 (---------) to 0000 (---------)\n",
                                    arg);
        proc_exit(getpid(), 1);
        exit(1);
    }

    return ret_file_path;
}

void proc_creat(FILE* log_file, char** argv, size_t n){
    if(log_file!=NULL){
        char* temp;
        asprintf(&temp,"%f ; %d ; PROC_CREAT ;", get_proc_time(), getpid());
        for (size_t i = 1; i < n; i++)
        {
            asprintf(&temp, "%s %s",temp, argv[i]);
        }
        fwrite(temp, sizeof (char), strlen(temp), log_file);
        fputc('\n', log_file);
    }
}

void proc_exit(int pid, int status) {
    //if(getpid()!=getpgid(getpid())){  //CASO O PAI NAO TENHA DE ENVIAR QUE MORREU
    signal_sent("SIGCHLD", getppid());
    //}
    if(log_path!=NULL){
        FILE* log_file = fopen(log_path,"a");
        char* temp;
        asprintf(&temp,"%f ; %d ; PROC EXIT ; %d\n", get_proc_time(), pid, status);
        /* Signal safe */
        write(log_file->_fileno, temp, strlen(temp));
        fclose(log_file);
    }
}

void file_modf(char* file_path, mode_t old_mode, mode_t new_mode, int pid){
    if(log_path!=NULL){
        FILE* log_file = fopen(log_path,"a");
        char* temp;
        old_mode &= 0777;
        asprintf(&temp,"%f ; %d ; FILE_MODF ; %s : %04o : %04o\n", get_proc_time(), pid, file_path, old_mode, new_mode);
        fwrite(temp, sizeof (char), strlen(temp), log_file);
        fclose(log_file);
    }
}

void signal_sent(char * signal, int pid){
    if(log_path!=NULL){
        FILE* log_file = fopen(log_path,"a");
        char* temp;
        asprintf(&temp,"%f ; %d ; SIGNAL_SENT ; %s : %d\n", get_proc_time(), getpid(), signal, pid); //See pid received
        /* Signal safe */
        write(log_file->_fileno, temp, strlen(temp));
        fclose(log_file);
    }
}

void signal_recv(char* signal){
    if(log_path!=NULL){
        FILE* log_file = fopen(log_path,"a");
        char* temp;
        asprintf(&temp,"%f ; %d ; SIGNAL_RECV ; %s\n", get_proc_time(), getpid(), signal);
        /* Signal safe */
        write(log_file->_fileno, temp, strlen(temp));
        fclose(log_file);
    }
}
