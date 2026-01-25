#include <curses.h>
#include "debug-win.h"
#include "win-manager.h"
#include "constants.h"

static WINDOW *win = NULL;
static int count = 0;

void create_debug_window(int h, int w, int y, int x) {
    win = newwin(h, w, y, x);
    register_window(win);
    refresh_debug_window();
}

void refresh_debug_window() {
    wbkgd(win, COLOR_PAIR(COLOR_OF_DEBUG_WIN));
    wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
    if (is_current_window(win)) {
        wattron(win, A_REVERSE);
    }    
    mvwaddstr(win, 0, 1, "Debug");
    if (is_current_window(win)) {
        wattroff(win, A_REVERSE);
    }
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