#include "../include/signals.h"

char* sig_name[] = {
                    "INVALID", "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL",
                    "SIGTRAP", "SIGABRT", "SIGBUS", "SIGFPE", "SIGKILL",
                    "SIGUSR1", "SIGSEGV", "SIGUSR2", "SIGPIPE", "SIGALRM",
                    "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT", "SIGSTOP",
                    "SIGTSTP", "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU",
                    "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH", "SIGPOLL",
                    "SIGPWR", "SIGSYS", NULL
                   };

extern uint32_t nftot;
extern uint32_t nfmod;

extern struct logs log_info;
extern char* file_path;

extern bool last_child_dead;

#define MAX_STR_LEN 1024

/**
 * @brief Shows information about all the processes
 *        in the process group
 * 
 */
static void proc_info(void) {
    char info[MAX_STR_LEN];
    snprintf(info, MAX_STR_LEN, "\n%d ; %s ; %d ; %d",
                 getpid(), file_path, nftot, nfmod);

    if (write(STDOUT_FILENO, info, strlen(info)) == -1) {
        if (log_info.available)
            proc_exit(getpid(), 1);

        exit(1);
    }
}

// --------------------- SIGNAL FINCTIONS ---------------------

/* DEFAULT handler */
static void sig_default_handler(int signal) {
    signal_recv(sig_name[signal]);
}

/* SIGUSR1 handler */
static void sigusr1_handler(int signal) {
    signal_recv(sig_name[signal]);

    if (!GROUP_LEADER) {
        proc_info();
        signal_sent(sig_name[SIGCONT], getppid());
        kill(getppid(), SIGCONT);
        pause();
    }
}

#define NO_INPUT 1

/* SIGINT handler */
static void sigint_handler(int signal) {
    signal_recv(sig_name[signal]);

    char temp[MAX_STR_LEN];

    if (GROUP_LEADER) {
        signal_sent(sig_name[SIGUSR1], getgid());
        proc_info();
        killpg(0, SIGUSR1);

        /* Group leader only waits if any child is still alive */
        if (!last_child_dead)
            pause();

        /* Give some margin to scanf buffering */
        usleep(1e5);

        while (NO_INPUT) {
            char msg[48] = "\nDo you want to keep running the program [y/n] ";
            if (write(STDOUT_FILENO, msg, sizeof(msg)) == -1) {
                if (log_info.available)
                    proc_exit(getpid(), 1);
                exit(1);
            }
            if (scanf("%s", temp) == EOF) {
                if (log_info.available)
                    proc_exit(getpid(), 1);
                exit(1);
            }

            if (strcmp(temp, "n") == 0) {
                signal_sent(sig_name[SIGTERM], getgid());
                killpg(0, SIGTERM);

            } else if (strcmp(temp, "y") == 0) {
                signal_sent(sig_name[SIGCONT], getgid());
                killpg(0, SIGCONT);
                break;
            }
        }

    } else {
        pause();
    }
}

/* SIGTERM handler */
static void sigterm_handler(int signal) {
    if (GROUP_LEADER)
        wait(NULL);

    signal_recv(sig_name[signal]);

    if (log_info.available)
        proc_exit(getpid(), 130);
    exit(130);
}

/* Instantiation of signal handlers */
void setup_signals(void) {
    struct sigaction new, old;
    sigset_t smask;  // Defines signals to block while func() is running

    /* prepare struct sigaction */
    sigemptyset(&smask);  // Remove all signals from

    new.sa_mask = smask;
    new.sa_flags = SA_RESTART;  // scanf() call

    /* SIGINT handler */
    new.sa_handler = sigint_handler;
    sigaction(SIGINT, &new, &old);

    /* SIGTERM handler */
    new.sa_handler = sigterm_handler;
    sigaction(SIGTERM, &new, &old);

    /* SIGUSR1 handler */
    new.sa_handler = sigusr1_handler;
    sigaction(SIGUSR1, &new, &old);

    /* DEFAULT handler */
    new.sa_handler = sig_default_handler;
    sigaction(SIGHUP, &new, &old);
    sigaction(SIGCONT, &new, &old);
    sigaction(SIGQUIT, &new, &old);
    sigaction(SIGUSR2, &new, &old);
    sigaction(SIGSEGV, &new, &old);
    sigaction(SIGPIPE, &new, &old);
    sigaction(SIGALRM, &new, &old);
    sigaction(SIGCHLD, &new, &old);

    /* 
     * sigaction only returns error on EBDAD or EINVAL
     * in this case either are irrelevant, so we do not check for errors
     */
}
