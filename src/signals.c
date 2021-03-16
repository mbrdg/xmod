#include "../headers/signals.h"

char *sig_name[]={"INVALID", "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT", "SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2", "SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP", "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU", "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH", "SIGPOLL", "SIGPWR", "SIGSYS", NULL};
extern unsigned int nftot;
extern unsigned int nfmod;
extern char* file_path;

static void proc_info(){
    char * info;
    if(asprintf(&info, "\n%d ; %s ; %d ; %d", getpid(), file_path, nftot, nfmod) == -1){
        proc_exit(getpid(), 1);
        exit(1);
    }
    if(write(STDOUT_FILENO, info, strlen(info)) == -1) {
        proc_exit(getpid(), 1);
        exit(1);
    }
}

static void sig_general_handler(int signal){
    signal_recv(sig_name[signal]);
    return;
}

static void sigusr1Handler(int signal) {
    signal_recv(sig_name[signal]);
    if(getpid() != getpgid(getpid())) {
        proc_info();
        signal_sent(sig_name[SIGCONT],getppid());
        kill(getppid(), SIGCONT);
        pause();
    }
}

static void sig_int_handler(int signal) {
    char temp[1024];
    signal_recv(sig_name[signal]);
    if(getpgid(getpid()) == getpid()){
        signal_sent(sig_name[SIGUSR1],getgid());
        proc_info();
        kill(0, SIGUSR1);
        pause();
        while(1){
            if(write(STDOUT_FILENO, "\n\nDo you want to keep running the program [y/n] ", 49) == -1){
                proc_exit(getpid(), 1);
                exit(1);
            }
            if(scanf("%s", temp) == EOF) {
                proc_exit(getpid(), 1);
                exit(1);
            }
            if(strcmp(temp, "n") == 0){
                signal_sent(sig_name[SIGTERM],getgid());
                kill(0, SIGTERM);
            }
            else if(strcmp(temp, "y") == 0) {
                signal_sent(sig_name[SIGCONT],getgid());
                kill(0, SIGCONT);
                return;
            }
        }
    } else {
        pause();
    }
}

static void sig_term_handler(int signal) {
    if(getpid()==getpgid(getpid())){
        wait(NULL);
    }
    signal_recv(sig_name[signal]);
    proc_exit(getpid(),0);
    exit(0);
}

void setup_signals(){
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
