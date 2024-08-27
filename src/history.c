#include <history.h>

void initialize_history() {
    using_history();
    read_history(HISTORY_FILE);

    stifle_history(MAX_HISTORY_LINES);
}

void save_history() {
    write_history(HISTORY_FILE);
}