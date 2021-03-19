#ifndef  SIGNALS_H_
#define  SIGNALS_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include <stdbool.h>

#include "../include/logs.h"

/**
 * @brief Sets all the handlers required for signals
 * 
 */
void setup_signals(void);

#endif  // SIGNALS_H_
