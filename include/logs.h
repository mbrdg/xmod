#ifndef  LOGS_H_
#define  LOGS_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/times.h>

#define GROUP_LEADER (getpid() == getpgid(getpid()))
#define GROUP_ID (getpgid(getpid()))

/**
 * @brief struct containg relevant information about program logs
 * 
 */
struct logs {
    FILE* fp; /* FILE struct pointing to LOG_FILENAME */
    char file_path[1024]; /* String containig LOG_FILENAME path */
    bool available; /* true if LOG_FILENAME is set, else false */
};

/**
 * @brief Checks if the LOG_FILENAME environment variable
 *        is properly set
 * 
 * @param argc count of arguments passed to the new process
 * @param argv vector of arguments passed to a process when it is created
 */
void logs_setup(int argc, char *argv[]);

// ----------------------- PROCESSES LOGS -----------------------

/**
 * @brief Registers to the LOG_FILENAME whenever a process is created
 * 
 * @param argc count of arguments passed to the new process
 * @param argv vector of arguments passed to a process when it is created
 */
void proc_creat(int argc, char* argv[]);

/**
 * @brief Registers to the LOG_FILENAME whenever a process is terminated
 * 
 * @param pid Terminated Processed ID
 * @param status Exit status from process
 */
void proc_exit(int pid, int status);

/**
 * @brief Registers to the LOG_FILENAME whenever a FILE/DIR is processed
 * 
 * @param file_path String that contains the path to FILE/DIR
 * @param old_mode previous FILE/DIR permissions mode
 * @param new_mode new FILE/DIR permissions mode
 * @param pid Process ID responsible for processing FILE/DIR
 */
void file_modf(char* file_path, mode_t old_mode, mode_t new_mode, pid_t pid);

// ----------------------- SIGNAL LOGS -----------------------

/**
 * @brief Registers to LOG_FILENAME whenever a process sends a signal
 * 
 * @param signal string that contains signal's name
 * @param pid Process ID of the signal's receiver
 */
void signal_sent(char* signal, pid_t pid);

/**
 * @brief Registers to LOG_FILENAME whenever a process receives a signal
 * 
 * @param signal string that contains signal's name
 */
void signal_recv(char* signal);

#endif  // LOGS_H_
