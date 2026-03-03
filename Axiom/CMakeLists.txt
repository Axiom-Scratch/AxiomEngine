add_library(Axiom STATIC)

target_sources(Axiom PRIVATE
    Core/Application.cpp
    Core/Log.cpp
    Core/Window.cpp
    Input/Input.cpp
    Platform/GLFW/GLFWInput.cpp
    Platform/GLFW/GLFWWindow.cpp
    RHI/RHI.cpp
    RHI/RHIBuffer.cpp
    RHI/RHIShader.cpp
    RHI/RHIVertexArray.cpp
    RHI/OpenGL/OpenGLBuffer.cpp
    RHI/OpenGL/OpenGLRHI.cpp
    RHI/OpenGL/OpenGLShader.cpp
    RHI/OpenGL/OpenGLVertexArray.cpp
    Renderer/Buffer.cpp
    Renderer/RenderCommand.cpp
    Renderer/Renderer.cpp
    Renderer/Shader.cpp
)

target_include_directories(Axiom
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Axiom
    PRIVATE
        glad
        glfw
)
