#pragma once
#include <string>

namespace Cube {
    class Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;
        virtual std::string getName() const;

        virtual void onAttach() {}  // ���ջʱ����
        virtual void onDetach() {}  // ��ջʱ����
        virtual void onEvent() {}
        virtual void onUpdate(float deltaTime) {}

    private:
        std::string name;
    };
}  // namespace Cube