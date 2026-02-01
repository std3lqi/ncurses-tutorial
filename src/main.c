#include <curses.h>
#include <panel.h>
#include <limits.h>
#include "help-win.h"
#include "contents-win.h"
#include "debug-win.h"
#include "file-win.h"
#include "menubar-win.h"
#include "win-manager.h"
#include "constants.h"

extern int entry_types_to_show;

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

    if (has_colors()) {
        start_color();
        init_pair(COLOR_OF_DIR, COLOR_YELLOW, COLOR_BLACK);
        init_pair(COLOR_OF_DEBUG_WIN, COLOR_WHITE, COLOR_BLUE);
        init_pair(COLOR_OF_MENU_FORE, COLOR_GREEN, COLOR_BLACK);
    }

    mousemask(ALL_MOUSE_EVENTS, NULL);

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
            case 'd': {
                debug_line("This is a debug message");
                break;
            }
            case 'h': {
                char dir[PATH_MAX];
                show_help_window(dir, PATH_MAX - 1);
                if (strlen(dir) > 0) {
                    list_dir_in_file_list_window(dir);
                }
                update_panels();
                doupdate();
                break;
            }
            case 'm': {
                int command = show_menu();
                if (command == COMMAND_HELP) {
                    ungetch('h');
                } else if (command == COMMAND_EXIT) {
                    ungetch('q');
                } else if ((command & COMMAND_SHOW_MASK) != 0) {
                    entry_types_to_show = command;
                    list_dir_in_file_list_window(NULL);
                }
                break;
            }
            case '\t':
                next_window();
                refresh_menu_bar_window();
                refresh_file_list_window();
                refresh_debug_window();
                refresh_contents_window();
                break;
            default:
                handle_key_in_current_window(ch);
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

