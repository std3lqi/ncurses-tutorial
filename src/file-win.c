#include <curses.h>
#include "file-win.h"

static WINDOW *win = NULL;

void create_file_list_window(int h, int w, int y, int x) {
    win = newwin(h, w, y, x);
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    mvwaddstr(win, 0, 1, "File List");
    wrefresh(win);
}

void delete_file_list_window() {
    delwin(win);
    win = NULL;
}