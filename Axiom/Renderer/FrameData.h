#pragma once

#include "Math/Math.h"

namespace Axiom
{
    struct FrameData
    {
        Math::Mat4 ViewProjection = Math::Identity();
    };
}
