#include "shell.h"

void execute(builtin *commands) {
    char input[100];

    while (1) {
        getInput(input);
        trimSpaces(input);

        char **temp = tokenize(input);

        if (strncmp(input, "type ", 5) == 0) {
            handleType(commands, input);
        } else if (strncmp(input, "cd ", 3) == 0) {
            handleCD(input);
        } else if (strcmp(input, "pwd") == 0) {
            char cwd[100];
            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
        } else if (strcmp(input, "exit") == 0 || strcmp(input, "exit 0") == 0) {
            freeCommands(commands);
            freeTokens(temp);
            exit(0);
        } else if (strncmp(input, "echo ", 5) == 0) {
            printf("%s\n", input + 5);
        } else if (strcmp(input, "history") == 0) {
            display_history();
        } else if (handleSysUtils(temp)) {
            // handled
        } else {
            printf("%s: command not found\n", input);
        }

        freeTokens(temp);
    }
}