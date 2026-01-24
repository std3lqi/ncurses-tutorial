#include <curses.h>
#include "contents-win.h"

static WINDOW *win = NULL;

void create_contents_window(int h, int w, int y, int x) {
    win = newwin(h, w, y, x);
    box(win, 0, 0);
    mvwaddstr(win, 0, 1, "Contents");
    wrefresh(win);
}

void delete_contents_window() {
    delwin(win);
    win = NULL;
}