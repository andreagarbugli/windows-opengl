add_executable(ProfessionalCmake "main.c")

target_link_libraries(ProfessionalCmake
    PRIVATE 
        OpenGL::GL
)

target_sources(ProfessionalCmake
    PRIVATE
        glad.c
        shader.h
        "shader.c"
        "window.c"
        window.h
        
 )