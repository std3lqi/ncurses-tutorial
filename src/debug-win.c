#include <curses.h>
#include "debug-win.h"

static WINDOW *win = NULL;
static int count = 0;

void create_debug_window(int h, int w, int y, int x) {
    win = newwin(h, w, y, x);
    wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwaddstr(win, 0, 1, "Debug");
    wrefresh(win);
}

void delete_debug_window() {
    delwin(win);
    win = NULL;
}

void debug_line(const char *fmt, ...) {
    // 1: This is a debug line
    // 2: This is another debug line
    int y = 1 + count;
    int x = 1;
    mvwprintw(win, y, x, "%d: ", count);
    count++;

    va_list varglist;
    va_start(varglist, fmt);
    vw_printw(win, fmt, varglist);
    va_end(varglist);

    wrefresh(win);
}