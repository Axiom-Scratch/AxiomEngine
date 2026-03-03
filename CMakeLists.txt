cmake_minimum_required(VERSION 3.20)

project(AxiomEngine LANGUAGES C CXX)

# -------------------------------------------------
# C++ Standard
# -------------------------------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------------------------------------------------
# Global Warnings (Engine Only)
# -------------------------------------------------
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# -------------------------------------------------
# GLFW
# -------------------------------------------------
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

if (TARGET glfw)
    if (MSVC)
        target_compile_options(glfw PRIVATE /W0)
    else()
        target_compile_options(glfw PRIVATE -w)
    endif()
endif()

# -------------------------------------------------
# GLAD
# -------------------------------------------------
add_subdirectory(external/glad)

# -------------------------------------------------
# SPDLOG
# -------------------------------------------------
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)

add_subdirectory(external/spdlog)

# -------------------------------------------------
# ASSIMP (HARDENED)
# -------------------------------------------------

# Use internal zlib (VERY IMPORTANT)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_MINIZIP OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)

add_subdirectory(external/assimp)

# Silence third-party warnings
if (TARGET assimp)
    if (MSVC)
        target_compile_options(assimp PRIVATE /W0)
    else()
        target_compile_options(assimp PRIVATE -w)
    endif()
endif()

# -------------------------------------------------
# Engine + Sandbox
# -------------------------------------------------
add_subdirectory(Axiom)
add_subdirectory(Axiom/Editor)
add_subdirectory(Sandbox)

# -------------------------------------------------
# Game Asset Copy (Build Output)
# -------------------------------------------------
if (EXISTS ${CMAKE_SOURCE_DIR}/Sandbox/Assets)
    add_custom_target(CopyGameAssets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Sandbox/Assets
                $<TARGET_FILE_DIR:Sandbox>/Assets
    )

    add_dependencies(Sandbox CopyGameAssets)
endif()
