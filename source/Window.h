#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>

#include "common.h"

typedef struct window {
    i32 width;
    i32 height;

    HDC dev_ctx;
    HGLRC rendering_ctx;

    char *title;

    HWND handle;
    HWND fake_handle;

    HINSTANCE h_instance;
} window_t;

window_t Window_new(i32 width, i32 height, char *title);

void Window_init(window_t *win);

void Window_destroy(window_t *win);

void Window_swap_buffers(window_t *win);

LRESULT Window_handle_messages(window_t *win, HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param);

#endif // WINDOW_H