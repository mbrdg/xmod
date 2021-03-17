#include "../headers/signals.h"

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

static void proc_info(){
    char * info;
    if(asprintf(&info, "\n%d ; %s ; %d ; %d", getpid(), file_path, nftot, nfmod) == -1){
        if (log_info.available)
            proc_exit(&log_info, getpid(), 1);
        exit(1);
    }
    if (write(STDOUT_FILENO, info, strlen(info)) == -1) {
        if (log_info.available)
            proc_exit(&log_info, getpid(), 1);
        exit(1);
    }
}

static void sig_general_handler(int signal){
    signal_recv(&log_info, sig_name[signal]);
    return;
}

static void sigusr1Handler(int signal) {
    signal_recv(&log_info, sig_name[signal]);

    if(!GROUP_LEADER) {
        proc_info();
        signal_sent(&log_info, sig_name[SIGCONT], getppid());
        kill(getppid(), SIGCONT);
        pause();
    }
}

static void sig_int_handler(int signal) {
    char temp[1024];
    signal_recv(&log_info, sig_name[signal]);

    if (GROUP_LEADER) {
        signal_sent(&log_info, sig_name[SIGUSR1],getgid());
        proc_info();
        killpg(0, SIGUSR1);
        pause();

        while (1) {

            if (write(STDOUT_FILENO, "\n\nDo you want to keep running the program [y/n] ", 49) == -1) {
                if (log_info.available)
                    proc_exit(&log_info, getpid(), 1);
                exit(1);
            }

            if(scanf("%s", temp) == EOF) {
                if (log_info.available)
                    proc_exit(&log_info, getpid(), 1);
                exit(1);
            }

            if (strncmp(temp, "n", 2) == 0) {
                signal_sent(&log_info, sig_name[SIGTERM], getgid());
                killpg(0, SIGTERM);

            } else if (strncmp(temp, "y", 2) == 0) {
                signal_sent(&log_info, sig_name[SIGCONT], getgid());
                killpg(0, SIGCONT);
                return;
            }
        }
        
    } else {
        pause();
    }
}

static void sig_term_handler(int signal) {
    if(GROUP_LEADER)
        wait(NULL);

    signal_recv(&log_info, sig_name[signal]);

    if (log_info.available)
        proc_exit(&log_info, getpid(), 130);
    exit(130);
}

void setup_signals() {
    struct sigaction new, old;
	sigset_t smask;	// defines signals to block while func() is running

    // prepare struct sigaction
    if (sigemptyset(&smask)==-1)	// block no signal
        perror ("sigsetfunctions");

    new.sa_mask = smask;
    new.sa_flags = SA_RESTART;	// usually works

    //int handler
    new.sa_handler = sig_int_handler;
    if(sigaction(SIGINT, &new, &old) == -1)
        perror ("sigaction");

    //term handler
    new.sa_handler = sig_term_handler;
    if(sigaction(SIGTERM, &new, &old) == -1)
        perror ("sigaction");

    new.sa_handler = sigusr1Handler;
    if(sigaction(SIGUSR1, &new, &old) == -1)
        perror ("sigaction");
    
    //general handler
    new.sa_handler = sig_general_handler;
    if(sigaction(SIGHUP, &new, &old) == -1)
        perror ("sigaction");
    if(sigaction(SIGCONT, &new, &old) == -1)
        perror ("sigaction");
    if(sigaction(SIGQUIT, &new, &old) == -1)
        perror ("sigaction");
    if(sigaction(SIGUSR2, &new, &old) == -1)
        perror ("sigaction");
    if(sigaction(SIGSEGV, &new, &old) == -1)
        perror ("sigaction");
    if(sigaction(SIGPIPE, &new, &old) == -1)
        perror ("sigaction");
    if(sigaction(SIGALRM, &new, &old) == -1)
        perror ("sigaction");
    if(sigaction(SIGCHLD, &new, &old) == -1)
        perror ("sigaction");
}
