#include <curses.h>
#include <panel.h>
#include "help-win.h"

void show_help_window(char *dir, int len) {
    int h = 20;
    int w = 60;
    WINDOW *win = newwin(h, w, (LINES - h) / 2, (COLS - w) / 2);
    PANEL *panel = new_panel(win);
    box(win, 0, 0);
    mvwaddstr(win, 0, 1, "Help");

    int y = 3;
    int x = 3;
    mvwprintw(win, y++, x, "ncurses : %s", curses_version());
    mvwprintw(win, y++, x, "terminal: %s", longname());
    mvwprintw(win, y++, x, "terminal: %s", termname());

    // Dir: _____________________
    mvwprintw(win, y, x, "Dir: ");
    x += strlen("Dir: ");
    wbkgdset(win, A_UNDERLINE);
    waddstr(win, "                   ");
    wbkgdset(win, A_NORMAL);

    wmove(win, y, x);
    curs_set(1);
    echo();
    nocbreak();
    wgetnstr(win, dir, len);
    curs_set(0);
    noecho();
    cbreak();

    wrefresh(win);

    update_panels();
    doupdate();

    del_panel(panel);
    delwin(win);
}
