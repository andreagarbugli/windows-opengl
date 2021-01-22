#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <GL/gl.h>
#include <cstdint>
#include <string>

class Window
{
private:
    int32_t width_;
    int32_t height_;

    HDC deviceContext_;
    HGLRC renderingContext_;

    std::string title_;

    HWND handle_;

    HINSTANCE hInstance_;

public:
    Window(int32_t width, int32_t height, std::string &title);

    ~Window();

    void init();

    void swapBuffers();

    HWND getHandle() const;

    int32_t getWidth() const;

    int32_t getHeight() const;

private:
    bool _createWindowClass(HINSTANCE hInstance);

    LRESULT handleMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK msgSetup(HWND hWnd, UINT msg,
                                     WPARAM wParam, LPARAM lParam)
    {
        if (msg == WM_NCCREATE)
        {
            // retrieve Window instance from window creation data and associate
            const CREATESTRUCT *const create = reinterpret_cast<CREATESTRUCT *>(lParam);
            auto *const window = static_cast<Window *>(create->lpCreateParams);

            SetWindowLongPtr(hWnd, GWLP_USERDATA,
                             reinterpret_cast<LONG_PTR>(window));

            SetWindowLongPtr(hWnd, GWLP_WNDPROC,
                             reinterpret_cast<LONG_PTR>(&Window::msgRouter));
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    static LRESULT CALLBACK msgRouter(HWND hWnd, UINT msg,
                                      WPARAM wParam, LPARAM lParam)
    {
        auto *const window = reinterpret_cast<Window *>(
            GetWindowLongPtr(hWnd, GWLP_USERDATA));

        // call the windows message handler
        return window->handleMessages(hWnd, msg, wParam, lParam);
    }
};

#endif // WINDOW_H