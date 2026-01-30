#include <curses.h>
#include "contents-win.h"
#include "win-manager.h"
#include "debug-win.h"

static WINDOW *win = NULL;
static WINDOW *pad = NULL;
static int index_of_first_line = 0;
static int index_of_first_column = 0;

static void key_handle(int ch);

void create_contents_window(int h, int w, int y, int x) {
    win = newwin(h, w, y, x);
    register_window_with_key_handle(win, refresh_contents_window, 
        key_handle, NULL);
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

    if (pad) {
        int y_win = getbegy(win);
        int x_win = getbegx(win);
        int h_win = getmaxy(win);
        int w_win = getmaxx(win);
        int h_contents = h_win - 2;
        int w_contents = w_win - 2;
        prefresh(pad, index_of_first_line, index_of_first_column, 
            y_win + 1, x_win + 1,
            y_win + 1 + h_contents - 1,
            x_win + 1 + w_contents - 1
        );
    }
}

void delete_contents_window() {
    delwin(win);
    win = NULL;
    delwin(pad);
    pad = NULL;
}

void show_text_file_in_contents_window(const char *file_path) {
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        debug_line("Failed to open %s", file_path);
        return;
    }

    int lines = 0;
    
    const int PAD_WIDTH = 1024;
    char buffer[PAD_WIDTH + 1];
    while (fgets(buffer, PAD_WIDTH + 1, f)) {
        lines++;
    }
    rewind(f);

    delwin(pad);
    pad = newpad(lines, PAD_WIDTH);

    while (fgets(buffer, PAD_WIDTH + 1, f)) {
        wprintw(pad, "%s", buffer);
    }
    fclose(f);

    refresh_contents_window();
}

static void key_handle(int ch) {
    switch(ch) {
        case KEY_DOWN: {
            int h_contents = getmaxy(win) - 2;
            int lines_pad = getmaxy(pad);
            if (index_of_first_line + h_contents < lines_pad) {
                index_of_first_line++;
                refresh_contents_window();
            }
            break;
        }
        case KEY_UP: {
            if (index_of_first_line > 0) {
                index_of_first_line--;
                refresh_contents_window();
            }
            break;
        }
        case KEY_LEFT: {
            if (index_of_first_column > 0) {
                index_of_first_column--;
                refresh_contents_window();
            }
            break;
        }
        case KEY_RIGHT: {
            int w_contents = getmaxx(win) - 2;
            int columns_pad = getmaxx(pad);
            if (index_of_first_column + w_contents < columns_pad) {
                index_of_first_column++;
                refresh_contents_window();
            }
            break;
        }
    }
}