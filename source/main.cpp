#include <string>

#include "Window.h"

int main(int, char **)
{
    Window window{800, 600, std::string{"Hello Window"}};

    window.init();

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
            glViewport(0, 0, window.getWidth(), window.getHeight());
            glClearColor(1.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);

            window.swapBuffers();
        }
    }

    return 0;
}