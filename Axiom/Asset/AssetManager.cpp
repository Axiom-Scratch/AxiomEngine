#include "Asset/AssetManager.h"

#include "Asset/ModelLoader.h"
#include "Renderer/Texture2D.h"

namespace Axiom
{
    std::unordered_map<std::string, std::shared_ptr<Model>> AssetManager::s_ModelCache{};
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> AssetManager::s_TextureCache{};

    std::shared_ptr<Model> AssetManager::LoadModel(const std::string& path)
    {
        auto it = s_ModelCache.find(path);
        if (it != s_ModelCache.end())
        {
            return it->second;
        }

        auto model = ModelLoader::Load(path);
        if (!model)
        {
            return nullptr;
        }
        s_ModelCache.emplace(path, model);
        return model;
    }

    std::shared_ptr<Texture2D> AssetManager::LoadTexture(const std::string& path)
    {
        auto it = s_TextureCache.find(path);
        if (it != s_TextureCache.end())
        {
            return it->second;
        }

        auto texture = Texture2D::CreateFromFile(path);
        if (!texture)
        {
            return nullptr;
        }

        s_TextureCache.emplace(path, texture);
        return texture;
    }
}
