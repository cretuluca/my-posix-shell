#ifndef BUILTINS_H
#define BUILTINS_H

#include "../includes/uthash.h"

typedef struct {
    char *command;
    UT_hash_handle hh;
} builtin;

void setUpCommands(builtin **commands);
int isBuiltin(builtin *commands, const char *command);
void handleType(builtin *commands, char *input);
void handleCD(char *input);
void freeCommands(builtin *commands);

#endif // BUILTINS_H