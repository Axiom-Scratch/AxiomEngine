#pragma once

#include <entt/entt.hpp>

#include <cstdint>
#include <type_traits>

namespace Axiom::Detail
{
    using EntityHandle = std::uint32_t;

    struct SceneRegistry
    {
        entt::registry Registry;
    };

    constexpr EntityHandle InvalidEntity = static_cast<EntityHandle>(entt::null);

    static_assert(std::is_same_v<std::underlying_type_t<entt::entity>, EntityHandle>,
        "entt::entity underlying type must match EntityHandle");

    inline entt::entity ToEnttEntity(EntityHandle handle)
    {
        return static_cast<entt::entity>(handle);
    }

    inline EntityHandle ToEntityHandle(entt::entity entity)
    {
        return static_cast<EntityHandle>(entity);
    }
}
