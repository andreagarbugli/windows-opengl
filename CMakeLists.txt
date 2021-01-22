cmake_minimum_required(VERSION 3.2)
project(ProfessionalCmake 
        VERSION 0.1.0
        LANGUAGES CXX C
    )

find_package(OpenGL REQUIRED)

add_executable(ProfessionalCmake source/main.cpp)

target_link_libraries(
    ProfessionalCmake
    PRIVATE OpenGL::GL
)

# In questo modo creo un app Win32 Desktop e che quindi sfrutta
# WinMain()
# add_executable(ProfessionalCmake WIN32 source/main.cpp)