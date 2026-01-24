#ifndef TUTORIAL_DEBUG_WIN
#define TUTORIAL_DEBUG_WIN

void create_debug_window(int h, int w, int y, int x);
void delete_debug_window();
void debug_line(const char *fmt, ...);

#endif