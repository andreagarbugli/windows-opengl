#include <stdbool.h>
#include <stdio.h>

#include "window.h"
#include "glad.h"
#include "shader.h"

const char *vertex_shader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragment_shader_src =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main(int argc, char *argv[])
{
    window_t win = Window_new(800, 600, "Hello Window");

    Window_init(&win);

    gladLoadGL();

    // vertex shader
    shader_t v_shader = Shader_create(GL_VERTEX_SHADER, vertex_shader_src);

    // fragment shader
    shader_t f_shader = Shader_create(GL_FRAGMENT_SHADER, fragment_shader_src);

    shader_program_t shader_prog = Shader_Program_create(&v_shader, &f_shader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f,  // right
        0.0f, 0.5f, 0.0f    // top
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO
	// as the vertex attribute's bound vertex buffer object so afterwards
	// we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally 
	// modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally 
	// don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    MSG msg = {0};
    bool is_running = true;
    while (is_running)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                is_running = false;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // draw our first triangle
            glUseProgram(shader_prog.id);
			// seeing as we only have a single VAO there's no need to bind it every 
			// time, but we'll do so to keep things a bit more organized
			glBindVertexArray(VAO); 
			glDrawArrays(GL_TRIANGLES, 0, 3);
            // glBindVertexArray(0); // no need to unbind it every time

            Window_swap_buffers(&win);
        }
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

	Shader_Program__delete(&shader_prog);

    Window_destroy(&win);

    return 0;
}