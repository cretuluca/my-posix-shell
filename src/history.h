#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define HISTORY_FILE ".shell_history"
#define MAX_HISTORY_LINES 1000

void initialize_history();
void save_history();