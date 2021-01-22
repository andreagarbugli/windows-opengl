#include <iostream>

#include "Windows.h"
#include "GL/gl.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool initApplication(HINSTANCE hInstance);

HWND initInstance(HINSTANCE hInstance);

HDC deviceContext;
HGLRC renderingContext;

int main(int, char **)
{
    auto hInstance = GetModuleHandle(nullptr);

    if (!initApplication(hInstance))
    {
        return -1;
    }

    HWND hWnd = initInstance(hInstance);
    if (!hWnd)
    {
        printf("[error] impossible to initInstance!");
        return -1;
    }

    ShowWindow(hWnd, SW_SHOWDEFAULT);

    UpdateWindow(hWnd);

    MSG msg{};
    bool isRunning = true;
    while (isRunning)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                isRunning = false;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {

            glClearColor(1.0, 0.0, 0.0, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            SwapBuffers(deviceContext);
        }
    }

    return 0;
}

bool initApplication(HINSTANCE hInstance)
{
    WNDCLASSEX wcx{};
    // Fill in the window class structure with parameters
    // that describe the main window.

    wcx.cbSize = sizeof(wcx); // size of structure
    wcx.style = CS_HREDRAW |
                CS_VREDRAW;    // redraw if size changes
    wcx.lpfnWndProc = WndProc; // points to window procedure
    wcx.cbClsExtra = 0;        // no extra class memory
    wcx.cbWndExtra = 0;        // no extra window memory
    wcx.hInstance = hInstance, // handle to instance
        wcx.hIcon = LoadIcon(nullptr,
                             IDI_APPLICATION); // predefined app. icon
    wcx.hCursor = LoadCursor(nullptr,
                             IDC_ARROW); // predefined arrow
    wcx.hbrBackground = (HBRUSH)GetStockObject(
        WHITE_BRUSH);                 // white background brush
    wcx.lpszMenuName = "MainMenu";    // name of menu resource
    wcx.lpszClassName = "MainWClass"; // name of window class
    wcx.hIconSm = nullptr;

    // Register the window class.

    return RegisterClassEx(&wcx);
}

HWND initInstance(HINSTANCE hInstance)
{
    HWND hWnd = CreateWindow(
        "MainWClass",        // name of window class
        "Sample",            // title-bar string
        WS_OVERLAPPEDWINDOW, // top-level window
        CW_USEDEFAULT,       // default horizontal position
        CW_USEDEFAULT,       // default vertical position
        CW_USEDEFAULT,       // default width
        CW_USEDEFAULT,       // default height
        nullptr,             // no owner window
        nullptr,             // use class menu
        hInstance,           // handle to application instance
        nullptr);

    return hWnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        PIXELFORMATDESCRIPTOR pfd =
            {
                sizeof(PIXELFORMATDESCRIPTOR),
                1,
                PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // Flags
                PFD_TYPE_RGBA,                                              // The kind of framebuffer. RGBA or palette.
                32,                                                         // Colordepth of the framebuffer.
                0, 0, 0, 0, 0, 0,
                0,
                0,
                0,
                0, 0, 0, 0,
                24, // Number of bits for the depthbuffer
                8,  // Number of bits for the stencilbuffer
                0,  // Number of Aux buffers in the framebuffer.
                PFD_MAIN_PLANE,
                0,
                0, 0, 0};

        deviceContext = GetDC(hWnd);

        int pixelFormat = ChoosePixelFormat(deviceContext, &pfd);

        SetPixelFormat(deviceContext, pixelFormat, &pfd);

        renderingContext = wglCreateContext(deviceContext);
        wglMakeCurrent(deviceContext, renderingContext);

        MessageBox(0, (char *)glGetString(GL_VERSION), "OPENGL VERSION", 0);
    }
        return 0;
    case WM_CLOSE:
    {

        wglMakeCurrent(deviceContext, nullptr);
        wglDeleteContext(renderingContext);
        PostQuitMessage(0);
    }
        return 0;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}