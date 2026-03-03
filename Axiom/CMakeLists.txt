add_library(Axiom STATIC)

target_sources(Axiom PRIVATE
    Core/Application.cpp
    Core/Window.cpp
    Input/Input.cpp
    Platform/GLFW/GLFWInput.cpp
    Platform/GLFW/GLFWWindow.cpp
)

target_include_directories(Axiom
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Axiom
    PRIVATE glfw
)
