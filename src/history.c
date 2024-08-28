#include "history.h"

static char history_file[1024];

void initialize_history() {
    char *home = getenv("HOME");
    if (home) {
        snprintf(history_file, sizeof(history_file), "%s/%s", home, HISTORY_FILE_NAME);
    } else {
        strcpy(history_file, HISTORY_FILE_NAME);
    }

    using_history();
    read_history(history_file);
    stifle_history(MAX_HISTORY_LINES);
}

void save_history() {
    write_history(history_file);
}

void display_history() {
    HIST_ENTRY *entry;
    int index;

    for (index = 0; index < history_length; index++) {
        entry = history_get(history_base + index);
        if (entry) {
            printf("%d  %s\n", history_base + index, entry->line);
        }
    }

    if (history_length == 0) {
        printf("No history available.\n");
    }
}

void add_to_history(const char *line) {
    if (line && *line) {
        add_history(line);
        write_history(history_file);
    }
}