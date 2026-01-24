#include <curses.h>
#include "menubar-win.h"

static WINDOW *win = NULL;

void create_menu_bar_window(int h, int w, int y, int x) {
    win = newwin(h, w, y, x);
    wrefresh(win);
}

void delete_menu_bar_window() {
    delwin(win);
    win = NULL;
}
