#pragma once

#include <glm/mat4x4.hpp>

#include <memory>
#include <string>

namespace Axiom
{
    class RHIShader;

    class Shader
    {
    public:
        ~Shader();

        static std::shared_ptr<Shader> Create(const std::string& vertexSource, const std::string& fragmentSource);

        void Bind() const;
        void Unbind() const;
        void SetMat4(const std::string& name, const glm::mat4& value);

    private:
        explicit Shader(std::unique_ptr<RHIShader> shader);

        std::unique_ptr<RHIShader> m_Shader;
    };
}
