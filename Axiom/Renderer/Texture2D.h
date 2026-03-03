#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace Axiom
{
    class Texture2D
    {
    public:
        virtual ~Texture2D() = default;

        virtual void Bind(uint32_t slot = 0) const = 0;

        static std::shared_ptr<Texture2D> CreateFromFile(const std::string& path);
        static std::shared_ptr<Texture2D> CreateFromMemory(const void* data, uint32_t size);
        static std::shared_ptr<Texture2D> CreateFromPixels(const void* data, uint32_t width, uint32_t height, bool srgb = true);
    };
}
