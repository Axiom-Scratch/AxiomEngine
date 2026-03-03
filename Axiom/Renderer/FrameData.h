#pragma once

#include <glm/mat4x4.hpp>

namespace Axiom
{
    struct FrameData
    {
        glm::mat4 ViewProjection{1.0f};
    };
}
