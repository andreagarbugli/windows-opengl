#include "Window.h"

Window::Window(int32_t width, int32_t height, std::string &title)
    : width_{width},
      height_{height},
      title_{std::move(title)}
{
    hInstance_ = GetModuleHandle(nullptr);

    _createWindowClass(hInstance_);

    RECT clientRect{0, 0, width_, height_};
    AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);

    handle_ = CreateWindowEx(
        0,
        "MainWClass",        // name of window class
        "Sample",            // title-bar string
        WS_OVERLAPPEDWINDOW, // top-level window
        CW_USEDEFAULT,       // default horizontal position
        CW_USEDEFAULT,       // default vertical position
        width_,              // default width
        height_,             // default height
        nullptr,             // no owner window
        nullptr,             // use class menu
        hInstance_,          // handle to application instance
        this);

    ShowWindow(handle_, SW_SHOWDEFAULT);

    UpdateWindow(handle_);
}

Window::~Window()
{
    UnregisterClass("MainWClass", hInstance_);
    DestroyWindow(handle_);
}

void Window::init()
{
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // Flags
    pfd.iPixelType = PFD_TYPE_RGBA;                                           // The kind of framebuffer. RGBA or palette.
    pfd.cColorBits = 32;                                                      // Colordepth of the framebuffer.
    pfd.cDepthBits = 24;                                                      // Number of bits for the depthbuffer
    pfd.cStencilBits = 8;                                                     // Number of bits for the stencilbuffer
    pfd.iLayerType = PFD_MAIN_PLANE;

    deviceContext_ = GetDC(handle_);

    int pixelFormatIndex = ChoosePixelFormat(deviceContext_, &pfd);

    PIXELFORMATDESCRIPTOR suggestedPixelFormat = {};
    DescribePixelFormat(deviceContext_, pixelFormatIndex,
                        sizeof(suggestedPixelFormat), &suggestedPixelFormat);

    SetPixelFormat(deviceContext_, pixelFormatIndex, &suggestedPixelFormat);

    renderingContext_ = wglCreateContext(deviceContext_);
    wglMakeCurrent(deviceContext_, renderingContext_);
}

void Window::swapBuffers()
{
    SwapBuffers(deviceContext_);
}

HWND Window::getHandle() const
{
    return handle_;
}

int32_t Window::getWidth() const
{
    return width_;
}

int32_t Window::getHeight() const
{
    return height_;
}

bool Window::_createWindowClass(HINSTANCE hInstance)
{
    // Fill in the window class structure with parameters
    // that describe the main window.
    WNDCLASSEX wcx{};
    wcx.cbSize = sizeof(wcx);                                             // size of structure
    wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;                       // redraw if size changes
    wcx.lpfnWndProc = Window::msgRouter;                                  // points to window procedure
    wcx.cbClsExtra = 0;                                                   // no extra class memory
    wcx.cbWndExtra = 0;                                                   // no extra window memory
    wcx.hInstance = hInstance;                                            // handle to instance
    wcx.hIcon = LoadIcon(nullptr, IDI_APPLICATION);                       // predefined app. icon
    wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);                         // predefined arrow
    wcx.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); // white background brush
    wcx.lpszMenuName = "MainMenu";                                        // name of menu resource
    wcx.lpszClassName = "MainWClass";                                     // name of window class
    wcx.hIconSm = nullptr;

    // Register the window class.
    return RegisterClassEx(&wcx);
}

LRESULT Window::handleMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
    {
        wglMakeCurrent(deviceContext_, nullptr);
        wglDeleteContext(renderingContext_);
        ReleaseDC(hWnd, deviceContext_);
        PostQuitMessage(0);
    }
        return 0;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}