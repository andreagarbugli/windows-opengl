#if !defined(SHADER_H)
#define SHADER_H

#include "glad.h"

#include "common.h"

typedef struct shader {
    u32 id;
} shader_t;

shader_t Shader_create(GLenum shader_type, const char *shader_src);

typedef struct shader_program {
    u32 id;
} shader_program_t;

shader_program_t Shader_Program_create(shader_t *v_shader, shader_t *f_shader);

i32 Shader_Program__delete(shader_program_t *prog);

#endif // SHADER_H
