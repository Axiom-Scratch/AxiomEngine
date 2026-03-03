#pragma once

#include "RHI/RHIShader.h"

#include <cstdint>
#include <unordered_map>

namespace Axiom
{
    class OpenGLShader final : public RHIShader
    {
    public:
        OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
        ~OpenGLShader() override;

        void Bind() const override;
        void Unbind() const override;
        void SetMat4(const std::string& name, const glm::mat4& value) override;

    private:
        int GetUniformLocation(const std::string& name) const;

        uint32_t m_RendererID = 0;
        mutable std::unordered_map<std::string, int> m_UniformLocationCache;
    };
}
