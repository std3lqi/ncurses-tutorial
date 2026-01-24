#include <curses.h>
#include <dirent.h>
#include "file-win.h"

static WINDOW *win = NULL;
static int entry_count = 0;
static struct dirent **entry_list = NULL;

void create_file_list_window(int h, int w, int y, int x) {
    win = newwin(h, w, y, x);
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    mvwaddstr(win, 0, 1, "File List");
    wrefresh(win);
}

void delete_file_list_window() {
    delwin(win);
    win = NULL;
    free(entry_list);
    entry_list = NULL;
    entry_count = 0;
}

void list_dir_in_file_list_window(const char *dir) {
    free(entry_list);
    entry_count = scandir(dir, &entry_list, NULL, NULL);

    int h = getmaxy(win);
    int w = getmaxx(win);

    // Draw border
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);

    // Print title
    if (strlen(dir) > w - 2) {
        mvwaddnstr(win, 0, 1, dir, w - 2 - 3);
        waddstr(win, "...");
    } else {
        mvwaddstr(win, 0, 1, dir);
    }

    // Print entries
    int y = 1;
    if (entry_count == 0) {
        mvwaddstr(win, y, 1, "Nothing");
    } else {
        for (int row = 0; row < h - 2; row++) {
            int i = row;
            if (i >= entry_count) {
                break;
            }
            struct dirent *entry = entry_list[i];
            mvwaddstr(win, y, 1, entry->d_name);
            y++;
        }
    }

    // Print footer
    y = h - 1;
    mvwprintw(win, y, 1, "%d", entry_count);

    wrefresh(win);
}