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
        // model.Draw();
        static Model Load(const std::string& path);
    };
}
