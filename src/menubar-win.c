#include <curses.h>
#include <menu.h>
#include <panel.h>
#include "menubar-win.h"
#include "win-manager.h"
#include "constants.h"

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

int show_menu() {
    ITEM *items[3];
    items[0] = new_item("Help", "Show help window");
    items[1] = new_item("Exit", NULL);
    items[2] = NULL;

    MENU *menu = new_menu(items);
    set_menu_mark(menu, " * ");
    set_menu_pad(menu, '-');
    set_menu_fore(menu, COLOR_PAIR(COLOR_OF_MENU_FORE));

    int rows, columns;
    scale_menu(menu, &rows, &columns);

    WINDOW *win_menu = newwin(rows + 2, columns + 2, 1, 1);
    PANEL *panel = new_panel(win_menu);
    box(win_menu, 0, 0);
    WINDOW *subwin_menu = derwin(win_menu, rows, columns, 1, 1);
    set_menu_win(menu, win_menu);
    set_menu_sub(menu, subwin_menu);

    post_menu(menu);
    update_panels();
    doupdate();

    int ch;
    int command = COMMAND_NONE;
    while ((command == COMMAND_NONE) && (ch = getch()) != 'm') {
        switch(ch) {
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                wrefresh(win_menu);
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                wrefresh(win_menu);
                break;
            case '\n': {
                ITEM *cur_item = current_item(menu);
                const char *name = item_name(cur_item);
                if (strcmp(name, "Help") == 0) {
                    command = COMMAND_HELP;
                } else if (strcmp(name, "Exit") == 0) {
                    command = COMMAND_EXIT;
                }
                break;
            }
        }
    }
    unpost_menu(menu);

    del_panel(panel);
    delwin(win_menu);
    delwin(subwin_menu);

    free_menu(menu);
    free_item(items[0]);
    free_item(items[1]);

    update_panels();
    doupdate();

    return command;
}