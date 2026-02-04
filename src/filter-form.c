#include <form.h>
#include <panel.h>
#include "filter-form.h"
#include "debug-win.h"

extern char *filter_text;
extern bool is_whole_text;

const char *yes_no_choices[] = {
    "<Yes>", 
    "<No >",
    NULL
};

void show_filter_form() {
    FIELD *fields[7];

    int row = 0;
    // Label
    FIELD *label_field = new_field(1, 10, row, 0, 0, 0);
    field_opts_off(label_field, O_ACTIVE);
    set_field_buffer(label_field, 0, "Text: ");
    fields[0] = label_field;

    // Input
    FIELD *text_field = new_field(1, 10, row, 10, 0, 0);
    set_field_type(text_field, TYPE_ALPHA, 1);
    set_field_back(text_field, A_UNDERLINE);
    field_opts_off(text_field, O_STATIC);
    set_max_field(text_field, 20);
    fields[1] = text_field;
    row++;

    // Label
    row = 0;
    FIELD *label_whole_field = new_field(1, 10, row, 0, 0, 0);
    field_opts_off(label_whole_field, O_ACTIVE);
    set_field_buffer(label_whole_field, 0, "WholeText:");
    fields[2] = label_whole_field;
    set_new_page(label_whole_field, TRUE);

    // Input
    FIELD *enum_field = new_field(1, 5, row, 10, 0, 0);
    set_field_type(enum_field, TYPE_ENUM, yes_no_choices);
    set_field_back(enum_field, A_UNDERLINE);
    field_opts_off(enum_field, O_EDIT);
    set_field_buffer(enum_field, 0, yes_no_choices[1]);
    fields[3] = enum_field;
    row++;

    // Cancel
    FIELD *cancel_field = new_field(1, 6, row, 0, 0, 0);
    field_opts_off(cancel_field, O_EDIT);
    set_field_buffer(cancel_field, 0, "Cancel");
    fields[4] = cancel_field;

    // OK
    FIELD *ok_field = new_field(1, 2, row, 10, 0, 0);
    field_opts_off(ok_field, O_EDIT);
    set_field_buffer(ok_field, 0, "OK");
    fields[5] = ok_field;
    row++;

    fields[6] = NULL;

    FORM *form = new_form(fields);
    int rows, columns;
    scale_form(form, &rows, &columns);

    WINDOW *win = newwin(rows + 2, columns + 2, 
        (LINES - rows - 2) / 2, (COLS - columns - 2) / 2);
    PANEL *panel = new_panel(win);
    box(win, 0, 0);
    set_form_win(form, win);
    set_form_sub(form, derwin(win, rows, columns, 1, 1));

    curs_set(1);
    form_driver(form, REQ_FIRST_FIELD);

    post_form(form);
    update_panels();
    doupdate();

    int ch;
    bool stop = FALSE;
    set_escdelay(50);
    while ((!stop) && (ch = getch()) != 27) {
        switch(ch) {
            case KEY_LEFT:
                form_driver(form, REQ_PREV_CHAR);
                wrefresh(win);
                break;
            case KEY_RIGHT:
                form_driver(form, REQ_NEXT_CHAR);
                wrefresh(win);
                break;
            case KEY_UP:
                form_driver(form, REQ_PREV_CHOICE);
                wrefresh(win);
                break;
            case KEY_DOWN:
                form_driver(form, REQ_NEXT_CHOICE);
                wrefresh(win);
                break;
            case '<':
                form_driver(form, REQ_PREV_PAGE);
                wrefresh(win);
                break;
            case '>':
                form_driver(form, REQ_NEXT_PAGE);
                wrefresh(win);
                break;
            case 127:
                form_driver(form, REQ_DEL_PREV);
                wrefresh(win);
                break;
            case '\t':
                form_driver(form, REQ_NEXT_FIELD);
                wrefresh(win);
                break;
            case '\n': {
                FIELD *f = current_field(form);
                char *buf = field_buffer(f, 0);
                if (strcmp(buf, "Cancel") == 0) {
                    stop = TRUE;
                } else if (strcmp(buf, "OK") == 0) {
                    char *text = field_buffer(text_field, 0);
                    // debug_line("[%s]", text);
                    int index = strlen(text) - 1;
                    while (index >= 0 && text[index] == ' ') {
                        text[index] = '\0';
                        index--;
                    }
                    // debug_line("[%s]", text);
                    filter_text = strdup(text);

                    text = field_buffer(enum_field, 0);
                    is_whole_text = strcmp(text, yes_no_choices[0]) == 0;
                    stop = TRUE;
                }
                break;
            }
            default:
                form_driver(form, ch);
                wrefresh(win);
                break;
        }
    }

    curs_set(0);
    unpost_form(form);
    del_panel(panel);
    delwin(win);
    update_panels();
    doupdate();

    free_form(form);
    free_field(fields[0]);
    free_field(text_field);
    free_field(fields[2]);
    free_field(fields[3]);
}