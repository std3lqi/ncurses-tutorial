#include <form.h>
#include <panel.h>
#include "filter-form.h"

void show_filter_form() {
    FIELD *fields[5];

    // Label
    fields[0] = new_field(1, 10, 0, 0, 0, 0);
    field_opts_off(fields[0], O_ACTIVE);
    set_field_buffer(fields[0], 0, "Text: ");

    // Input
    fields[1] = new_field(1, 10, 0, 10, 0, 0);
    set_field_type(fields[1], TYPE_ALPHA, 1);
    set_field_back(fields[1], A_UNDERLINE);
    field_opts_off(fields[1], O_STATIC);
    set_max_field(fields[1], 20);

    // Cancel
    fields[2] = new_field(1, 10, 1, 0, 0, 0);
    field_opts_off(fields[2], O_EDIT);
    set_field_buffer(fields[2], 0, "Cancel");

    // OK
    fields[3] = new_field(1, 10, 1, 10, 0, 0);
    field_opts_off(fields[3], O_EDIT);
    set_field_buffer(fields[3], 0, "OK");

    fields[4] = NULL;

    FORM *form = new_form(fields);
    int rows, columns;
    scale_form(form, &rows, &columns);

    WINDOW *win = newwin(rows + 2, columns + 2, 
        (LINES - rows - 2) / 2, (COLS - columns - 2) / 2);
    PANEL *panel = new_panel(win);
    box(win, 0, 0);
    set_form_win(form, win);
    set_form_sub(form, derwin(win, rows, columns, 1, 1));

    post_form(form);
    update_panels();
    doupdate();

    getch();

    unpost_form(form);
    del_panel(panel);
    delwin(win);
    update_panels();
    doupdate();

    free_form(form);
    free_field(fields[0]);
    free_field(fields[1]);
    free_field(fields[2]);
    free_field(fields[3]);
}