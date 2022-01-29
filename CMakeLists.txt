cmake_minimum_required(VERSION 3.2)
project(ProfessionalCmake 
        VERSION 0.1.0
        LANGUAGES CXX C
    )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

find_package(OpenGL REQUIRED)

add_subdirectory(source)

# In questo modo creo un app Win32 Desktop e che quindi sfrutta
# WinMain()
# add_executable(ProfessionalCmake WIN32 source/main.cpp)