#ifndef UTILS_H
#define UTILS_H

void trimSpaces(char *string);
int findPath(char *path);
int isExecutable(const char *path);
char *isInPath(char *argument);
char **tokenize(char *string);
char *getInput(char *input);
void freeTokens(char **tokens);
int handleSysUtils(char **temp);

#endif // UTILS_H