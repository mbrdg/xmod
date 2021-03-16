/*
*	Example of signals in action (with SIGACTION)
*/
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <wait.h>
#include <stdlib.h>


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


    signal(SIGINT, sigintHandler); //reposição

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
        signal(SIGCONT, sigcontHandler);
        pause();
    }
}

int main() {

	
    signal(SIGINT, sigintHandler);

    //handler user1
    signal(SIGUSR1, sigusr1Handler);

    printf("\nSIGUSR1 handler is ready...");

    int pid = fork();
    switch (pid){
        case -1/* constant-expression */:
            perror("fork");
            break;
        case 0:
            for(int i = 0; i < 10000000; i++){
                printf(" ");
            }
            printf("\nfinito\n");
            break;
        default:
            int status = 0;
            printf("sending...\n");
            waitpid(pid, &status, 0);

            return 0;
            break;
        }

    return 0;
}