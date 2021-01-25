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

            glColor3f(1.0f, 0.0f, 1.0f);
            glBegin(GL_TRIANGLES);
            glVertex2d(0.5f, -0.5f);
            glVertex2d(-0.5f, -0.5f);
            glVertex2d(0.0f, 0.5f);
            glEnd();

            window.swapBuffers();
        }
    }

    return 0;
}