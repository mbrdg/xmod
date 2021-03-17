#ifndef  HEADERS_SIGNALS_H_
#define  HEADERS_SIGNALS_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#include "../headers/logs.h"

/**
 * @brief Sets all the handlers required for signals
 * 
 */
void setup_signals(void);

#endif  // HEADERS_SIGNALS_H_
