#include "utils.h"
#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>

void trimSpaces(char *string) {
    char command[256];
    snprintf(command, sizeof(command), "echo \"%s\" | sed -E 's/ +/ /g' | sed -E 's/^ +//g' | sed -E 's/ +$//g'", string);

    FILE *fd = popen(command, "r");

    char buffer[256];
    fgets(buffer, sizeof(buffer), fd);
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
    char *line = readline("$ ");
    if (line == NULL) {
        strcpy(input, "exit");
    } else {
        strncpy(input, line, 99);
        input[99] = '\0';
        if (*line) {
            add_to_history(line);
        }
        free(line);
    }
    return input;
}

void freeTokens(char **tokens) {
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
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