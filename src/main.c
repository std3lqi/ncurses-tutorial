#include <curses.h>
#include <panel.h>
#include "help-win.h"
#include "contents-win.h"
#include "debug-win.h"
#include "file-win.h"
#include "menubar-win.h"
#include "win-manager.h"

int main(int argc, char *argv[]) {
    initscr();      // Enter curses mode
    cbreak();       // Disable line buffering
    noecho();       // Disable echoing
    keypad(stdscr, TRUE);   // Enable function keys like F1
    curs_set(0);            // Hide cursor

    addch('N');
    addch('S' | A_STANDOUT);
    addch('U' | A_UNDERLINE);
    addch('R' | A_REVERSE);
    addch('K' | A_BLINK);
    addch('D' | A_DIM);
    addch('B' | A_BOLD);
    addch('P' | A_PROTECT);
    addch('H' | A_INVIS);
    addch('A' | A_ALTCHARSET);
    addch('I' | A_ITALIC);

    refresh();
    getch();

    endwin();       // Exit from curses mode
    return 0;
}

