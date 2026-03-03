#pragma once

#include <cstdint>
#include <functional>

namespace Axiom
{
    class UUID
    {
    public:
        UUID();
        explicit UUID(uint64_t value);

        bool operator==(const UUID&) const = default;
        explicit operator uint64_t() const { return m_UUID; }

    private:
        uint64_t m_UUID = 0;
    };
}

namespace std
{
    template<>
    struct hash<Axiom::UUID>
    {
        size_t operator()(const Axiom::UUID& uuid) const noexcept
        {
            return hash<uint64_t>()(static_cast<uint64_t>(uuid));
        }
    };
}
