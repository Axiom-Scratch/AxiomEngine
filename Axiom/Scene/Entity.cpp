#include "Scene/Entity.h"

#include "Scene/Scene.h"
#include "Scene/SceneRegistry.h"

#include <exception>
#include <utility>

namespace Axiom
{
    Entity::Entity(Handle handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {
    }

    bool Entity::IsValid() const
    {
        return m_Scene != nullptr && m_EntityHandle != Detail::InvalidEntity;
    }

    Entity::operator bool() const
    {
        return IsValid();
    }

    template<typename T, typename... Args>
    T& Entity::AddComponent(Args&&... args)
    {
        if (!IsValid())
        {
            std::terminate();
        }

        auto& registry = m_Scene->m_Registry->Registry;
        return registry.emplace<T>(Detail::ToEnttEntity(m_EntityHandle), std::forward<Args>(args)...);
    }

    template<typename T>
    T& Entity::GetComponent()
    {
        if (!IsValid())
        {
            std::terminate();
        }

        auto& registry = m_Scene->m_Registry->Registry;
        return registry.get<T>(Detail::ToEnttEntity(m_EntityHandle));
    }

    template<typename T>
    bool Entity::HasComponent() const
    {
        if (!IsValid())
        {
            return false;
        }

        auto& registry = m_Scene->m_Registry->Registry;
        return registry.all_of<T>(Detail::ToEnttEntity(m_EntityHandle));
    }

    template<typename T>
    void Entity::RemoveComponent()
    {
        if (!IsValid())
        {
            std::terminate();
        }

        auto& registry = m_Scene->m_Registry->Registry;
        registry.remove<T>(Detail::ToEnttEntity(m_EntityHandle));
    }

    template IDComponent& Entity::AddComponent<IDComponent>();
    template TagComponent& Entity::AddComponent<TagComponent>();
    template TransformComponent& Entity::AddComponent<TransformComponent>();
    template MeshComponent& Entity::AddComponent<MeshComponent>();
    template MeshComponent& Entity::AddComponent<MeshComponent, std::shared_ptr<Mesh>&>(std::shared_ptr<Mesh>&);
    template MeshComponent& Entity::AddComponent<MeshComponent, const std::shared_ptr<Mesh>&>(const std::shared_ptr<Mesh>&);
    template MeshComponent& Entity::AddComponent<MeshComponent, std::shared_ptr<Mesh>>(std::shared_ptr<Mesh>&&);
    template ModelComponent& Entity::AddComponent<ModelComponent>();
    template ModelComponent& Entity::AddComponent<ModelComponent, std::shared_ptr<Model>&>(std::shared_ptr<Model>&);
    template ModelComponent& Entity::AddComponent<ModelComponent, const std::shared_ptr<Model>&>(const std::shared_ptr<Model>&);
    template ModelComponent& Entity::AddComponent<ModelComponent, std::shared_ptr<Model>>(std::shared_ptr<Model>&&);
    template CameraComponent& Entity::AddComponent<CameraComponent>();

    template IDComponent& Entity::GetComponent<IDComponent>();
    template TagComponent& Entity::GetComponent<TagComponent>();
    template TransformComponent& Entity::GetComponent<TransformComponent>();
    template MeshComponent& Entity::GetComponent<MeshComponent>();
    template ModelComponent& Entity::GetComponent<ModelComponent>();
    template CameraComponent& Entity::GetComponent<CameraComponent>();

    template bool Entity::HasComponent<IDComponent>() const;
    template bool Entity::HasComponent<TagComponent>() const;
    template bool Entity::HasComponent<TransformComponent>() const;
    template bool Entity::HasComponent<MeshComponent>() const;
    template bool Entity::HasComponent<ModelComponent>() const;
    template bool Entity::HasComponent<CameraComponent>() const;

    template void Entity::RemoveComponent<IDComponent>();
    template void Entity::RemoveComponent<TagComponent>();
    template void Entity::RemoveComponent<TransformComponent>();
    template void Entity::RemoveComponent<MeshComponent>();
    template void Entity::RemoveComponent<ModelComponent>();
    template void Entity::RemoveComponent<CameraComponent>();
}
