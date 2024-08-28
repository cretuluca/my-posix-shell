#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../includes/uthash.h"
#include "builtins.h"
#include "utils.h"
#include "history.h"

void execute(builtin *commands);

#endif // SHELL_H
