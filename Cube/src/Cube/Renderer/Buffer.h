#pragma once
#include "BufferLayout.h"

namespace Cube {

    class VertexBuffer {
    public:
        VertexBuffer(const std::vector<float>& vertices, uint32_t usage = GL_STATIC_DRAW);
        ~VertexBuffer();
        void bind();
        static void unbind();

        void setLayout(const BufferLayout& layout);
        const BufferLayout& getLayout() const;

    private:
        uint32_t id;
        BufferLayout layout;
    };

    class IndexBuffer {
    public:
        IndexBuffer(const std::vector<uint32_t>& indices, uint32_t usage = GL_STATIC_DRAW);
        ~IndexBuffer();
        void bind();
        static void unbind();
        uint32_t getCount() const;

    private:
        uint32_t id;
        uint32_t count;
    };

}  // namespace Cube