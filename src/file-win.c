#include <curses.h>
#include <panel.h>
#include <dirent.h>
#include <limits.h>
#include "file-win.h"
#include "win-manager.h"
#include "constants.h"
#include "contents-win.h"

static WINDOW *win = NULL;
static PANEL *panel = NULL;
static int entry_count = 0;
static struct dirent **entry_list = NULL;
static char current_path[PATH_MAX];
static int index_of_selected = -1;
static int index_of_first_line = -1;

static void handle_key(int ch);
static void handle_mouse(MEVENT *event);

void create_file_list_window(int h, int w, int y, int x) {
    win = newwin(h, w, y, x);
    panel = new_panel(win);
    register_window_with_key_handle(win, refresh_file_list_window, 
        handle_key, handle_mouse);
    current_path[0] = '\0';
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    mvwaddstr(win, 0, 1, "File List");
    wrefresh(win);
}

void delete_file_list_window() {
    del_panel(panel);
    panel = NULL;
    delwin(win);
    win = NULL;
    free(entry_list);
    entry_list = NULL;
    entry_count = 0;
}

void list_dir_in_file_list_window(const char *dir) {
    free(entry_list);
    entry_list = NULL;
    entry_count = scandir(dir, &entry_list, NULL, NULL);
    index_of_selected = 0;
    index_of_first_line = 0;
    strcpy(current_path, dir);
    refresh_file_list_window();
}

void refresh_file_list_window() {
    int h = getmaxy(win);
    int w = getmaxx(win);

    wclear(win);

    // Draw border
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);

    // Print title
    if (is_current_window(win)) {
        wattron(win, A_REVERSE);
    }
    if (strlen(current_path) > w - 2) {
        mvwaddnstr(win, 0, 1, current_path, w - 2 - 3);
        waddstr(win, "...");
    } else {
        mvwaddstr(win, 0, 1, current_path);
    }
    if (is_current_window(win)) {
        wattroff(win, A_REVERSE);
    }

    // Print entries
    int y = 1;
    if (entry_count == 0) {
        mvwaddstr(win, y, 1, "Nothing");
    } else {
        for (int row = 0; row < h - 2; row++) {
            int i = row + index_of_first_line;
            if (i >= entry_count) {
                break;
            }
            struct dirent *entry = entry_list[i];
            if (i == index_of_selected) {
                wattron(win, A_REVERSE);
            }
            if (entry->d_type == DT_DIR) {
                wattron(win, COLOR_PAIR(COLOR_OF_DIR));
            }
            mvwaddstr(win, y, 1, entry->d_name);
            if (entry->d_type == DT_DIR) {
                wattroff(win, COLOR_PAIR(COLOR_OF_DIR));
            }
            if (i == index_of_selected) {
                wattroff(win, A_REVERSE);
            }
            y++;
        }
    }

    // Print footer
    y = h - 1;
    mvwprintw(win, y, 1, "%d", entry_count);

    wrefresh(win);

    update_panels();
    doupdate();
}

void enter_dir(struct dirent *entry) {
    if (strcmp(entry->d_name, ".") == 0) {
        return;
    }
    char new_dir[PATH_MAX];
    if (strcmp(entry->d_name, "..") == 0) {
        strcpy(new_dir, current_path);
        char *end = strrchr(new_dir, '/');
        *end = '\0';        
    } else {
        sprintf(new_dir, "%s/%s", current_path, entry->d_name);
    }
    list_dir_in_file_list_window(new_dir);
}

void show_text_file(struct dirent *entry) {
    char file_path[PATH_MAX];
    sprintf(file_path, "%s/%s", current_path, entry->d_name);
    show_text_file_in_contents_window(file_path);
}

static void handle_mouse(MEVENT *event) {
    int y = event->y;
    int x = event->x;
    wmouse_trafo(win, &y, &x, FALSE);
    if ((event->bstate & BUTTON1_CLICKED) ||
        (event->bstate & BUTTON1_DOUBLE_CLICKED)) {
        index_of_selected = y - 1;
    }
    if (event->bstate & BUTTON1_DOUBLE_CLICKED) {
        if (index_of_selected >= 0 && index_of_selected < entry_count) {
            struct dirent *entry = entry_list[index_of_selected];
            if (entry->d_type == DT_DIR) {
                enter_dir(entry);
            }
        }
    }
}

static void handle_key(int ch) {
    switch (ch) {
        case KEY_DOWN:
            if (index_of_selected < entry_count - 1) {
                index_of_selected++;
                int lines = getmaxy(win) - 2;
                while (index_of_selected - index_of_first_line >= lines) {
                    index_of_first_line++;
                }
                refresh_file_list_window();
            }
            break;
        case KEY_UP:
            if (index_of_selected > 0) {
                index_of_selected--;
                while (index_of_selected - index_of_first_line < 0) {
                    index_of_first_line--;
                }
                refresh_file_list_window();
            }
            break;
        case '\n':
            if (0 <= index_of_selected && index_of_selected < entry_count) {
                struct dirent *entry = entry_list[index_of_selected];
                if (entry->d_type == DT_DIR) {
                    enter_dir(entry);
                } else if (entry->d_type == DT_REG) {
                    show_text_file(entry);
                }
            }
            break;
    }
}