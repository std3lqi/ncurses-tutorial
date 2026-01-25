#include <curses.h>
#include "win-manager.h"

#define WINDOWS 4

static WINDOW *windows[WINDOWS];
static int count = 0;
static int current_window_index = -1;

void init_win_manager() {
    for (int i = 0; i < WINDOWS; i++) {
        windows[i] = NULL;
    }
}

void register_window(WINDOW *win) {
    windows[count] = win;
    count++;
    if (current_window_index == -1) {
        current_window_index = 0;
    }
}

void next_window() {
    if (current_window_index < count - 1) {
        current_window_index++;
    } else {
        current_window_index = 0;
    }
}

bool is_current_window(WINDOW *win) {
    return windows[current_window_index] == win;
}