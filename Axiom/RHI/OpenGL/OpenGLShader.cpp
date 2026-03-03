#include "RHI/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>

namespace Axiom
{
    namespace
    {
        uint32_t CompileShader(uint32_t type, const std::string& source)
        {
            const uint32_t shader = glCreateShader(type);
            const char* src = source.c_str();
            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);

            int compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (compiled == GL_FALSE)
            {
                int length = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
                std::string infoLog(static_cast<size_t>(length), '\0');
                glGetShaderInfoLog(shader, length, &length, infoLog.data());
                std::fprintf(stderr, "Shader compile failure: %s\n", infoLog.c_str());
                glDeleteShader(shader);
                std::abort();
            }

            return shader;
        }
    }

    OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
    {
        const uint32_t program = glCreateProgram();
        const uint32_t vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
        const uint32_t fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        int linked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (linked == GL_FALSE)
        {
            int length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
            std::string infoLog(static_cast<size_t>(length), '\0');
            glGetProgramInfoLog(program, length, &length, infoLog.data());
            std::fprintf(stderr, "Shader link failure: %s\n", infoLog.c_str());
            glDeleteProgram(program);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            std::abort();
        }

        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        m_RendererID = program;
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        const int location = GetUniformLocation(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    int OpenGLShader::GetUniformLocation(const std::string& name) const
    {
        const auto cached = m_UniformLocationCache.find(name);
        if (cached != m_UniformLocationCache.end())
        {
            return cached->second;
        }

        const int location = glGetUniformLocation(m_RendererID, name.c_str());
        m_UniformLocationCache[name] = location;
        return location;
    }
}
