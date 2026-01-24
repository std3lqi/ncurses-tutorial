#include <curses.h>
#include "debug-win.h"

static WINDOW *win = NULL;

void create_debug_window(int h, int w, int y, int x) {
    win = newwin(h, w, y, x);
    wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(win);
}

void delete_debug_window() {
    delwin(win);
    win = NULL;
}