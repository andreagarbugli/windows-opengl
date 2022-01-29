#include "window.h"

#include "glad.h"
#include "wglext.h"

static LRESULT CALLBACK msg_router(HWND h_wnd, UINT msg,
                                   WPARAM w_param, LPARAM l_param)
{
    window_t *win = (window_t *)GetWindowLongPtr(h_wnd, GWLP_USERDATA);

    // call the windows message handler
    return Window_handle_messages(win, h_wnd, msg, w_param, l_param);
}

static LRESULT CALLBACK msg_setup(HWND h_wnd, UINT msg,
                                  WPARAM w_param, LPARAM l_param)
{
    if (msg == WM_NCCREATE)
    {
        // retrieve Window instance from window creation data and associate
        CREATESTRUCT *create = (CREATESTRUCT *)l_param;
        window_t *win = (window_t *)create->lpCreateParams;

        SetWindowLongPtr(h_wnd, GWLP_USERDATA, (LONG_PTR)win);

        SetWindowLongPtr(h_wnd, GWLP_WNDPROC, (LONG_PTR)msg_router);
    }

    return DefWindowProc(h_wnd, msg, w_param, l_param);
}

static ATOM create_window_class(HINSTANCE h_instance)
{
    // Fill in the window class structure with parameters
    // that describe the main window.
    WNDCLASSEX wcx = {0};
    wcx.cbSize = sizeof(wcx);                                // size of structure
    wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;          // redraw if size changes
    wcx.lpfnWndProc = msg_setup;                             // points to window procedure
    wcx.cbClsExtra = 0;                                      // no extra class memory
    wcx.cbWndExtra = 0;                                      // no extra window memory
    wcx.hInstance = h_instance;                              // handle to instance
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);             // predefined app. icon
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);               // predefined arrow
    wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // white background brush
    wcx.lpszMenuName = "MainMenu";                           // name of menu resource
    wcx.lpszClassName = "MainWClass";                        // name of window class
    wcx.hIconSm = NULL;

    // Register the window class.
    return RegisterClassEx(&wcx);
}

window_t Window_new(i32 width, i32 height, char *title)
{
    window_t win = {0};
    win.height = height;
    win.width = width;
    win.title = title;

    HINSTANCE h_instance = GetModuleHandle(NULL);
    create_window_class(h_instance);

    RECT clientRect = {0, 0, width, height};
    AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);

    win.fake_handle = CreateWindowEx(
        0,
        "MainWClass",        // name of window class
        "Sample",            // title-bar string
        WS_OVERLAPPEDWINDOW, // top-level window
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL, NULL, // use class menu
        win.h_instance,
        (void *)&win);

    return win;
}

void Window_destroy(window_t *win)
{
    UnregisterClass("MainWClass", win->h_instance);
    DestroyWindow(win->fake_handle);
    DestroyWindow(win->handle);
}

void Window_init(window_t *win)
{
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags =
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // Flags
    pfd.iPixelType = PFD_TYPE_RGBA;                                 // The kind of framebuffer. RGBA or palette.
    pfd.cColorBits = 32;                                            // Colordepth of the framebuffer.
    pfd.cDepthBits = 24;                                            // Number of bits for the depthbuffer
    pfd.cStencilBits = 8;                                           // Number of bits for the stencilbuffer
    pfd.iLayerType = PFD_MAIN_PLANE;

    HDC fake_dc = GetDC(win->fake_handle);

    int pf_index = ChoosePixelFormat(fake_dc, &pfd);

    PIXELFORMATDESCRIPTOR suggested_pf = {0};
    DescribePixelFormat(fake_dc, pf_index,
                        sizeof(suggested_pf), &suggested_pf);

    SetPixelFormat(fake_dc, pf_index, &suggested_pf);

    HGLRC fake_rendering_ctx = wglCreateContext(fake_dc);
    wglMakeCurrent(fake_dc, fake_rendering_ctx);

    PFNWGLCHOOSEPIXELFORMATEXTPROC wglChoosePixelFormatARB =
        (PFNWGLCHOOSEPIXELFORMATEXTPROC)wglGetProcAddress("wglChoosePixelFormatARB");

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    win->handle = CreateWindowEx(
        0, "MainWClass", "Sample",
        WS_OVERLAPPEDWINDOW, // top-level window
        CW_USEDEFAULT, CW_USEDEFAULT,
        win->width, win->height,
        NULL, NULL,
        win->h_instance, // handle to application instance
        (void *)win);

    win->dev_ctx = GetDC(win->handle);

    const int pixel_attribs[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_ALPHA_BITS_ARB, 8,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
        WGL_SAMPLES_ARB, 4,
        0};

    int pf_id;
    UINT num_formats;
    wglChoosePixelFormatARB(win->dev_ctx, pixel_attribs,
                            NULL, 1, &pf_id, &num_formats);

    PIXELFORMATDESCRIPTOR real_pfd;
    DescribePixelFormat(win->dev_ctx, pf_id, sizeof(real_pfd), &real_pfd);
    SetPixelFormat(win->dev_ctx, pf_id, &real_pfd);

    const int major_min = 4;
    const int minor_min = 2;
    int context_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
        WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0};

    win->rendering_ctx = wglCreateContextAttribsARB(win->dev_ctx, 0, context_attribs);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(fake_rendering_ctx);

    ReleaseDC(win->fake_handle, fake_dc);

    wglMakeCurrent(win->dev_ctx, win->rendering_ctx);

    ShowWindow(win->handle, SW_SHOW);
}

void Window_swap_buffers(window_t *win)
{
    SwapBuffers(win->dev_ctx);
}

LRESULT Window_handle_messages(window_t *win, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_SIZE:
    {
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);
        win->width = width;
        win->height = height;
        return 0;
    }
    case WM_CLOSE:
    {
        wglMakeCurrent(win->dev_ctx, NULL);
        wglDeleteContext(win->rendering_ctx);
        ReleaseDC(hWnd, win->dev_ctx);
        PostQuitMessage(0);
        return 0;
    }

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}