#include "Core/UUID.h"

#include <random>

namespace Axiom
{
    UUID::UUID()
    {
        static std::random_device randomDevice;
        static std::mt19937_64 generator(randomDevice());
        static std::uniform_int_distribution<uint64_t> distribution;
        m_UUID = distribution(generator);
    }

    UUID::UUID(uint64_t value)
        : m_UUID(value)
    {
    }
}
