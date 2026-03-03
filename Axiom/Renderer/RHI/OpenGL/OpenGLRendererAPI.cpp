#include "Renderer/RHI/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cstdlib>

namespace
{
    GLADapiproc LoadGLProc(const char* name)
    {
        return reinterpret_cast<GLADapiproc>(glfwGetProcAddress(name));
    }
}

namespace Axiom
{
    std::unique_ptr<RendererAPI> RendererAPI::Create(RendererAPIType type)
    {
        switch (type)
        {
            case RendererAPIType::OpenGL:
                return std::make_unique<OpenGLRendererAPI>();
            case RendererAPIType::None:
                return nullptr;
        }
        return nullptr;
    }

    void OpenGLRendererAPI::Init()
    {
        if (gladLoadGL(LoadGLProc) == 0)
        {
            std::abort();
        }

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_FRAMEBUFFER_SRGB);
    }

    void OpenGLRendererAPI::Clear(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(uint32_t indexCount)
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    }
}
