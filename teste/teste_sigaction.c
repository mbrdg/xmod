/*
*	Example of signals in action (with SIGACTION)
*/
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <wait.h>
#include <stdlib.h>

int child;

void sigcontHandler(int signal){
    return;
}

void sigusr1Handler(int signo) {
    sleep(1);
	fprintf(stderr, "\nReceived signal [USR1] %d!\n", signo);
}

void sigintHandler(int signo) {
    printf("\n\npid = %d | pgid = %d\n\n", getpid(), getpgid(getpid()));
    char temp[1024];
    struct sigaction new, old;
	sigset_t smask;	// defines signals to block while func() is running

    // prepare struct sigaction
    if (sigemptyset(&smask)==-1)	// block no signal
        perror ("sigsetfunctions");

    new.sa_mask = smask;
    new.sa_flags = SA_RESTART;	// usually works
    new.sa_handler = sigcontHandler;
    if(sigaction(SIGCONT, &new, &old) == -1)
        perror ("sigaction");

    new.sa_handler = sigintHandler;
    if(sigaction(SIGINT, &new, &old) == -1)
        perror ("sigaction");

    if(getpgid(getpid()) == getpid()){
        while(1){
            printf("pid = %d | seriously? [y/n]\n", getpid());
            scanf("%s", temp);
            if(strcmp(temp, "y") == 0){
                kill(0, SIGKILL);
            }
            else if(strcmp(temp, "n") == 0) {
                kill(0, SIGCONT);
                return;
            }
        }
    } else {
        pause();
    }
}

int main() {

	struct sigaction new, old;
	sigset_t smask;	// defines signals to block while func() is running

    // prepare struct sigaction
    if (sigemptyset(&smask)==-1)	// block no signal
        perror ("sigsetfunctions");

    new.sa_mask = smask;
    new.sa_flags = SA_RESTART;	// usually works

    //int handler
    new.sa_handler = sigintHandler;
    if(sigaction(SIGINT, &new, &old) == -1)
        perror ("sigaction");

    //handler user1
    new.sa_handler = sigusr1Handler;
    if(sigaction(SIGUSR1, &new, &old) == -1)
        perror ("sigaction");

    printf("\nSIGUSR1 handler is ready...");

    int pid = fork();
    switch (pid){
        case -1/* constant-expression */:
            perror("fork");
            break;
        case 0:
            for(int i = 0; i < 1000000000; i++){
                printf(" ");
            }
            printf("\nfinito\n");
            break;
        default:{
            int status = 0;
            child = pid;
            printf("sending...\n");
            waitpid(pid, &status, 0);

            printf("CARALHOOO\n");
            return 0;
            break;
        }
        }

    printf("I am %d\n", getpid());
    return 0;
}