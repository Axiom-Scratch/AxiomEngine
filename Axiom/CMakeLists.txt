add_library(Axiom STATIC)

target_sources(Axiom PRIVATE
    Asset/Model.cpp
    Asset/ModelLoader.cpp
    Core/Application.cpp
    Core/Log.cpp
    Core/Window.cpp
    Input/Input.cpp
    Platform/GLFW/GLFWInput.cpp
    Platform/GLFW/GLFWWindow.cpp
    Renderer/Camera.cpp
    Renderer/Material.cpp
    Renderer/Mesh.cpp
    Renderer/RenderCommand.cpp
    Renderer/Renderer.cpp
    Renderer/SceneRenderer.cpp
    Renderer/Texture2D.cpp
    Renderer/RHI/RHIBuffer.cpp
    Renderer/RHI/OpenGL/OpenGLBuffer.cpp
    Renderer/RHI/OpenGL/OpenGLRendererAPI.cpp
    Renderer/RHI/OpenGL/OpenGLShader.cpp
    Renderer/RHI/OpenGL/OpenGLTexture2D.cpp
    Renderer/RHI/OpenGL/OpenGLVertexArray.cpp
)

target_include_directories(Axiom
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/external/assimp/contrib
)

target_link_libraries(Axiom
    PRIVATE
        assimp
        glad
        glfw
)
