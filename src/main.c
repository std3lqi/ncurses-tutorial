#include <curses.h>
#include <panel.h>
#include "help-win.h"
#include "contents-win.h"
#include "debug-win.h"
#include "file-win.h"
#include "menubar-win.h"
#include "win-manager.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: ./main <dir>\n");
        exit(1);
    }

    initscr();      // Enter curses mode
    cbreak();       // Disable line buffering
    noecho();       // Disable echoing
    keypad(stdscr, TRUE);   // Enable function keys like F1
    curs_set(0);            // Hide cursor
    refresh();

    // addch('N');
    // addch('S' | A_STANDOUT);
    // addch('U' | A_UNDERLINE);
    // addch('R' | A_REVERSE);
    // addch('K' | A_BLINK);
    // addch('D' | A_DIM);
    // addch('B' | A_BOLD);
    // addch('P' | A_PROTECT);
    // addch('H' | A_INVIS);
    // addch('A' | A_ALTCHARSET);
    // addch('I' | A_ITALIC);

    char *dir = argv[1];
    // Remove the trailing /
    int len = strlen(dir);
    if (dir[len - 1] == '/') {
        dir[len - 1] = '\0';
    }

    int h, w;
    getmaxyx(stdscr, h, w);
    int h_files = h - 1 - 6;
    int w_files = w * 0.3;
    init_win_manager();
    create_menu_bar_window(1, w, 0, 0);
    create_file_list_window(h_files, w_files, 1, 0);
    create_contents_window(h_files, w - w_files, 1, w_files);
    create_debug_window(6, w, 1 + h_files, 0);

    list_dir_in_file_list_window(dir);

    int ch;
    while ((ch = getch()) != 'q') {
        switch (ch) {
            case '\t':
                next_window();
                refresh_menu_bar_window();
                refresh_file_list_window();
                refresh_debug_window();
                refresh_contents_window();
                break;
        }
    }

    delete_menu_bar_window();
    delete_file_list_window();
    delete_contents_window();
    delete_debug_window();

    endwin();       // Exit from curses mode
    return 0;
}

