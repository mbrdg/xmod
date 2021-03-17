#ifndef  HEADERS_LOGS_H_
#define  HEADERS_LOGS_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/times.h>
#include <time.h>
#include <math.h>

#define GROUP_LEADER ( getpid() == getpgid( getpid() ) )

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
 * @param argc 
 * @param argv 
 * @param log_file struct logs with a FILE* pointing to LOG_FILENAME
 * @return true LOG_FILENAME is set to a given file path
 * @return false LOG_FILENAME is not set to a given file path
 */
void logs_setup(struct logs* log_file, int argc, char *argv[]);

// ----------------------- PROCESSES LOGS -----------------------

/**
 * @brief Registers to the LOG_FILENAME whenever a process is created
 * 
 * @param log_file 
 * @param argv 
 * @param n 
 */
void proc_creat(struct logs* log_file, int argc, char* argv[]);

/**
 * @brief Registers to the LOG_FILENAME whenever a process is terminated
 * 
 * @param log_file struct logs with a FILE* pointing to LOG_FILENAME
 * @param pid Terminated Processed ID
 * @param status Exit status from process
 */
void proc_exit(struct logs* log_file, int pid, int status);

/**
 * @brief Registers to the LOG_FILENAME whenever a FILE/DIR is processed
 * 
 * @param log_file struct logs with a FILE* pointing to LOG_FILENAME
 * @param file_path String that contains the path to FILE/DIR
 * @param old_mode previous FILE/DIR permissions mode
 * @param new_mode new FILE/DIR permissions mode
 * @param pid Process ID responsible for processing FILE/DIR
 */
void file_modf(struct logs* log_file, char* file_path, 
               mode_t old_mode, mode_t new_mode, pid_t pid);

// ----------------------- SIGNAL LOGS -----------------------

/**
 * @brief Registers to LOG_FILENAME whenever a process sends a signal
 * 
 * @param log_file struct logs with a FILE* pointing to LOG_FILENAME
 * @param signal string that contains signal's name
 * @param pid 
 */
void signal_sent(struct logs* log_file, char* signal, pid_t pid);

/**
 * @brief Registers to LOG_FILENAME whenever a process receives a signal
 * 
 * @param log_file struct logs with a FILE* pointing to LOG_FILENAME
 * @param signal string that contains signal's name
 */
void signal_recv(struct logs* log_file, char* signal);

#endif  // HEADERS_LOGS_H_
