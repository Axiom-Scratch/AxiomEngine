#pragma once

#include "Scene/Components.h"
#include "Scene/Scene.h"

#include <cstdint>

namespace Axiom
{
    class Entity
    {
    public:
        using Handle = std::uint32_t;

        Entity() = default;
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args);

        template<typename T>
        T& GetComponent();

        template<typename T>
        bool HasComponent() const;

        template<typename T>
        void RemoveComponent();

        operator bool() const;

    private:
        friend class Scene;

        Entity(Handle handle, Scene* scene);
        bool IsValid() const;

        Handle m_EntityHandle = 0;
        Scene* m_Scene = nullptr;
    };

    extern template IDComponent& Entity::AddComponent<IDComponent>();
    extern template TagComponent& Entity::AddComponent<TagComponent>();
    extern template TransformComponent& Entity::AddComponent<TransformComponent>();
    extern template MeshComponent& Entity::AddComponent<MeshComponent>();
    extern template MeshComponent& Entity::AddComponent<MeshComponent, std::shared_ptr<Mesh>&>(std::shared_ptr<Mesh>&);
    extern template MeshComponent& Entity::AddComponent<MeshComponent, const std::shared_ptr<Mesh>&>(const std::shared_ptr<Mesh>&);
    extern template MeshComponent& Entity::AddComponent<MeshComponent, std::shared_ptr<Mesh>>(std::shared_ptr<Mesh>&&);
    extern template ModelComponent& Entity::AddComponent<ModelComponent>();
    extern template ModelComponent& Entity::AddComponent<ModelComponent, std::shared_ptr<Model>&>(std::shared_ptr<Model>&);
    extern template ModelComponent& Entity::AddComponent<ModelComponent, const std::shared_ptr<Model>&>(const std::shared_ptr<Model>&);
    extern template ModelComponent& Entity::AddComponent<ModelComponent, std::shared_ptr<Model>>(std::shared_ptr<Model>&&);
    extern template CameraComponent& Entity::AddComponent<CameraComponent>();

    extern template IDComponent& Entity::GetComponent<IDComponent>();
    extern template TagComponent& Entity::GetComponent<TagComponent>();
    extern template TransformComponent& Entity::GetComponent<TransformComponent>();
    extern template MeshComponent& Entity::GetComponent<MeshComponent>();
    extern template ModelComponent& Entity::GetComponent<ModelComponent>();
    extern template CameraComponent& Entity::GetComponent<CameraComponent>();

    extern template bool Entity::HasComponent<IDComponent>() const;
    extern template bool Entity::HasComponent<TagComponent>() const;
    extern template bool Entity::HasComponent<TransformComponent>() const;
    extern template bool Entity::HasComponent<MeshComponent>() const;
    extern template bool Entity::HasComponent<ModelComponent>() const;
    extern template bool Entity::HasComponent<CameraComponent>() const;

    extern template void Entity::RemoveComponent<IDComponent>();
    extern template void Entity::RemoveComponent<TagComponent>();
    extern template void Entity::RemoveComponent<TransformComponent>();
    extern template void Entity::RemoveComponent<MeshComponent>();
    extern template void Entity::RemoveComponent<ModelComponent>();
    extern template void Entity::RemoveComponent<CameraComponent>();
}
