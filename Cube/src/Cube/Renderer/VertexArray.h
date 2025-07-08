#pragma once
#include <memory>
#include <vector>
#include "Buffer.h"

namespace Cube {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();
        void bind();
        static void unbind();
        void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vb);
        void setIndexBuffer(const std::shared_ptr<IndexBuffer>& ib);
    private:
        uint32_t id;
        std::vector<std::shared_ptr<VertexBuffer>> vbs;
        std::shared_ptr<IndexBuffer> ib;
        uint32_t vertexBufferIndex;
    };

}