#include "../include/logs.h"

#undef MAX_STR_LEN
#define MAX_STR_LEN 1024

#define TIME_INDEX 21

/**
 * @brief Function responsible to determine how long it took since
 *        the process start until an Interruption occured
 * 
 * @return double time delta in milliseconds
 */
static double get_proc_time(void) {
    time_t proc_time = 0ull;
    char temp[MAX_STR_LEN];
    char* token;

    char proc_path[32];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", GROUP_ID);

    FILE* fp;
    if ((fp = fopen(proc_path, "r")) == NULL)
        exit(1);

    if (fgets(temp, MAX_STR_LEN, fp) != NULL) {
        char* dummy_ptr = temp;  // Avoids compilation Warnings

        /* 
         * strtok_r has some known bugs, however none 
         * of them matter here, hence the use!
         */
        token = strtok_r(temp, " ", &dummy_ptr);
        for (int32_t i = 0; i < TIME_INDEX; ++i)
            token = strtok_r(NULL, " ", &dummy_ptr);

        proc_time = atoll(token);
    }
    fclose(fp);

    /* Works like charme, time since system boot */
    struct timespec time;
    clock_gettime(CLOCK_BOOTTIME, &time);

    /*
     * time.tvsec + time.tvnsec * 1e-9: time since system boot
     * proc_time: time since the beggining of the group process leader exec
     * delta: instant of time for an event
     */
    return (time.tv_sec + (time.tv_nsec * 1e-9) -
           (double) proc_time / sysconf(_SC_CLK_TCK)) * 1e3;
}


// -------------------- LOGS public module --------------------

extern struct logs log_info;

/* LOGS Setup */
void logs_setup(int argc, char *argv[]) {
    log_info.available = (getenv("LOG_FILENAME") != NULL);

    if (log_info.available) {
        strncpy(log_info.file_path, getenv("LOG_FILENAME"), MAX_STR_LEN - 1);

        if (GROUP_LEADER) {
            char log_header[30] = " instant ; pid ; event ; info\n";

            /* LOG_FILENAME Truncation */
            log_info.fp = fopen(log_info.file_path, "w");
            fwrite(log_header, sizeof(char), sizeof(log_header), log_info.fp);
            fclose(log_info.fp);

            proc_creat(argc, argv);
        }
    }
}

/* PROCESS CREATED */
void proc_creat(int argc, char* argv[]) {
    char temp[MAX_STR_LEN];
    snprintf(temp, MAX_STR_LEN, "%4.2f ; %d ; PROC_CREAT ;",
             get_proc_time(), getpid());

    for (int32_t i = 1; i < argc; ++i) {
        strncat(temp, " ", 2);
        strncat(temp, argv[i], MAX_STR_LEN - strlen(temp));
    }
    strncat(temp, "\n", 2);

    log_info.fp = fopen(log_info.file_path, "a");

    /* To avoid compilation warnings, it's useless calling proc_exit() */
    if (fwrite(temp, sizeof(char), strlen(temp), log_info.fp) < strlen(temp))
        exit(1);

    fclose(log_info.fp);
}

/* PROCESS TERMINATED */
void proc_exit(pid_t pid, int status) {
    /* Every proc must send a SICHILD telling his parent that is terminates */
    signal_sent("SIGCHLD", getppid());

    char temp[MAX_STR_LEN];
    /* Not signal safe but there's no other way to match xmod specification */
    snprintf(temp, MAX_STR_LEN, "%4.2f ; %d ; PROC_EXIT ; %d\n",
             get_proc_time(), pid, status);

    if (log_info.available) {
        int32_t fl = open(log_info.file_path, O_WRONLY | O_APPEND);

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

    /* mask with only the last 4 digits of FILE/DIR permissions mode */
    old_mode &= 0777;
    new_mode &= 0777;

    /* Not signal safe but there's no other way to match xmod specification */
    snprintf(temp, MAX_STR_LEN, "%4.2f ; %d ; FILE_MODF ; %s : %04o : %04o\n",
             get_proc_time(), pid, file_path, old_mode, new_mode);

    /* To avoid compilation warnings, it's useless calling proc_exit() */
    if (fwrite(temp, sizeof(char), strlen(temp), log_info.fp) < strlen(temp))
        exit(1);

    fclose(log_info.fp);
}


/* Signal Sent */
void signal_sent(char* signal, pid_t pid) {
    char temp[MAX_STR_LEN];
    /* Not signal safe but there's no other way to match xmod specification */
    snprintf(temp, MAX_STR_LEN, "%4.2f ; %d ; SIGNAL_SENT ; %s : %d\n",
             get_proc_time(), getpid(), signal, pid);

    if (log_info.available) {
        int32_t fl = open(log_info.file_path, O_WRONLY | O_APPEND);

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
    /* Not signal safe but there's no other way to match xmod specification */
    snprintf(temp, MAX_STR_LEN, "%4.2f ; %d ; SIGNAL_RECV ; %s\n",
             get_proc_time(), getpid(), signal);

    if (log_info.available) {
        int32_t fl = open(log_info.file_path, O_WRONLY | O_APPEND);

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
