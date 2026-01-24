#include <curses.h>
#include "file-win.h"

static WINDOW *win = NULL;

void create_file_list_window(int h, int w, int y, int x) {
    win = newwin(h, w, y, x);
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(win);
}

void delete_file_list_window() {
    delwin(win);
    win = NULL;
}