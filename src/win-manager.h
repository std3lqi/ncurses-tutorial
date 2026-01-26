#ifndef TUTORIAL_WIN_MANAGER
#define TUTORIAL_WIN_MANAGER

typedef void (*WIN_KEY_HANDLE)(int);
typedef void (*WIN_MOUSE_HANDLE)(MEVENT *event);
typedef void (*WIN_REFRESH)();

void init_win_manager();
void register_window(WINDOW *win, WIN_REFRESH refresh_func);
void next_window();
bool is_current_window(WINDOW *win);
void register_window_with_key_handle(WINDOW *win, 
    WIN_REFRESH refresh_func,
    WIN_KEY_HANDLE key_handle,
    WIN_MOUSE_HANDLE mouse_handle);
void handle_key_in_current_window(int ch);

#endif