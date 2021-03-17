#include "../headers/logs.h"

#define MAX_STR_LEN 1024
#define TIME_INDEX 21

// MIGHT NEED SOME REFACTORING
static double get_proc_time() {
    time_t all_time = 0l, proc_time = 0l;
    char temp[MAX_STR_LEN];

    FILE* fp = fopen("/proc/stat", "r");
    while (fgets(temp, MAX_STR_LEN, fp) != NULL) {
        /* Getting btime - time since epoch to machine boot */
        if (strstr(temp, "btime") != NULL) {
            char* token_1;

            token_1 = strtok(temp, " ");
            token_1 = strtok(NULL, " ");
            all_time = atoll(token_1);
        }
    }
    
    char proc_path[MAX_STR_LEN/4];
    snprintf(proc_path, MAX_STR_LEN/4, "/proc/%d/stat", getpgid(getpid()));

    FILE* fp_proc = fopen(proc_path, "r");
    if (fgets(temp, MAX_STR_LEN, fp_proc) != NULL) {
        char* token_2;

        token_2 = strtok(temp, " ");
        for(int i = 0; i < TIME_INDEX; ++i)
            token_2 = strtok(NULL, " ");

        proc_time = atoll(token_2);
    }

    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    /*
     * time.tvsec + time.tvnsec * 10^-9 = tempo desde 1 de Janeiro de 1970 até agora 
     * all_time = tempo desde a época até ao momento em que ligamos a máquina
     * time.tvsec - all_time = tempo desde que ligamos a máquina
     * proc_time = tempo desde que a máquina ligou até o processo inicial
     * subtração: tempo que passou desde que o processo começou
     */
    return (double)((time.tv_sec + (time.tv_nsec*pow(10.0, -9.0)) - all_time) -
                     (proc_time / sysconf(_SC_CLK_TCK)) ) * 1000;
}


/* LOGS Setup */
void logs_setup(struct logs* log_file, int argc, char *argv[]) {
    char log_path[MAX_STR_LEN];
    strncpy(log_path, getenv("LOG_FILENAME"), MAX_STR_LEN - 1);

    log_file->available = log_path != NULL;
    strncpy(log_file->file_path, log_path, MAX_STR_LEN);

    if (log_file->available) {
        if (GROUP_LEADER) {
            /* LOG_FILENAME Truncation */
            log_file->fp = fopen(log_file->file_path, "r+");
            fclose(log_file->fp);

            proc_creat(log_file, argc, argv);
        }
    }

}

/* PROCESS CREATED */
void proc_creat(struct logs* log_file, int argc, char* argv[]) {
    log_file->fp = fopen(log_file->file_path, "a");
    
    char temp[MAX_STR_LEN];
    snprintf(temp, MAX_STR_LEN, "%f ; %d ; PROC_CREAT  ;\n",
             get_proc_time(), getpid());

    for (int i = 1; i < argc; ++i) {
        strncat(temp, " ", 2);
        strncat(temp, argv[i], MAX_STR_LEN - strlen(temp));
    }

    /* To avoid compilation warnings, it's useless calling proc_exit() */
    if (fwrite(temp, sizeof(char), strlen(temp) + 1, log_file->fp) !=
        strlen(temp))
        exit(1);

    fclose(log_file->fp);
}

/* PROCESS TERMINATED */
void proc_exit(struct logs* log_file, pid_t pid, int status) {
    signal_sent(log_file, "SIGCHLD", getppid());        

    int fl = open(log_file->file_path, O_APPEND);
    char temp[MAX_STR_LEN];
    snprintf(temp, MAX_STR_LEN, "%f ; %d ; PROC EXIT   ; %d\n",
             get_proc_time(), pid, status);

    /* 
     * Signal safe!
     * Function might be called from a signal handler,
     * however fwrite isn't listed in signal-safety(7)
     */
    if (write(fl, temp, strlen(temp) + 1) == -1)
        exit(1);

    close(fl);
}

/* Modified FILE/DIR */
void file_modf(struct logs* log_file, char* file_path, 
               mode_t old_mode, mode_t new_mode, pid_t pid) {
    log_file->fp = fopen(log_file->file_path, "a");
    char temp[MAX_STR_LEN];

    old_mode &= 0777;

    snprintf(temp, MAX_STR_LEN, "%.5f ; %d ; FILE_MODF   ; %s : %04o : %04o\n",
             get_proc_time(), pid, file_path, old_mode, new_mode);

    /* To avoid compilation warnings, it's useless calling proc_exit() */
    if (fwrite(temp, sizeof(char), strlen(temp) + 1, log_file->fp) !=
        strlen(temp))
        exit(1);

    fclose(log_file->fp);
}


/* Signal Sent */
void signal_sent(struct logs* log_file, char* signal, pid_t pid) {
    char temp[MAX_STR_LEN];
    snprintf(temp, MAX_STR_LEN, "%.5f ; %d ; SIGNAL_SENT ; %s : %d\n",
             get_proc_time(), getpid(), signal, pid);
    int fl = open(log_file->file_path, O_APPEND);
    
    /* 
     * Signal safe!
     * Function might be called from a signal handler,
     * however fwrite isn't listed in signal-safety(7)
     */
    if (write(fl, temp, strlen(temp) + 1) == -1)
        exit(1);

    close(fl);
}

/* Signal Received */
void signal_recv(struct logs* log_file, char* signal){
    char temp[MAX_STR_LEN];
    snprintf(temp, MAX_STR_LEN, "%.5f ; %d ; SIGNAL_RECV ; %s\n", 
             get_proc_time(), getpid(), signal);
    int fl = open(log_file->file_path, O_APPEND);

    /* 
     * Signal safe!
     * Function might be called from a signal handler,
     * however fwrite isn't listed in signal-safety(7)
     */
    if (write(fl, temp, strlen(temp) + 1) == -1)
        exit(1);

    close(fl);
}
