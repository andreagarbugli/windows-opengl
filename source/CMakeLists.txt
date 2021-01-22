add_executable(ProfessionalCmake main.cpp)

target_link_libraries(
    ProfessionalCmake
    PRIVATE OpenGL::GL
)

target_sources(ProfessionalCmake
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Window.h
)