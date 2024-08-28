#include "shell.h"
#include "history.h"

int main() {
    initialize_history();
    builtin *commands = NULL;
    setUpCommands(&commands);
    execute(commands);
    save_history();
    return 0;
}