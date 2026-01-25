#ifndef TUTORIAL_WIN_MANAGER
#define TUTORIAL_WIN_MANAGER

void init_win_manager();
void register_window(WINDOW *win);
void next_window();
bool is_current_window(WINDOW *win);

#endif