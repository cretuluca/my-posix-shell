#include "builtins.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void addCommand(builtin **commands, char *command) {
    builtin *commandToAdd = malloc(sizeof(builtin));
    commandToAdd->command = strdup(command);
    HASH_ADD_KEYPTR(hh, *commands, commandToAdd->command, strlen(commandToAdd->command), commandToAdd);
}

void setUpCommands(builtin **commands) {
    char *commandsToAdd[] = {"exit", "echo", "type", "pwd", "cd", "history"};
    for (int i = 0; i < 6; i++) {
        addCommand(commands, commandsToAdd[i]);
    }
}

int isBuiltin(builtin *commands, const char *command) {
    builtin *entry;
    HASH_FIND_STR(commands, command, entry);
    return entry != NULL;
}

void handleType(builtin *commands, char *input) {
    if (isBuiltin(commands, input + 5)) {
        printf("%s is a shell builtin\n", input + 5);
    } else {
        char *k = isInPath(input + 5);
        if (k != NULL) {
            printf("%s is %s\n", input + 5, k);
        } else {
            printf("%s: not found\n", input + 5);
        }
    }
}

void handleCD(char *input) {
    if (strchr(input + 3, '.') != NULL) {
        if (findPath(input + 3)) {
            chdir(input + 3);
        } else {
            printf("cd: %s: No such file or directory\n", input + 3);
        }
    } else if (strchr(input + 3, '~') != NULL) {
        char *home = getenv("HOME");
        chdir(home);
    } else {
        if (findPath(input + 3)) {
            chdir(input + 3);
        } else {
            printf("cd: %s: No such file or directory\n", input + 3);
        }
    }
}

void freeCommands(builtin *commands) {
    builtin *current, *temp;
    HASH_ITER(hh, commands, current, temp) {
        HASH_DEL(commands, current);
        free(current->command);
        free(current);
    }
}