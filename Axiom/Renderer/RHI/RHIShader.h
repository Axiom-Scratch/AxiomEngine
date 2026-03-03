#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <memory>
#include <string>

namespace Axiom
{
    class RHIShader
    {
    public:
        virtual ~RHIShader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
        virtual void SetVec4(const std::string& name, const glm::vec4& value) = 0;
        virtual void SetInt(const std::string& name, int value) = 0;

        static std::unique_ptr<RHIShader> Create(const std::string& vertexSource, const std::string& fragmentSource);
    };
}
