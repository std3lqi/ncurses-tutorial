#include <curses.h>
#include "win-manager.h"

#define WINDOWS 4


static WINDOW *windows[WINDOWS];
static WIN_KEY_HANDLE key_handle_funcs[WINDOWS];
static int count = 0;
static int current_window_index = -1;

void init_win_manager() {
    for (int i = 0; i < WINDOWS; i++) {
        windows[i] = NULL;
    }
}

void register_window_with_key_handle(WINDOW *win, 
    WIN_KEY_HANDLE handle) {
    windows[count] = win;
    key_handle_funcs[count] = handle;
    count++;
    if (current_window_index == -1) {
        current_window_index = 0;
    }
}

void register_window(WINDOW *win) {
    register_window_with_key_handle(win, NULL);
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

void handle_key_in_current_window(int ch) {
    if (0 <= current_window_index && current_window_index < count) {
        if (key_handle_funcs[current_window_index]) {
            key_handle_funcs[current_window_index](ch);
        }
    }
}