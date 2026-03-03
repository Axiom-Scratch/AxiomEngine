#pragma once

#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

namespace Axiom
{
    enum class ShaderDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    uint32_t ShaderDataTypeSize(ShaderDataType type);

    struct BufferElement
    {
        std::string Name;
        ShaderDataType Type = ShaderDataType::None;
        uint32_t Size = 0;
        uint32_t Offset = 0;
        bool Normalized = false;

        BufferElement() = default;
        BufferElement(ShaderDataType type, std::string name, bool normalized = false);

        uint32_t GetComponentCount() const;
    };

    class BufferLayout
    {
    public:
        BufferLayout() = default;
        BufferLayout(std::initializer_list<BufferElement> elements);

        [[nodiscard]] uint32_t GetStride() const;
        [[nodiscard]] const std::vector<BufferElement>& GetElements() const;

        std::vector<BufferElement>::iterator begin();
        std::vector<BufferElement>::iterator end();
        std::vector<BufferElement>::const_iterator begin() const;
        std::vector<BufferElement>::const_iterator end() const;

    private:
        void CalculateOffsetsAndStride();

        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };

    class RHIVertexBuffer
    {
    public:
        virtual ~RHIVertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static std::unique_ptr<RHIVertexBuffer> Create(const float* vertices, uint32_t size);
    };

    class RHIIndexBuffer
    {
    public:
        virtual ~RHIIndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        [[nodiscard]] virtual uint32_t GetCount() const = 0;

        static std::unique_ptr<RHIIndexBuffer> Create(const uint32_t* indices, uint32_t count);
    };
}
