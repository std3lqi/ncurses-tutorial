#include <curses.h>
#include "debug-win.h"
#include "win-manager.h"
#include "constants.h"

static WINDOW *win = NULL;
static WINDOW *inner_win = NULL;
static int count = 0;

void create_debug_window(int h, int w, int y, int x) {
    win = newwin(h, w, y, x);
    wbkgd(win, COLOR_PAIR(COLOR_OF_DEBUG_WIN));
    // inner_win = subwin(win, h - 2, w - 2, y + 1, x + 1);
    inner_win = derwin(win, h - 2, w - 2, 1, 1);
    scrollok(inner_win, TRUE);
    register_window(win, refresh_debug_window);
    refresh_debug_window();
}

void refresh_debug_window() {
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
    delwin(inner_win);
    inner_win = NULL;
}

void debug_line(const char *fmt, ...) {
    // 1: This is a debug line
    // 2: This is another debug line
    int y = count;
    if (count >= 4) {
        wscrl(inner_win, 1);
        y = 3;
    }
    int x = 0;
    mvwprintw(inner_win, y, x, "%d: ", count);
    count++;

    va_list varglist;
    va_start(varglist, fmt);
    vw_printw(inner_win, fmt, varglist);
    va_end(varglist);

    wrefresh(inner_win);
}