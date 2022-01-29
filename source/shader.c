#include "shader.h"

#include <stdio.h>

#include "glad.h"

shader_t Shader_create(GLenum shader_type, const char *shader_src)
{
    GLuint shader_id = glCreateShader(shader_type);

    glShaderSource(shader_id, 1, &shader_src, NULL);

    glCompileShader(shader_id);

    // check for shader compile errors
    GLint success;
    char info_log[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_id, 512, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n", info_log);
    }

    shader_t result = {0};
    result.id = shader_id;
    return result;
}

shader_program_t Shader_Program_create(shader_t *v_shader, shader_t *f_shader)
{
    // link shaders
    GLuint shader_prog_id = glCreateProgram();
    glAttachShader(shader_prog_id, v_shader->id);
    glAttachShader(shader_prog_id, f_shader->id);
    glLinkProgram(shader_prog_id);
 
    GLint success;
    char info_log[512];
    // check for linking errors
    glGetProgramiv(shader_prog_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_prog_id, 512, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED: %s", info_log);
    }

    glDeleteShader(v_shader->id);
    glDeleteShader(f_shader->id);

    shader_program_t result = {0};
    result.id = shader_prog_id;

    return result;
}

i32 Shader_Program__delete(shader_program_t *prog) {
	glDeleteProgram(shader_prog.id);
}
