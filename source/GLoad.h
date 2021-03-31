#ifndef G_LOAD_H
#define G_LOAD_H

typedef void (*typeglGenBuffers)(GLsizei n, GLuint *buffers);
typedef void (*typeglDeleteBuffers)(GLsizei n, GLuint *buffers);
typedef void (*typeglBindBuffer)(GLenum target, GLuint buffer);
typedef void (*typeglBufferData)(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
typedef GLuint (*typeglCreateShader)(GLenum shaderType);
typedef void (*typeglShaderSource)(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
typedef void (*typeglCompileShader)(GLuint shader);
typedef GLuint (*typeglCreateProgram)(void);
typedef void (*typeglAttachShader)(GLuint program, GLuint shader);
typedef void (*typeglLinkProgram)(GLuint program);
typedef void (*typeglUseProgram)(GLuint program);
typedef void (*typeglDeleteProgram)(GLuint program);
typedef void (*typeglDeleteShader)(GLuint shader);
typedef void (*typeglVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
typedef void (*typeglEnableVertexAttribArray)(GLuint index);
typedef void (*typeglDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
typedef void (*typeglGetShaderiv)(GLuint shader, GLenum pname, GLint *params);
typedef void (*typeglGetShaderInfoLog)(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void (*typeglGetProgramiv)(GLuint program, GLenum pname, GLint *params);
typedef void (*typeglGenVertexArrays)(GLsizei n, GLuint *arrays);
typedef void (*typeglGetProgramInfoLog)(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void (*typeglBindVertexArray)(GLuint array);

typeglGenBuffers glGenBuffers = nullptr;
typeglDeleteBuffers glDeleteBuffers = nullptr;
typeglBindBuffer glBindBuffer = nullptr;
typeglBufferData glBufferData = nullptr;
typeglCreateShader glCreateShader = nullptr;
typeglShaderSource glShaderSource = nullptr;
typeglCompileShader glCompileShader = nullptr;
typeglCreateProgram glCreateProgram = nullptr;
typeglAttachShader glAttachShader = nullptr;
typeglLinkProgram glLinkProgram = nullptr;
typeglUseProgram glUseProgram = nullptr;
typeglDeleteProgram glDeleteProgram = nullptr;
typeglDeleteShader glDeleteShader = nullptr;
typeglVertexAttribPointer glVertexAttribPointer = nullptr;
typeglEnableVertexAttribArray glEnableVertexAttribArray = nullptr;
typeglDeleteVertexArrays glDeleteVertexArrays = nullptr;
typeglGetShaderiv glGetShaderiv = nullptr;
typeglGetShaderInfoLog glGetShaderInfoLog = nullptr;
typeglGetProgramiv glGetProgramiv = nullptr;
typeglGenVertexArrays glGenVertexArrays = nullptr;
typeglGetProgramInfoLog glGetProgramInfoLog = nullptr;
typeglBindVertexArray glBindVertexArray = nullptr;

#if defined(_WIN32)
#include <wingdi.h>
#endif

void loadOpenGLFunctions()
{
#if defined(_WIN32)
    glGenBuffers = (typeglGenBuffers)wglGetProcAddress("glGenBuffers");
    glDeleteBuffers = (typeglDeleteBuffers)wglGetProcAddress("glDeleteBuffers");
    glBindBuffer = (typeglBindBuffer)wglGetProcAddress("glBindBuffer");
    glBufferData = (typeglBufferData)wglGetProcAddress("glBufferData");
    glCreateShader = (typeglCreateShader)wglGetProcAddress("glCreateShader");
    glShaderSource = (typeglShaderSource)wglGetProcAddress("glShaderSource");
    glCompileShader = (typeglCompileShader)wglGetProcAddress("glCompileShader");
    glCreateProgram = (typeglCreateProgram)wglGetProcAddress("glCreateProgram");
    glAttachShader = (typeglAttachShader)wglGetProcAddress("glAttachShader");
    glLinkProgram = (typeglLinkProgram)wglGetProcAddress("glLinkProgram");
    glUseProgram = (typeglUseProgram)wglGetProcAddress("glUseProgram");
    glDeleteProgram = (typeglDeleteProgram)wglGetProcAddress("glDeleteProgram");
    glDeleteShader = (typeglDeleteShader)wglGetProcAddress("glDeleteShader");
    glVertexAttribPointer = (typeglVertexAttribPointer)wglGetProcAddress("glVertexAttribPointer");
    glEnableVertexAttribArray = (typeglEnableVertexAttribArray)wglGetProcAddress("glEnableVertexAttribArray");
    glDeleteVertexArrays = (typeglDeleteVertexArrays)wglGetProcAddress("glDeleteVertexArrays");
    glGetShaderiv = (typeglGetShaderiv)wglGetProcAddress("glGetShaderiv");
    glGetShaderInfoLog = (typeglGetShaderInfoLog)wglGetProcAddress("glGetShaderInfoLog");
    glGetProgramiv = (typeglGetProgramiv)wglGetProcAddress("glGetProgramiv");
    glGenVertexArrays = (typeglGenVertexArrays)wglGetProcAddress("glGenVertexArrays");
    glGetProgramInfoLog = (typeglGetProgramInfoLog)wglGetProcAddress("glGetProgramInfoLog");
    glBindVertexArray = (typeglBindVertexArray)wglGetProcAddress("glBindVertexArray");
#endif
}

#endif // G_LOAD_H