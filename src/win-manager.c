#include <curses.h>
#include "win-manager.h"

#define WINDOWS 4


static WINDOW *windows[WINDOWS];
static WIN_KEY_HANDLE key_handle_funcs[WINDOWS];
static WIN_MOUSE_HANDLE mouse_handle_funcs[WINDOWS];
static WIN_REFRESH refresh_funcs[WINDOWS];
static int count = 0;
static int current_window_index = -1;

void init_win_manager() {
    for (int i = 0; i < WINDOWS; i++) {
        windows[i] = NULL;
    }
}

void register_window_with_key_handle(WINDOW *win, 
    WIN_REFRESH refresh_func,
    WIN_KEY_HANDLE key_handle_func,
    WIN_MOUSE_HANDLE mouse_handle_func) {
    windows[count] = win;
    key_handle_funcs[count] = key_handle_func;
    mouse_handle_funcs[count] = mouse_handle_func;
    refresh_funcs[count] = refresh_func;
    count++;
    if (current_window_index == -1) {
        current_window_index = 0;
    }
}

void register_window(WINDOW *win, WIN_REFRESH refresh_func) {
    register_window_with_key_handle(win, refresh_func, NULL, NULL);
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

static void handle_mouse(MEVENT *event) {
    // which window is under this mouse click
    int window_index = -1;
    if ((event->bstate & BUTTON1_CLICKED) ||
        (event->bstate & BUTTON1_DOUBLE_CLICKED)) {
        for (int i = 0; i < count; i++) {
            if (wenclose(windows[i], event->y, event->x)) {
                window_index = i;
                break;
            }
        }
        if (window_index == -1) {
            return;
        }
        // update current_window_index
        int previous_window_index = current_window_index;
        current_window_index = window_index;
        // refresh the previous current window
        if (refresh_funcs[previous_window_index]) {
            refresh_funcs[previous_window_index]();
        }

        // Forward mouse event
        if (mouse_handle_funcs[current_window_index]) {
            mouse_handle_funcs[current_window_index](event);
        }

        // refresh the current window
        if (refresh_funcs[current_window_index]) {
            refresh_funcs[current_window_index]();
        }
    }
}

void handle_key_in_current_window(int ch) {
    if (ch == KEY_MOUSE) {
        MEVENT event;
        if (getmouse(&event) == OK) {
            handle_mouse(&event);
        }
        return;
    }
    if (0 <= current_window_index && current_window_index < count) {
        if (key_handle_funcs[current_window_index]) {
            key_handle_funcs[current_window_index](ch);
        }
    }
}