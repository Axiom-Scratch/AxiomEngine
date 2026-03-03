#pragma once

#include "Math/Math.h"

#include <memory>
#include <string>
#include <vector>

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
        void OnRender(SceneRenderer& renderer, const Math::Mat4& viewProjection);
        std::vector<Entity> GetAllEntities();
        std::unique_ptr<Scene> Clone() const;

    private:
        friend class Entity;

        std::unique_ptr<Detail::SceneRegistry> m_Registry;
    };
}
