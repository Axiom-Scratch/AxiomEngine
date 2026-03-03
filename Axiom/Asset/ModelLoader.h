#pragma once

#include "Asset/Model.h"

#include <string>

namespace Axiom
{
    class ModelLoader
    {
    public:
        // Example:
        // auto model = Axiom::ModelLoader::Load("assets/models/spaceship.fbx");
        // for (const auto& submesh : model.GetSubmeshes()) { /* submit to renderer */ }
        static Model Load(const std::string& path);
    };
}
