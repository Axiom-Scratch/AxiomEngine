cmake_minimum_required(VERSION 3.20)

project(AxiomEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

if(TARGET glfw)
    if(MSVC)
        target_compile_options(glfw PRIVATE /W0)
    else()
        target_compile_options(glfw PRIVATE -w)
    endif()
endif()

add_subdirectory(external/glad)

add_subdirectory(Axiom)
add_subdirectory(Sandbox)
