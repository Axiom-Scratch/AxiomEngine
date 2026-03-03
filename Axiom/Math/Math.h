#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Axiom::Math
{
    using Vec2 = glm::vec2;
    using Vec3 = glm::vec3;
    using Vec4 = glm::vec4;

    using Mat4 = glm::mat4;

    inline Mat4 Identity()
    {
        return glm::mat4(1.0f);
    }

    inline Mat4 Translate(const Mat4& m, const Vec3& v)
    {
        return glm::translate(m, v);
    }

    inline Mat4 Rotate(const Mat4& m, float angle, const Vec3& axis)
    {
        return glm::rotate(m, angle, axis);
    }

    inline Mat4 Scale(const Mat4& m, const Vec3& s)
    {
        return glm::scale(m, s);
    }

    inline Mat4 Perspective(float fov, float aspect, float nearClip, float farClip)
    {
        return glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
    }
}
