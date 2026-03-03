#pragma once

#include <memory>
#include <string>

namespace Axiom
{
    class Entity;
    class SceneRenderer;
    namespace Detail
    {
        struct SceneRegistry;
    }

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = "");
        void DestroyEntity(Entity entity);

        void OnUpdate(float deltaTime);
        void OnRender(SceneRenderer& renderer);

    private:
        friend class Entity;

        std::unique_ptr<Detail::SceneRegistry> m_Registry;
    };
}
