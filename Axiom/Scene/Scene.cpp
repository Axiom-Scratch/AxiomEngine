#include "Scene/Scene.h"

#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Scene/SceneRegistry.h"

#include "Renderer/SceneRenderer.h"

namespace Axiom
{
    namespace
    {
        template<typename T>
        void CopyIfExists(entt::registry& dstRegistry, entt::registry& srcRegistry,
            entt::entity dstEntity, entt::entity srcEntity)
        {
            if (srcRegistry.all_of<T>(srcEntity))
            {
                dstRegistry.emplace<T>(dstEntity, srcRegistry.get<T>(srcEntity));
            }
        }
    }

    Scene::Scene()
        : m_Registry(std::make_unique<Detail::SceneRegistry>())
    {
        auto camera = CreateEntity("Main Camera");
        camera.AddComponent<CameraComponent>();
        camera.GetComponent<TransformComponent>().Translation = Math::Vec3(0.0f, 0.0f, 5.0f);
    }

    Scene::~Scene() = default;

    Entity Scene::CreateEntity(const std::string& name)
    {
        auto& registry = m_Registry->Registry;
        const entt::entity handle = registry.create();
        Entity entity(Detail::ToEntityHandle(handle), this);

        entity.AddComponent<IDComponent>();
        entity.AddComponent<TransformComponent>();

        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;

        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        if (entity)
        {
            m_Registry->Registry.destroy(Detail::ToEnttEntity(entity.m_EntityHandle));
        }
    }

    void Scene::OnUpdate(float)
    {
    }

    void Scene::OnRender(SceneRenderer& renderer)
    {
        Camera* primaryCamera = nullptr;

        auto& registry = m_Registry->Registry;
        auto cameraView = registry.view<TransformComponent, CameraComponent>();
        for (const auto entity : cameraView)
        {
            auto& transform = cameraView.get<TransformComponent>(entity);
            auto& camera = cameraView.get<CameraComponent>(entity);
            if (!camera.Primary)
            {
                continue;
            }

            camera.CameraData.SetPosition(transform.Translation);
            camera.CameraData.SetRotation(transform.Rotation.y, transform.Rotation.x);
            primaryCamera = &camera.CameraData;
            break;
        }

        if (!primaryCamera)
        {
            return;
        }

        renderer.BeginScene(*primaryCamera);

        auto view = registry.view<TransformComponent, MeshComponent>();
        for (const auto entity : view)
        {
            auto& transform = view.get<TransformComponent>(entity);
            auto& mesh = view.get<MeshComponent>(entity);
            renderer.Submit(mesh.MeshPtr, transform.GetTransform());
        }

        renderer.EndScene();
    }

    void Scene::OnRender(SceneRenderer& renderer, const Math::Mat4& viewProjection)
    {
        auto& registry = m_Registry->Registry;
        renderer.BeginScene(viewProjection);

        auto view = registry.view<TransformComponent, MeshComponent>();
        for (const auto entity : view)
        {
            auto& transform = view.get<TransformComponent>(entity);
            auto& mesh = view.get<MeshComponent>(entity);
            renderer.Submit(mesh.MeshPtr, transform.GetTransform());
        }

        renderer.EndScene();
    }

    std::vector<Entity> Scene::GetAllEntities()
    {
        std::vector<Entity> entities;
        auto& registry = m_Registry->Registry;
        auto view = registry.view<IDComponent>();
        for (const auto entity : view)
        {
            Entity wrapper(Detail::ToEntityHandle(entity), this);
            entities.push_back(wrapper);
        }
        return entities;
    }

    std::unique_ptr<Scene> Scene::Clone() const
    {
        auto clone = std::make_unique<Scene>();
        auto& srcRegistry = m_Registry->Registry;
        auto& dstRegistry = clone->m_Registry->Registry;

        auto view = srcRegistry.view<IDComponent>();
        for (const auto srcEntity : view)
        {
            const auto dstEntity = dstRegistry.create();

            dstRegistry.emplace<IDComponent>(dstEntity, srcRegistry.get<IDComponent>(srcEntity));
            CopyIfExists<TagComponent>(dstRegistry, srcRegistry, dstEntity, srcEntity);
            CopyIfExists<TransformComponent>(dstRegistry, srcRegistry, dstEntity, srcEntity);
            CopyIfExists<MeshComponent>(dstRegistry, srcRegistry, dstEntity, srcEntity);
            CopyIfExists<ModelComponent>(dstRegistry, srcRegistry, dstEntity, srcEntity);
            CopyIfExists<CameraComponent>(dstRegistry, srcRegistry, dstEntity, srcEntity);
        }

        return clone;
    }
}
