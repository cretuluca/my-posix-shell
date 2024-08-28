#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_HISTORY_LINES 1000
#define HISTORY_FILE_NAME ".my_shell_history"

void initialize_history();
void save_history();
void display_history();
void add_to_history(const char *line);

#endif // HISTORY_H