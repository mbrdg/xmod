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
        for (int i = 0; i < TIME_INDEX; ++i)
            token_2 = strtok(NULL, " ");

        proc_time = atoll(token_2);
    }

    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    /*
     * time.tvsec + time.tvnsec * 10^-9 = tempo desde a Epoch (1970-1-1)
     * all_time = tempo desde a época até ao momento em que ligamos a máquina
     * time.tvsec - all_time = tempo desde que ligamos a máquina
     * proc_time = tempo desde que a máquina ligou até o processo inicial
     * subtração: tempo que passou desde que o processo começou
     */
    return (double)((time.tv_sec + (time.tv_nsec*pow(10.0, -9.0)) - all_time) -
                     (proc_time / sysconf(_SC_CLK_TCK)) ) * 1000;
}


extern struct logs log_info;

/* LOGS Setup */
void logs_setup(int argc, char *argv[]) {
    log_info.available = (getenv("LOG_FILENAME") != NULL);

    if (log_info.available) {
        strncpy(log_info.file_path, getenv("LOG_FILENAME"), MAX_STR_LEN);

        if (GROUP_LEADER) {
            char log_header[30] = " instant ; pid ; event ; info";

            /* LOG_FILENAME Truncation */
            log_info.fp = fopen(log_info.file_path, "w");
            fwrite(log_header, sizeof(char), strlen(log_header), log_info.fp);
            fclose(log_info.fp);
        }

        proc_creat(argc, argv);
    }
}

/* PROCESS CREATED */
void proc_creat(int argc, char* argv[]) {    
    char temp[MAX_STR_LEN];
    snprintf(temp, MAX_STR_LEN, "\n%.5f ; %d ; PROC_CREAT ;",
             get_proc_time(), getpid());

    for (int i = 1; i < argc; ++i) {
        strncat(temp, " ", 2);
        strncat(temp, argv[i], MAX_STR_LEN - strlen(temp));
    }

    log_info.fp = fopen(log_info.file_path, "a");

    /* To avoid compilation warnings, it's useless calling proc_exit() */
    if (fwrite(temp, sizeof(char), strlen(temp), log_info.fp) <
        strlen(temp))
        exit(1);

    fclose(log_info.fp);
}

/* PROCESS TERMINATED */
void proc_exit(pid_t pid, int status) {
    signal_sent("SIGCHLD", getppid());        

    char temp[MAX_STR_LEN];
    snprintf(temp, MAX_STR_LEN, "\n%.5f ; %d ; PROC_EXIT ; %d",
             get_proc_time(), pid, status);

    if (log_info.available) {
        int fl = open(log_info.file_path, O_WRONLY | O_APPEND);

        /* 
        * Signal safe!
        * Function might be called from a signal handler,
        * however fwrite isn't listed in signal-safety(7)
        */
        if (write(fl, temp, strlen(temp)) == -1)
            exit(1);

        close(fl);
    }
}

/* Modified FILE/DIR */
void file_modf(char* file_path, mode_t old_mode, mode_t new_mode, pid_t pid) {
    log_info.fp = fopen(log_info.file_path, "a");
    char temp[MAX_STR_LEN];

    old_mode &= 0777;

    snprintf(temp, MAX_STR_LEN, "\n%.5f ; %d ; FILE_MODF ; %s : %04o : %04o",
             get_proc_time(), pid, file_path, old_mode, new_mode);

    /* To avoid compilation warnings, it's useless calling proc_exit() */
    if (fwrite(temp, sizeof(char), strlen(temp), log_info.fp) <
        strlen(temp))
        exit(1);

    fclose(log_info.fp);
}


/* Signal Sent */
void signal_sent(char* signal, pid_t pid) {
    char temp[MAX_STR_LEN];
    snprintf(temp, MAX_STR_LEN, "\n%.5f ; %d ; SIGNAL_SENT ; %s : %d",
             get_proc_time(), getpid(), signal, pid);
    
    if (log_info.available) {
        int fl = open(log_info.file_path, O_WRONLY | O_APPEND);

        /* 
        * Signal safe!
        * Function might be called from a signal handler,
        * however fwrite isn't listed in signal-safety(7)
        */
        if (write(fl, temp, strlen(temp)) == -1)
            exit(1);

        close(fl);
    }
}

/* Signal Received */
void signal_recv(char* signal) {
    char temp[MAX_STR_LEN];
    snprintf(temp, MAX_STR_LEN, "\n%.5f ; %d ; SIGNAL_RECV ; %s", 
             get_proc_time(), getpid(), signal);

    if (log_info.available) {
        int fl = open(log_info.file_path, O_WRONLY | O_APPEND);

        /* 
        * Signal safe!
        * Function might be called from a signal handler,
        * however fwrite isn't listed in signal-safety(7)
        */
        if (write(fl, temp, strlen(temp)) == -1) 
            exit(1);

        close(fl);
    }
}
