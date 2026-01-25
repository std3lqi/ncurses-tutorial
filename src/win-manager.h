#ifndef TUTORIAL_WIN_MANAGER
#define TUTORIAL_WIN_MANAGER

typedef void (*WIN_KEY_HANDLE)(int);

void init_win_manager();
void register_window(WINDOW *win);
void next_window();
bool is_current_window(WINDOW *win);
void register_window_with_key_handle(WINDOW *win, 
    WIN_KEY_HANDLE handle);
void handle_key_in_current_window(int ch);

#endif