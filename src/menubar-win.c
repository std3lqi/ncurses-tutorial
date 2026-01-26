#include <curses.h>
#include "menubar-win.h"
#include "win-manager.h"

static WINDOW *win = NULL;

void refresh_menu_bar_window() {
    if (is_current_window(win)) {
        wattron(win, A_REVERSE);
    }
    mvwaddstr(win, 0, 1, "Menu");
    if (is_current_window(win)) {
        wattroff(win, A_REVERSE);
    }

    wrefresh(win);
}

void create_menu_bar_window(int h, int w, int y, int x) {
    win = newwin(h, w, y, x);
    register_window(win, refresh_menu_bar_window);
    refresh_menu_bar_window();
}

void delete_menu_bar_window() {
    delwin(win);
    win = NULL;
}
