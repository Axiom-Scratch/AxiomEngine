#include "Scene/Scene.h"

#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Scene/SceneRegistry.h"

#include "Asset/Model.h"
#include "Renderer/SceneRenderer.h"

namespace Axiom
{
    Scene::Scene()
        : m_Registry(std::make_unique<Detail::SceneRegistry>())
    {
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

        auto modelView = registry.view<TransformComponent, ModelComponent>();
        for (const auto entity : modelView)
        {
            auto& transform = modelView.get<TransformComponent>(entity);
            auto& model = modelView.get<ModelComponent>(entity);
            if (!model.ModelPtr)
            {
                continue;
            }

            const auto entityTransform = transform.GetTransform();
            for (const auto& submesh : model.ModelPtr->GetSubmeshes())
            {
                if (!submesh.MeshPtr || !submesh.MaterialPtr)
                {
                    continue;
                }
                renderer.Submit(submesh.MeshPtr, submesh.MaterialPtr, entityTransform * submesh.Transform);
            }
        }

        renderer.EndScene();
    }
}
