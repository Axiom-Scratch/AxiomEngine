#include "RHI/OpenGL/OpenGLRHI.h"

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
    void OpenGLRHI::Init()
    {
        if (gladLoadGL(LoadGLProc) == 0)
        {
            std::abort();
        }

        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRHI::Clear(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRHI::DrawIndexed(uint32_t count)
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRHI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    }
}
