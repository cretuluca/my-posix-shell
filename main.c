#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "uthash.h"

typedef struct {
    char *command;
    UT_hash_handle hh;
} builtin;

void trimSpaces(char *string) {
    char command[256];
    snprintf(command, sizeof(command), "echo \"%s\" | sed -E 's/ +/ /g' | sed -E 's/^ +//g' | sed -E 's/ +$//g'", string);

    FILE *fd = popen(command, "r");

    char buffer[256];
    char *result = fgets(buffer, sizeof(buffer), fd);
    pclose(fd);

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    strcpy(string, buffer);
}

int findPath(char *path) {
    return access(path, F_OK) == 0;
}

void addCommand(builtin **commands, char *command) {
    builtin *commandToAdd = malloc(sizeof(builtin));
    commandToAdd->command = strdup(command);
    HASH_ADD_KEYPTR(hh, *commands, commandToAdd->command, strlen(commandToAdd->command), commandToAdd);
}

int isBuiltin(builtin *commands, const char *command) {
    builtin *entry;
    HASH_FIND_STR(commands, command, entry);
    return entry != NULL;
}

int isExecutable(const char *path) {
    return access(path, X_OK) == 0;
}

char *isInPath(char *argument) {
    char *env = getenv("PATH");
    if (!env) return NULL;

    char *path = strdup(env);
    char *token = strtok(path, ":");
    static char fullPath[1024];

    while (token != NULL) {
        snprintf(fullPath, sizeof(fullPath), "%s/%s", token, argument);
        if (isExecutable(fullPath)) {
            free(path);
            return fullPath;
        }
        token = strtok(NULL, ":");
    }

    free(path);
    return NULL;
}

void setUpCommands(builtin **commands) {
    char *commandsToAdd[] = {"exit", "echo", "type", "pwd", "cd"};

    for (int i = 0; i < 5; i++) {
        addCommand(commands, commandsToAdd[i]);
    }
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

char **tokenize(char *string) {
    char **tokens = (char **) malloc(sizeof(char *) * 20);
    for (int i = 0; i < 20; i++) {
        tokens[i] = (char *) malloc(sizeof(char) * 256);
    }

    char *stringDup = strdup(string);
    char *token = strtok(stringDup, " ");

    int index = 0;

    while (token != NULL) {
        strcpy(tokens[index++], token);
        token = strtok(NULL, " ");
    }

    tokens[index] = NULL;

    free(stringDup);
    return tokens;
}

char *getInput(char *input) {
    fflush(stdout);

    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';

    return input;
}

void freeCommands(builtin *commands) {
    builtin *current, *temp;
    HASH_ITER(hh, commands, current, temp) {
        HASH_DEL(commands, current);
        free(current->command);
        free(current);
    }
}

void freeTokens(char **tokens) {
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
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

int handleSysUtils(char **temp) {
    char *path = isInPath(temp[0]);
    if (path != NULL) {
        if (fork() == 0) {
            execv(path, temp);
            perror("execv failed");
            exit(1);
        }

        wait(0);
        return 1;
    } else {
        return 0;
    }
}

void execute(builtin *commands) {
    char input[100];

    while (1) {
        printf("$ ");

        getInput(input);
        // trimSpaces(input);

        char **temp = tokenize(input);

        if (strncmp(input, "type ", 5) == 0) {
            handleType(commands, input);
        } else if (strncmp(input, "cd ", 3) == 0) {
            handleCD(input);
        } else if (strcmp(input, "pwd") == 0) {
            char cwd[100];
            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
        } else if (strcmp(input, "exit 0") == 0) {
            freeCommands(commands);
            freeTokens(temp);
            exit(0);
        } else if (strncmp(input, "echo ", 5) == 0) {
            printf("%s\n", input + 5);
        } else if (handleSysUtils(temp)) {
            // handled
        } else {
            printf("%s: command not found\n", input);
        }

        freeTokens(temp);
    }
}

int main() {
    builtin *commands = NULL;
    setUpCommands(&commands);
    execute(commands);
    return 0;
}
