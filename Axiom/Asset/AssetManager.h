#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace Axiom
{
    class Model;
    class Texture2D;

    class AssetManager
    {
    public:
        static std::shared_ptr<Model> LoadModel(const std::string& path);
        static std::shared_ptr<Texture2D> LoadTexture(const std::string& path);

    private:
        static std::unordered_map<std::string, std::shared_ptr<Model>> s_ModelCache;
        static std::unordered_map<std::string, std::shared_ptr<Texture2D>> s_TextureCache;
    };
}
