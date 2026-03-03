#pragma once

#include "Asset/Model.h"

#include <memory>
#include <string>

namespace Axiom
{
    class ModelLoader
    {
    public:
        // Example:
        // auto model = Axiom::ModelLoader::Load("assets/models/spaceship.fbx");
        // if (model) { for (const auto& submesh : model->GetSubmeshes()) { /* submit to renderer */ } }
        static std::shared_ptr<Model> Load(const std::string& path);
    };
}
