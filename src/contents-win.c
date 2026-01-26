#include <curses.h>
#include "contents-win.h"
#include "win-manager.h"

static WINDOW *win = NULL;

void create_contents_window(int h, int w, int y, int x) {
    win = newwin(h, w, y, x);
    register_window(win, refresh_contents_window);
    refresh_contents_window();
}

void refresh_contents_window() {
    box(win, 0, 0);
    if (is_current_window(win)) {
        wattron(win, A_REVERSE);
    }        
    mvwaddstr(win, 0, 1, "Contents");
    if (is_current_window(win)) {
        wattroff(win, A_REVERSE);
    }    
    wrefresh(win);
}

void delete_contents_window() {
    delwin(win);
    win = NULL;
}